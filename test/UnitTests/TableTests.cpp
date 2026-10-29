#include "Common/MdbStructs.h"
#include "LockBased/MdbTables.h"
#include "Rcu/PointerSwap/Mdb.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/Mdb.h"
#include "Rcu/StableRow/StableRowTables.h"
#include "TestSupport/TestSupport.h"

#include "doctest/doctest.h"

#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <optional>
#include <thread>
#include <vector>

namespace
{
	using Mdb::AccountIdType;
	using Mdb::DateType;
	using Mdb::ExchangeIdType;
	using Mdb::InstrumentIdType;
	using Mdb::Order;
	using Mdb::PosiDirectionType;
	using Mdb::Position;
	using Mdb::PrimaryAccount;
	using Mdb::Product;
	using Mdb::TradingDay;
	using TestSupport::FillFixedString;
	using TestSupport::FillTradingDay;
	using TestSupport::HasPairedDates;

	constexpr std::int32_t PairedPrimaryKey = 1;
	constexpr std::int32_t SharedOfferId = 10;
	constexpr std::int32_t UniqueOfferId = 20;
	constexpr std::int32_t MovedOfferId = 30;
	constexpr std::int32_t BatchOfferId = 40;
	constexpr int ReaderThreadCount = 3;
	constexpr int ConcurrentUpdateCount = 20000;
	constexpr int BulkRowCount = 2000;
	constexpr int BatchInsertRowCount = 128;

	template <typename Iterator>
	int CountIterators(Iterator first, Iterator last)
	{
		int count = 0;
		for (; first != last; ++first)
		{
			++count;
		}
		return count;
	}

	template <typename Table, typename Record>
	bool InsertRecord(Table& table, const Record& record)
	{
		Record* row = Record::Allocate();
		*row = record;
		return table.Insert(row);
	}

	template <typename Table>
	int CountPrimaryKeyRows(Table& table)
	{
		auto range = table.PrimaryKey->SelectAll();
		return CountIterators(range.begin(), range.end());
	}

	template <typename Table>
	int CountOfferId(Table& table, std::int32_t offerId)
	{
		auto rangeLease = table.OfferIdIndex->EqualRange(offerId);
		return CountIterators(rangeLease.begin(), rangeLease.end());
	}

	template <typename Table>
	std::optional<Order> SelectOrderByClientOrderId(Table& table, const Order& key)
	{
		return table.ClientOrderIdUniqueKey->Select(key.TradingDay, key.AccountId, key.ExchangeId,
			key.InstrumentId, key.SessionId, key.ClientOrderId);
	}

	void FillPrimaryAccount(PrimaryAccount& record, const char* primaryAccountId, std::int32_t offerId, const char* name)
	{
		FillFixedString(record.PrimaryAccountId, primaryAccountId);
		FillFixedString(record.PrimaryAccountName, name);
		record.OfferId = offerId;
	}

	PrimaryAccount MakePrimaryAccount(const char* primaryAccountId, std::int32_t offerId)
	{
		PrimaryAccount record{};
		FillPrimaryAccount(record, primaryAccountId, offerId, primaryAccountId);
		return record;
	}

	Order MakeOrder(const char* tradingDay, const char* accountId, const char* exchangeId, const char* instrumentId,
		std::int32_t orderId, std::int64_t sessionId, std::int32_t clientOrderId)
	{
		Order record{};
		FillFixedString(record.TradingDay, tradingDay);
		FillFixedString(record.AccountId, accountId);
		FillFixedString(record.ExchangeId, exchangeId);
		FillFixedString(record.InstrumentId, instrumentId);
		record.OrderId = orderId;
		record.SessionId = sessionId;
		record.ClientOrderId = clientOrderId;
		return record;
	}

	bool HasDate(const std::optional<TradingDay>& record, const char* expectedDate)
	{
		return record.has_value() && std::strcmp(record->CurrTradingDay, expectedDate) == 0;
	}

	// 事件里的记录归事件所有：留到最后一起释放，Erase 若错拿会被回收的活记录这里就会暴露
	class CountingSubscriber : public DbAdapters::MdbSubscriber
	{
	public:
		void OnRowChange(const DbAdapters::RowChangeEventPtr& event) override
		{
			if (event == nullptr)
			{
				return;
			}
			if (event->Type == DbAdapters::RowChangeType::Insert)
			{
				++insertCount_;
			}
			if (event->Type == DbAdapters::RowChangeType::Erase)
			{
				++eraseCount_;
			}
			retainedEvents_.push_back(event);
		}

		void OnRowBatchChange(const std::vector<DbAdapters::RowChangeEventPtr>& events) override
		{
			++batchCount_;
			lastBatchSize_ = static_cast<int>(events.size());
		}

		int insertCount_ = 0;
		int eraseCount_ = 0;
		int batchCount_ = 0;
		int lastBatchSize_ = 0;
		std::vector<DbAdapters::RowChangeEventPtr> retainedEvents_;
	};

	// 副本订阅者：快照与增量走不同入口，接收方靠「快照开始 → 批量 → 快照完成」的顺序判断何时可以入队
	class CountingReplicaSubscriber : public DbAdapters::MdbReplicaSubscriber
	{
	public:
		void OnRowChange(const DbAdapters::RowChangeEventPtr& event) override
		{
			++changeCount_;
			lastTableId_ = event->TableId;
		}

		void OnRowBatchChange(const std::vector<DbAdapters::RowChangeEventPtr>& events) override
		{
			++batchCount_;
			lastBatchSize_ = static_cast<int>(events.size());
		}

		void OnSnapshotBegin(unsigned int tableId) override
		{
			snapshotBeginTableId_ = tableId;
			snapshotCompleted_ = false;
		}

		void OnSnapshotComplete(unsigned int tableId) override
		{
			snapshotCompleteTableId_ = tableId;
			snapshotCompleted_ = true;
		}

		int changeCount_ = 0;
		int batchCount_ = 0;
		int lastBatchSize_ = 0;
		unsigned int lastTableId_ = 0;
		unsigned int snapshotBeginTableId_ = 0;
		unsigned int snapshotCompleteTableId_ = 0;
		bool snapshotCompleted_ = false;
	};

	template <typename Table>
	void CheckSingleRowInterface(Table& table)
	{
		TradingDay* firstRow = TradingDay::Allocate();
		FillTradingDay(*firstRow, 1, 100);
		CHECK(table.Insert(firstRow));
		CHECK(HasDate(table.PrimaryKey->Select(1), "100"));

		std::optional<TradingDay> detached = table.PrimaryKey->Select(1);
		REQUIRE(detached.has_value());
		std::snprintf(detached->CurrTradingDay, sizeof(detached->CurrTradingDay), "999");
		CHECK(HasDate(table.PrimaryKey->Select(1), "100"));

		TradingDay* duplicateRow = TradingDay::Allocate();
		FillTradingDay(*duplicateRow, 1, 200);
		// 被拒绝的记录由 Insert 自行归还对象池
		CHECK(!table.Insert(duplicateRow));
		CHECK(HasDate(table.PrimaryKey->Select(1), "100"));

		TradingDay updatedRecord{};
		FillTradingDay(updatedRecord, 1, 300);
		CHECK(table.Update(updatedRecord));
		CHECK(HasDate(table.PrimaryKey->Select(1), "300"));

		TradingDay missingRecord{};
		missingRecord.Pk = 777;
		CHECK(!table.Update(missingRecord));

		TradingDay eraseKey{};
		eraseKey.Pk = 1;
		CHECK(table.Erase(eraseKey));
		CHECK(!table.PrimaryKey->Select(1).has_value());
		CHECK(!table.Erase(eraseKey));
	}

	template <typename Table>
	void CheckBulkAndRange(Table& table)
	{
		bool allInserted = true;
		for (int index = 0; index < BulkRowCount; ++index)
		{
			TradingDay* row = TradingDay::Allocate();
			FillTradingDay(*row, 1000 + index, index);
			allInserted = table.Insert(row) && allInserted;
		}
		CHECK(allInserted);
		CHECK(table.PrimaryKey->Select(1000).has_value());
		CHECK(table.PrimaryKey->Select(1000 + BulkRowCount - 1).has_value());

		bool allErased = true;
		for (int index = 0; index < BulkRowCount; index += 2)
		{
			TradingDay key{};
			key.Pk = 1000 + index;
			allErased = table.Erase(key) && allErased;
		}
		CHECK(allErased);
		CHECK(!table.PrimaryKey->Select(1000).has_value());
		CHECK(table.PrimaryKey->Select(1001).has_value());

		int liveCount = 0;
		table.ForEach([&liveCount](TradingDay*)
		{
			++liveCount;
			return true;
		});
		CHECK(liveCount == BulkRowCount / 2);

		std::atomic<bool> writerFinished{ false };
		std::thread writer;
		{
			auto rangeLease = table.PrimaryKey->SelectAll();
			int leaseCount = 0;
			for (auto it = rangeLease.begin(); it != rangeLease.end(); ++it)
			{
				++leaseCount;
			}
			CHECK(leaseCount == BulkRowCount / 2);

			writer = std::thread([&table, &writerFinished]()
			{
				TradingDay row{};
				FillTradingDay(row, 1001, 7);
				[[maybe_unused]] const bool updated = table.Update(row);
				writerFinished.store(true);
			});
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			CHECK(!writerFinished.load());
		}
		writer.join();
		CHECK(writerFinished.load());
	}

	template <typename Table>
	void CheckSubscribers(Table& table)
	{
		CountingSubscriber subscriber;
		table.Subscribe(&subscriber);

		TradingDay* eventRow = TradingDay::Allocate();
		FillTradingDay(*eventRow, 9000, 1);
		CHECK(table.Insert(eventRow));

		TradingDay eventKey{};
		eventKey.Pk = 9000;
		CHECK(table.Erase(eventKey));
		table.UnSubscribe();

		CHECK(subscriber.insertCount_ == 1);
		CHECK(subscriber.eraseCount_ == 1);
		REQUIRE(subscriber.retainedEvents_.size() == 2);

		bool eraseEventCarriesRowCopy = false;
		for (const DbAdapters::RowChangeEventPtr& event : subscriber.retainedEvents_)
		{
			if (event->Type == DbAdapters::RowChangeType::Erase && event->Record.Get() != nullptr)
			{
				eraseEventCarriesRowCopy = static_cast<const TradingDay*>(event->Record.Get())->Pk == 9000;
			}
		}
		CHECK(eraseEventCarriesRowCopy);
	}

	template <typename Table>
	void CheckConcurrentSelect(Table& table)
	{
		TradingDay* pairedRow = TradingDay::Allocate();
		FillTradingDay(*pairedRow, PairedPrimaryKey, 0);
		CHECK(table.Insert(pairedRow));

		std::atomic<bool> stopReaders{ false };
		std::atomic<int> tornReadCount{ 0 };
		std::atomic<long long> readCount{ 0 };
		std::vector<std::thread> readers;
		for (int readerIndex = 0; readerIndex < ReaderThreadCount; ++readerIndex)
		{
			readers.emplace_back([&table, &stopReaders, &tornReadCount, &readCount]()
			{
				while (!stopReaders.load(std::memory_order_relaxed))
				{
					const std::optional<TradingDay> copy = table.PrimaryKey->Select(PairedPrimaryKey);
					if (!copy.has_value() || !HasPairedDates(*copy))
					{
						++tornReadCount;
					}
					++readCount;
				}
			});
		}

		for (int index = 0; index < ConcurrentUpdateCount; ++index)
		{
			TradingDay row{};
			FillTradingDay(row, PairedPrimaryKey, index);
			[[maybe_unused]] const bool updated = table.Update(row);
		}
		stopReaders.store(true);
		for (std::thread& reader : readers)
		{
			reader.join();
		}

		CHECK(tornReadCount.load() == 0);
		CHECK(readCount.load() > 0);
	}

	template <typename Table>
	void CheckTruncateClearsTable(Table& table)
	{
		TradingDay* row = TradingDay::Allocate();
		FillTradingDay(*row, PairedPrimaryKey, 0);
		CHECK(table.Insert(row));

		table.TruncateTable();
		int liveCountAfterTruncate = 0;
		table.ForEach([&liveCountAfterTruncate](TradingDay*)
		{
			++liveCountAfterTruncate;
			return true;
		});
		CHECK(liveCountAfterTruncate == 0);
		CHECK(!table.PrimaryKey->Select(PairedPrimaryKey).has_value());
	}

	// 批量插入是独立入口：所有权整批移交、事件以单次批量下发，不逐行发 OnRowChange
	template <typename Table>
	void CheckBatchInsert(Table& table)
	{
		CountingSubscriber subscriber;
		table.Subscribe(&subscriber);

		std::vector<TradingDay*> records;
		records.reserve(BatchInsertRowCount);
		for (int index = 0; index < BatchInsertRowCount; ++index)
		{
			TradingDay* row = TradingDay::Allocate();
			FillTradingDay(*row, 20000 + index, index);
			records.push_back(row);
		}
		table.BatchInsert(records);
		table.UnSubscribe();

		CHECK(records.empty());
		CHECK(table.PrimaryKey->Select(20000).has_value());
		CHECK(table.PrimaryKey->Select(20000 + BatchInsertRowCount - 1).has_value());
		CHECK(CountPrimaryKeyRows(table) == BatchInsertRowCount);
		CHECK(subscriber.batchCount_ == 1);
		CHECK(subscriber.lastBatchSize_ == BatchInsertRowCount);
		CHECK(subscriber.insertCount_ == 0);
	}

	// InitDb 的快照要把在册行整批交给副本，之后单行变更才走 OnRowChange
	template <typename Table>
	void CheckReplicaSubscriber(Table& table)
	{
		TradingDay* snapshotRow = TradingDay::Allocate();
		FillTradingDay(*snapshotRow, 40001, 1);
		CHECK(table.Insert(snapshotRow));

		CountingReplicaSubscriber replica;
		table.SubscribeReplica(&replica);
		table.InitDb();

		CHECK(replica.snapshotBeginTableId_ == TradingDay::TableId);
		CHECK(replica.snapshotCompleteTableId_ == TradingDay::TableId);
		CHECK(replica.snapshotCompleted_);
		CHECK(replica.batchCount_ == 1);
		CHECK(replica.lastBatchSize_ == 1);
		CHECK(replica.changeCount_ == 0);

		TradingDay* changeRow = TradingDay::Allocate();
		FillTradingDay(*changeRow, 40002, 2);
		CHECK(table.Insert(changeRow));
		CHECK(replica.changeCount_ == 1);
		CHECK(replica.lastTableId_ == TradingDay::TableId);

		table.UnSubscribe();

		TradingDay* afterUnsubscribeRow = TradingDay::Allocate();
		FillTradingDay(*afterUnsubscribeRow, 40003, 3);
		CHECK(table.Insert(afterUnsubscribeRow));
		CHECK(replica.changeCount_ == 1);
	}

	// 批量插入要顺手登记索引：否则行进了主键、索引里查不到
	template <typename Table>
	void CheckBatchInsertKeepsIndexedRows(Table& table)
	{
		std::vector<PrimaryAccount*> records;
		records.reserve(BatchInsertRowCount);
		for (int index = 0; index < BatchInsertRowCount; ++index)
		{
			char primaryAccountId[32];
			std::snprintf(primaryAccountId, sizeof(primaryAccountId), "BATCH%d", index);
			PrimaryAccount* row = PrimaryAccount::Allocate();
			FillPrimaryAccount(*row, primaryAccountId, BatchOfferId, primaryAccountId);
			records.push_back(row);
		}
		table.BatchInsert(records);

		CHECK(records.empty());
		CHECK(CountPrimaryKeyRows(table) == BatchInsertRowCount);
		CHECK(CountOfferId(table, BatchOfferId) == BatchInsertRowCount);
	}

	// 唯一键同理：批量插入漏登记，后续按键查不到、同键还能再插一份
	template <typename Table>
	void CheckBatchInsertKeepsUniqueKey(Table& table)
	{
		std::vector<Order*> records;
		records.reserve(BatchInsertRowCount);
		for (int index = 0; index < BatchInsertRowCount; ++index)
		{
			Order* row = Order::Allocate();
			*row = MakeOrder("20261029", "ACC1", "SHFE", "cu2601", index + 1, 1, 8000 + index);
			records.push_back(row);
		}
		table.BatchInsert(records);

		CHECK(records.empty());
		CHECK(CountPrimaryKeyRows(table) == BatchInsertRowCount);
		CHECK(SelectOrderByClientOrderId(table, MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 1, 1, 8000)).has_value());
		CHECK(SelectOrderByClientOrderId(table,
			MakeOrder("20261029", "ACC1", "SHFE", "cu2601", BatchInsertRowCount, 1,
				8000 + BatchInsertRowCount - 1)).has_value());
	}

	// 两个 RCU 形态：B（换指针）不生成索引与唯一键，B2（稳定行地址）两者都生成
	namespace StableRow
	{
		using Mdb::Rcu::StableRow::OrderTable;
		using Mdb::Rcu::StableRow::PrimaryAccountTable;
		using Mdb::Rcu::StableRow::PrimaryAccountTableIndexOfferId;

		// 只有 StableRow 生成二级索引：B 形态按形态分工一律忽略 indexes
		void CheckPrimaryAccountIndex()
		{
			PrimaryAccountTable table;
			CountingSubscriber subscriber;
			table.Subscribe(&subscriber);

			bool allInserted = true;
			const char* const accountIds[] = { "A", "B", "C" };
			const std::int32_t offerIds[] = { SharedOfferId, UniqueOfferId, SharedOfferId };
			for (int index = 0; index < 3; ++index)
			{
				PrimaryAccount row{};
				FillPrimaryAccount(row, accountIds[index], offerIds[index], accountIds[index]);
				allInserted = InsertRecord(table, row) && allInserted;
			}
			CHECK(allInserted);
			CHECK(CountOfferId(table, SharedOfferId) == 2);
			CHECK(CountOfferId(table, UniqueOfferId) == 1);

			{
				auto rangeLease = table.OfferIdIndex->Range(SharedOfferId, UniqueOfferId);
				CHECK(CountIterators(rangeLease.begin(), rangeLease.end()) == 3);
			}

			// 改索引键：节点必须在索引中重排，主键查询读到新值
			PrimaryAccount movedRecord{};
			FillPrimaryAccount(movedRecord, "C", MovedOfferId, "C");
			CHECK(table.Update(movedRecord));
			AccountIdType movedAccountKey{};
			FillFixedString(movedAccountKey, "C");
			const std::optional<PrimaryAccount> updatedRecord = table.PrimaryKey->Select(movedAccountKey);
			REQUIRE(updatedRecord.has_value());
			CHECK(updatedRecord->OfferId == MovedOfferId);
			CHECK(CountOfferId(table, SharedOfferId) == 1);
			CHECK(CountOfferId(table, MovedOfferId) == 1);

			// 改非索引字段：节点不动，计数与键都保持
			PrimaryAccount renamedRecord{};
			FillPrimaryAccount(renamedRecord, "A", SharedOfferId, "renamed");
			CHECK(table.Update(renamedRecord));
			CHECK(CountOfferId(table, SharedOfferId) == 1);

			CHECK(table.EraseByOfferIdIndex(UniqueOfferId) == 1);
			CHECK(CountOfferId(table, UniqueOfferId) == 0);
			AccountIdType erasedAccountKey{};
			FillFixedString(erasedAccountKey, "B");
			CHECK(!table.PrimaryKey->Select(erasedAccountKey).has_value());
			table.UnSubscribe();

			bool eraseByIndexEventFound = false;
			for (const DbAdapters::RowChangeEventPtr& event : subscriber.retainedEvents_)
			{
				if (event->Type != DbAdapters::RowChangeType::EraseByIndex)
				{
					continue;
				}
				const PrimaryAccount* eventRecord = static_cast<const PrimaryAccount*>(event->Record.Get());
				eraseByIndexEventFound = event->IndexId == PrimaryAccountTableIndexOfferId::IndexID
					&& eventRecord != nullptr && eventRecord->OfferId == UniqueOfferId;
			}
			CHECK(eraseByIndexEventFound);

			table.TruncateTable();
			CHECK(CountOfferId(table, SharedOfferId) == 0);
			AccountIdType keptAccountKey{};
			FillFixedString(keptAccountKey, "A");
			CHECK(!table.PrimaryKey->Select(keptAccountKey).has_value());
		}

		// 唯一键只在 StableRow 生成：B 形态不维护任何辅助结构
		void CheckOrderUniqueKey()
		{
			OrderTable table;

			const Order firstOrder = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 100);
			CHECK(InsertRecord(table, firstOrder));

			DateType tradingDay{};
			FillFixedString(tradingDay, "20261027");
			AccountIdType accountId{};
			FillFixedString(accountId, "ACC1");
			ExchangeIdType exchangeId{};
			FillFixedString(exchangeId, "CFFEX");
			InstrumentIdType instrumentId{};
			FillFixedString(instrumentId, "IF2612");
			const auto selectOrder = [&table, &tradingDay, &accountId, &exchangeId, &instrumentId](std::int32_t clientOrderId)
			{
				return table.ClientOrderIdUniqueKey->Select(tradingDay, accountId, exchangeId, instrumentId, 1, clientOrderId);
			};
			CHECK(selectOrder(100).has_value());

			// 主键不同，六个唯一键字段全同
			const Order duplicateOrder = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 2, 1, 100);
			CHECK(!InsertRecord(table, duplicateOrder));

			const Order changedKey = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 200);
			CHECK(!table.Update(changedKey));
			CHECK(selectOrder(100).has_value());

			Order changedPrice = firstOrder;
			changedPrice.Price = 3800.5;
			CHECK(table.Update(changedPrice));
			const std::optional<Order> updatedOrder = selectOrder(100);
			REQUIRE(updatedOrder.has_value());
			CHECK(updatedOrder->Price == 3800.5);

			CHECK(table.Erase(MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 100)));
			CHECK(!selectOrder(100).has_value());

			CHECK(InsertRecord(table, duplicateOrder));
		}
	}

	// 读写锁版的表级直连：索引 / 唯一键 / 复合主键 / 清表 / 导出
	namespace LockBased
	{
		using Mdb::OrderTable;
		using Mdb::PositionTable;
		using Mdb::PrimaryAccountTable;
		using Mdb::ProductTable;

		Product MakeProduct(const char* exchangeId, const char* productId, const char* productName)
		{
			Product record{};
			FillFixedString(record.ExchangeId, exchangeId);
			FillFixedString(record.ProductId, productId);
			FillFixedString(record.ProductName, productName);
			return record;
		}

		std::optional<Product> SelectProduct(ProductTable& table, const char* exchangeId, const char* productId)
		{
			Product key = MakeProduct(exchangeId, productId, "");
			return table.PrimaryKey->Select(key.ExchangeId, key.ProductId);
		}

		Position MakePositionKey(const char* tradingDay, const char* accountId)
		{
			Position key{};
			FillFixedString(key.TradingDay, tradingDay);
			FillFixedString(key.AccountId, accountId);
			return key;
		}

		Position MakePosition(const char* tradingDay, const char* accountId, const char* exchangeId,
			const char* instrumentId, PosiDirectionType posiDirection)
		{
			Position record = MakePositionKey(tradingDay, accountId);
			FillFixedString(record.ExchangeId, exchangeId);
			FillFixedString(record.InstrumentId, instrumentId);
			record.PosiDirection = posiDirection;
			return record;
		}

		std::optional<Position> SelectPosition(PositionTable& table, const Position& key)
		{
			return table.PrimaryKey->Select(key.TradingDay, key.AccountId, key.ExchangeId, key.InstrumentId,
				key.PosiDirection);
		}

		int CountAccountIndex(PositionTable& table, const char* tradingDay, const char* accountId)
		{
			Position key = MakePositionKey(tradingDay, accountId);
			auto range = table.AccountIndex->EqualRange(key.TradingDay, key.AccountId);
			return CountIterators(range.begin(), range.end());
		}

		int CountTradingDayIndex(PositionTable& table, const char* tradingDay)
		{
			Position key = MakePositionKey(tradingDay, "");
			auto range = table.TradingDayIndex->EqualRange(key.TradingDay);
			return CountIterators(range.begin(), range.end());
		}

		int EraseByAccountIndex(PositionTable& table, const char* tradingDay, const char* accountId)
		{
			Position key = MakePositionKey(tradingDay, accountId);
			return table.EraseByAccountIndex(key.TradingDay, key.AccountId);
		}

		// 复合字符串主键：Product 按 (ExchangeId, ProductId) 定行，Update 只能改非键字段
		void CheckCompositeKeyTable()
		{
			ProductTable table;

			CHECK(InsertRecord(table, MakeProduct("SHFE", "cu2601", "Copper2601")));
			CHECK(SelectProduct(table, "SHFE", "cu2601").has_value());
			CHECK(!SelectProduct(table, "CFFEX", "cu2601").has_value());

			CHECK(!InsertRecord(table, MakeProduct("SHFE", "cu2601", "Duplicate")));
			CHECK(InsertRecord(table, MakeProduct("SHFE", "ag2601", "Silver2601")));

			CHECK(table.Update(MakeProduct("SHFE", "cu2601", "Copper2601Renamed")));
			const std::optional<Product> renamedRow = SelectProduct(table, "SHFE", "cu2601");
			REQUIRE(renamedRow.has_value());
			CHECK(std::strcmp(renamedRow->ProductName, "Copper2601Renamed") == 0);

			const Product eraseKey = MakeProduct("SHFE", "cu2601", "");
			CHECK(table.Erase(eraseKey));
			CHECK(!SelectProduct(table, "SHFE", "cu2601").has_value());
			CHECK(SelectProduct(table, "SHFE", "ag2601").has_value());
			CHECK(!table.Erase(eraseKey));
		}

		// 多列二级索引：重复键全部保留，按索引批量删要连带摘主键
		void CheckPositionAccountIndex()
		{
			PositionTable table;

			const bool allInserted =
				InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long))
				&& InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Long))
				&& InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Short))
				&& InsertRecord(table, MakePosition("20261030", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long));
			CHECK(allInserted);
			CHECK(SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long)).has_value());
			CHECK(SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Short)).has_value());

			CHECK(CountAccountIndex(table, "20261029", "ACC1") == 3);
			CHECK(CountTradingDayIndex(table, "20261029") == 3);
			CHECK(CountTradingDayIndex(table, "20261030") == 1);

			CHECK(EraseByAccountIndex(table, "20261029", "ACC1") == 3);
			CHECK(!SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Long)).has_value());
			CHECK(CountAccountIndex(table, "20261029", "ACC1") == 0);
			CHECK(SelectPosition(table,
				MakePosition("20261030", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long)).has_value());
			CHECK(EraseByAccountIndex(table, "20261029", "ACC1") == 0);
		}

		// 索引列不在主键里：Update 改它要把记录在新旧键之间挪动，挪不动二叉搜索就会漏查
		void CheckPrimaryAccountIndexReorder()
		{
			PrimaryAccountTable table;

			const bool allInserted = InsertRecord(table, MakePrimaryAccount("ACC1", 10))
				&& InsertRecord(table, MakePrimaryAccount("ACC2", 20))
				&& InsertRecord(table, MakePrimaryAccount("ACC3", 30));
			CHECK(allInserted);
			CHECK(CountOfferId(table, 10) == 1);
			CHECK(CountOfferId(table, 20) == 1);
			CHECK(CountOfferId(table, 30) == 1);

			CHECK(table.Update(MakePrimaryAccount("ACC1", 90)));
			CHECK(CountOfferId(table, 10) == 0);
			CHECK(CountOfferId(table, 90) == 1);
			CHECK(CountOfferId(table, 20) == 1);

			PrimaryAccount eraseKey{};
			FillFixedString(eraseKey.PrimaryAccountId, "ACC1");
			CHECK(table.Erase(eraseKey));
			CHECK(CountOfferId(table, 90) == 0);
		}

		// TruncateTable 必须把主键、索引、唯一键三份登记一起清掉，否则同键再插会撞旧登记
		void CheckTruncateClearsRegistrations()
		{
			PositionTable positionTable;
			const Position longPosition = MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long);
			const Position shortPosition = MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Short);
			CHECK(InsertRecord(positionTable, longPosition));
			CHECK(InsertRecord(positionTable, shortPosition));
			CHECK(CountAccountIndex(positionTable, "20261029", "ACC1") == 2);

			OrderTable orderTable;
			const Order firstOrder = MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 1, 1, 7001);
			CHECK(InsertRecord(orderTable, firstOrder));
			CHECK(SelectOrderByClientOrderId(orderTable, firstOrder).has_value());

			positionTable.TruncateTable();
			orderTable.TruncateTable();
			CHECK(CountPrimaryKeyRows(positionTable) == 0);
			CHECK(CountAccountIndex(positionTable, "20261029", "ACC1") == 0);
			CHECK(CountPrimaryKeyRows(orderTable) == 0);
			CHECK(!SelectOrderByClientOrderId(orderTable, firstOrder).has_value());

			CHECK(InsertRecord(positionTable, longPosition));
			CHECK(CountAccountIndex(positionTable, "20261029", "ACC1") == 1);
			CHECK(InsertRecord(orderTable, firstOrder));
			CHECK(SelectOrderByClientOrderId(orderTable, firstOrder).has_value());
		}

		void CheckDumpWritesRows()
		{
			OrderTable table;
			CHECK(InsertRecord(table, MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 1, 1, 7001)));
			CHECK(InsertRecord(table, MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 2, 1, 7002)));

			table.Dump(".");
			FILE* dumpFile = std::fopen(".//Orders.csv", "r");
			REQUIRE(dumpFile != nullptr);

			int lineCount = 0;
			bool headerMatched = false;
			char line[4096];
			while (std::fgets(line, sizeof(line), dumpFile) != nullptr)
			{
				headerMatched = headerMatched
					|| (lineCount == 0 && std::strncmp(line, "TradingDay,AccountId", 20) == 0);
				++lineCount;
			}
			std::fclose(dumpFile);
			CHECK(headerMatched);
			CHECK(lineCount == 3);
		}
	}
}

// 通用场景：三种实现走同一套断言，只有指针换 / 稳定行地址的实现差异
TEST_CASE_TEMPLATE("单行接口：插入、副本读、改、删", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckSingleRowInterface(table);
}

TEST_CASE_TEMPLATE("批量插入与读租约阻塞写者", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckBulkAndRange(table);
}

TEST_CASE_TEMPLATE("订阅者收到插入与删除事件，删除事件带记录副本", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckSubscribers(table);
}

TEST_CASE_TEMPLATE("并发读不撕裂", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckConcurrentSelect(table);
}

TEST_CASE_TEMPLATE("清表后行与主键查询一起失效", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckTruncateClearsTable(table);
}

TEST_CASE_TEMPLATE("批量插入移交所有权并以单次批量下发事件", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckBatchInsert(table);
}

TEST_CASE_TEMPLATE("副本订阅：先整批快照，再收单行增量", Table,
	Mdb::Rcu::PointerSwap::TradingDayTable, Mdb::Rcu::StableRow::TradingDayTable, Mdb::TradingDayTable)
{
	Table table;
	CheckReplicaSubscriber(table);
}

TEST_CASE_TEMPLATE("聚合只建出申请的表", MdbType, Mdb::Rcu::PointerSwap::Mdb, Mdb::Rcu::StableRow::Mdb)
{
	const unsigned int requestedTableIds[] = { Mdb::TradingDay::TableId };
	const Mdb::TableList tableList{ "aggregate", requestedTableIds, 1 };
	MdbType mdb(tableList);
	CHECK(mdb.TradingDays != nullptr);
	CHECK(mdb.Trades == nullptr);
}

TEST_CASE("StableRow 二级索引：重复键、区间、改键重排、按索引批删")
{
	StableRow::CheckPrimaryAccountIndex();
}

TEST_CASE("StableRow 唯一键：冲突插入与改键被拒、删除后键可复用")
{
	StableRow::CheckOrderUniqueKey();
}

TEST_CASE("StableRow 批量插入登记二级索引")
{
	Mdb::Rcu::StableRow::PrimaryAccountTable table;
	CheckBatchInsertKeepsIndexedRows(table);
}

TEST_CASE("StableRow 批量插入登记唯一键")
{
	Mdb::Rcu::StableRow::OrderTable table;
	CheckBatchInsertKeepsUniqueKey(table);
}

TEST_CASE("LockBased 复合字符串主键：增删改查与主键区分度")
{
	LockBased::CheckCompositeKeyTable();
}

TEST_CASE("LockBased 多列二级索引与按索引批删")
{
	LockBased::CheckPositionAccountIndex();
}

TEST_CASE("LockBased 索引列改值后重排")
{
	LockBased::CheckPrimaryAccountIndexReorder();
}

TEST_CASE("LockBased 清表清掉主键、索引与唯一键三份登记")
{
	LockBased::CheckTruncateClearsRegistrations();
}

TEST_CASE("LockBased 导出 csv 的表头与行数")
{
	LockBased::CheckDumpWritesRows();
}

TEST_CASE("LockBased 批量插入登记二级索引")
{
	Mdb::PrimaryAccountTable table;
	CheckBatchInsertKeepsIndexedRows(table);
}

TEST_CASE("LockBased 批量插入登记唯一键")
{
	Mdb::OrderTable table;
	CheckBatchInsertKeepsUniqueKey(table);
}
