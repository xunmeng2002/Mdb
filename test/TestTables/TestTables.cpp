#include "Common/MdbStructs.h"
#include "LockBased/MdbTables.h"
#include "Rcu/PointerSwap/Mdb.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/Mdb.h"
#include "Rcu/StableRow/StableRowTables.h"
#include "TestSupport/TestSupport.h"

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
	using TestSupport::CheckReporter;
	using TestSupport::FillFixedString;
	using TestSupport::FillTradingDay;
	using TestSupport::HasPairedDates;

	constexpr std::int32_t PairedPrimaryKey = 1;
	constexpr std::int32_t SharedOfferId = 10;
	constexpr std::int32_t UniqueOfferId = 20;
	constexpr std::int32_t MovedOfferId = 30;
	constexpr int ReaderThreadCount = 3;
	constexpr int ConcurrentUpdateCount = 20000;
	constexpr int BulkRowCount = 2000;

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

	void FillPrimaryAccount(PrimaryAccount& record, const char* primaryAccountId, std::int32_t offerId, const char* name)
	{
		FillFixedString(record.PrimaryAccountId, primaryAccountId);
		FillFixedString(record.PrimaryAccountName, name);
		record.OfferId = offerId;
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

		int insertCount_ = 0;
		int eraseCount_ = 0;
		std::vector<DbAdapters::RowChangeEventPtr> retainedEvents_;
	};

	template <typename Table>
	void CheckSingleRowInterface(Table& table, CheckReporter& report)
	{
		TradingDay* firstRow = TradingDay::Allocate();
		FillTradingDay(*firstRow, 1, 100);
		report.Check(table.Insert(firstRow), "insertAccepted");
		report.Check(HasDate(table.PrimaryKey->Select(1), "100"), "selectFound");

		std::optional<TradingDay> detached = table.PrimaryKey->Select(1);
		if (detached.has_value())
		{
			std::snprintf(detached->CurrTradingDay, sizeof(detached->CurrTradingDay), "999");
		}
		report.Check(HasDate(table.PrimaryKey->Select(1), "100"), "selectReturnsDetachedCopy");

		TradingDay* duplicateRow = TradingDay::Allocate();
		FillTradingDay(*duplicateRow, 1, 200);
		// 被拒绝的记录由 Insert 自行归还对象池
		report.Check(!table.Insert(duplicateRow), "duplicateRejected");
		report.Check(HasDate(table.PrimaryKey->Select(1), "100"), "duplicateKeptOriginal");

		TradingDay updatedRecord{};
		FillTradingDay(updatedRecord, 1, 300);
		report.Check(table.Update(updatedRecord), "updateAccepted");
		report.Check(HasDate(table.PrimaryKey->Select(1), "300"), "updateApplied");

		TradingDay missingRecord{};
		missingRecord.Pk = 777;
		report.Check(!table.Update(missingRecord), "updateMissingRejected");

		TradingDay eraseKey{};
		eraseKey.Pk = 1;
		report.Check(table.Erase(eraseKey), "eraseAccepted");
		report.Check(!table.PrimaryKey->Select(1).has_value(), "erasedRowGone");
		report.Check(!table.Erase(eraseKey), "eraseMissingRejected");
	}

	template <typename Table>
	void CheckBulkAndRange(Table& table, CheckReporter& report)
	{
		bool allInserted = true;
		for (int index = 0; index < BulkRowCount; ++index)
		{
			TradingDay* row = TradingDay::Allocate();
			FillTradingDay(*row, 1000 + index, index);
			allInserted = table.Insert(row) && allInserted;
		}
		report.Check(allInserted, "bulkInsertAccepted");
		report.Check(table.PrimaryKey->Select(1000).has_value(), "bulkFirstFoundAfterRebuild");
		report.Check(table.PrimaryKey->Select(1000 + BulkRowCount - 1).has_value(), "bulkLastFound");

		bool allErased = true;
		for (int index = 0; index < BulkRowCount; index += 2)
		{
			TradingDay key{};
			key.Pk = 1000 + index;
			allErased = table.Erase(key) && allErased;
		}
		report.Check(allErased, "bulkEraseAccepted");
		report.Check(!table.PrimaryKey->Select(1000).has_value(), "bulkErasedRowGone");
		report.Check(table.PrimaryKey->Select(1001).has_value(), "keptRowStillReachableAcrossTombstones");

		int liveCount = 0;
		table.ForEach([&liveCount](TradingDay*)
		{
			++liveCount;
			return true;
		});
		report.Check(liveCount == BulkRowCount / 2, "forEachCount");

		std::atomic<bool> writerFinished{ false };
		std::thread writer;
		{
			auto rangeLease = table.PrimaryKey->SelectAll();
			int leaseCount = 0;
			for (auto it = rangeLease.begin(); it != rangeLease.end(); ++it)
			{
				++leaseCount;
			}
			report.Check(leaseCount == BulkRowCount / 2, "selectAllCount");

			writer = std::thread([&table, &writerFinished]()
			{
				TradingDay row{};
				FillTradingDay(row, 1001, 7);
				[[maybe_unused]] const bool updated = table.Update(row);
				writerFinished.store(true);
			});
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			report.Check(!writerFinished.load(), "selectAllBlocksWriter");
		}
		writer.join();
		report.Check(writerFinished.load(), "writerResumedAfterLease");
	}

	template <typename Table>
	void CheckSubscribers(Table& table, CheckReporter& report)
	{
		CountingSubscriber subscriber;
		table.Subscribe(&subscriber);

		TradingDay* eventRow = TradingDay::Allocate();
		FillTradingDay(*eventRow, 9000, 1);
		report.Check(table.Insert(eventRow), "subscribedInsertAccepted");

		TradingDay eventKey{};
		eventKey.Pk = 9000;
		report.Check(table.Erase(eventKey), "subscribedEraseAccepted");
		table.UnSubscribe();

		report.Check(subscriber.insertCount_ == 1, "subscriberSawInsert");
		report.Check(subscriber.eraseCount_ == 1, "subscriberSawErase");
		report.Check(subscriber.retainedEvents_.size() == 2, "subscriberRetainedEvents");

		bool eraseEventCarriesRowCopy = false;
		for (const DbAdapters::RowChangeEventPtr& event : subscriber.retainedEvents_)
		{
			if (event->Type == DbAdapters::RowChangeType::Erase && event->Record.Get() != nullptr)
			{
				eraseEventCarriesRowCopy = static_cast<const TradingDay*>(event->Record.Get())->Pk == 9000;
			}
		}
		report.Check(eraseEventCarriesRowCopy, "eraseEventCarriesRowCopy");
	}

	template <typename Table>
	void CheckConcurrentSelect(Table& table, CheckReporter& report)
	{
		TradingDay* pairedRow = TradingDay::Allocate();
		FillTradingDay(*pairedRow, PairedPrimaryKey, 0);
		report.Check(table.Insert(pairedRow), "pairedRowInserted");

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

		report.Check(tornReadCount.load() == 0, "concurrentNoTornReads");
		report.Check(readCount.load() > 0, "concurrentReadersMadeProgress");
	}

	template <typename Table>
	int RunScenario(const char* variantName)
	{
		CheckReporter report(variantName);
		Table table;

		CheckSingleRowInterface(table, report);
		CheckBulkAndRange(table, report);
		CheckSubscribers(table, report);
		CheckConcurrentSelect(table, report);

		table.TruncateTable();
		int liveCountAfterTruncate = 0;
		table.ForEach([&liveCountAfterTruncate](TradingDay*)
		{
			++liveCountAfterTruncate;
			return true;
		});
		report.Check(liveCountAfterTruncate == 0, "truncateEmptiesTable");
		report.Check(!table.PrimaryKey->Select(PairedPrimaryKey).has_value(), "truncateSelectGone");
		return report.FailureCount();
	}

	template <typename MdbType>
	int CheckAggregate(const char* variantName)
	{
		CheckReporter report(variantName);
		const unsigned int requestedTableIds[] = { TradingDay::TableId };
		const Mdb::TableList tableList{ "aggregate", requestedTableIds, 1 };
		MdbType mdb(tableList);
		report.Check(mdb.TradingDays != nullptr, "aggregateCreatesRequestedTable");
		report.Check(mdb.Trades == nullptr, "aggregateLeavesUnrequestedTable");
		return report.FailureCount();
	}

	// 两个 RCU 形态：B（换指针）不生成索引与唯一键，B2（稳定行地址）两者都生成
	namespace StableRow
	{
		using Mdb::Rcu::StableRow::OrderTable;
		using Mdb::Rcu::StableRow::PrimaryAccountTable;
		using Mdb::Rcu::StableRow::PrimaryAccountTableIndexOfferId;

		// 只有 StableRow 生成二级索引：B 形态按形态分工一律忽略 indexes
		int CheckPrimaryAccountIndex()
		{
			CheckReporter report("StableRowIndex");
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
			report.Check(allInserted, "accountInsertAccepted");
			report.Check(CountOfferId(table, SharedOfferId) == 2, "indexKeepsDuplicateKeys");
			report.Check(CountOfferId(table, UniqueOfferId) == 1, "indexSingleKeyCount");

			{
				auto rangeLease = table.OfferIdIndex->Range(SharedOfferId, UniqueOfferId);
				report.Check(CountIterators(rangeLease.begin(), rangeLease.end()) == 3, "indexRangeCoversBothBounds");
			}

			// 改索引键：节点必须在索引中重排，主键查询读到新值
			PrimaryAccount movedRecord{};
			FillPrimaryAccount(movedRecord, "C", MovedOfferId, "C");
			report.Check(table.Update(movedRecord), "indexKeyUpdateAccepted");
			AccountIdType movedAccountKey{};
			FillFixedString(movedAccountKey, "C");
			const std::optional<PrimaryAccount> updatedRecord = table.PrimaryKey->Select(movedAccountKey);
			report.Check(updatedRecord.has_value() && updatedRecord->OfferId == MovedOfferId, "indexKeyUpdateApplied");
			report.Check(CountOfferId(table, SharedOfferId) == 1, "indexKeyMovedOutOfOldKey");
			report.Check(CountOfferId(table, MovedOfferId) == 1, "indexKeyMovedIntoNewKey");

			// 改非索引字段：节点不动，计数与键都保持
			PrimaryAccount renamedRecord{};
			FillPrimaryAccount(renamedRecord, "A", SharedOfferId, "renamed");
			report.Check(table.Update(renamedRecord), "indexUnrelatedFieldUpdateAccepted");
			report.Check(CountOfferId(table, SharedOfferId) == 1, "indexUnchangedAfterUnrelatedFieldUpdate");

			report.Check(table.EraseByOfferIdIndex(UniqueOfferId) == 1, "eraseByIndexCount");
			report.Check(CountOfferId(table, UniqueOfferId) == 0, "eraseByIndexRemovedNode");
			AccountIdType erasedAccountKey{};
			FillFixedString(erasedAccountKey, "B");
			report.Check(!table.PrimaryKey->Select(erasedAccountKey).has_value(), "eraseByIndexRemovedRow");
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
			report.Check(eraseByIndexEventFound, "eraseByIndexEventCarriesIndexId");

			table.TruncateTable();
			report.Check(CountOfferId(table, SharedOfferId) == 0, "truncateClearsIndex");
			AccountIdType keptAccountKey{};
			FillFixedString(keptAccountKey, "A");
			report.Check(!table.PrimaryKey->Select(keptAccountKey).has_value(), "truncateClearsIndexedRows");
			return report.FailureCount();
		}

		// 唯一键只在 StableRow 生成：B 形态不维护任何辅助结构
		int CheckOrderUniqueKey()
		{
			CheckReporter report("StableRowUniqueKey");
			OrderTable table;

			const Order firstOrder = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 100);
			report.Check(InsertRecord(table, firstOrder), "uniqueKeyInsertAccepted");

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
			report.Check(selectOrder(100).has_value(), "uniqueKeySelectFound");

			// 主键不同，六个唯一键字段全同
			const Order duplicateOrder = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 2, 1, 100);
			report.Check(!InsertRecord(table, duplicateOrder), "duplicateUniqueKeyRejected");

			const Order changedKey = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 200);
			report.Check(!table.Update(changedKey), "uniqueKeyUpdateRejected");
			report.Check(selectOrder(100).has_value(), "uniqueKeyKeptAfterRejectedUpdate");

			Order changedPrice = firstOrder;
			changedPrice.Price = 3800.5;
			report.Check(table.Update(changedPrice), "nonKeyUpdateAccepted");
			const std::optional<Order> updatedOrder = selectOrder(100);
			report.Check(updatedOrder.has_value() && updatedOrder->Price == 3800.5, "uniqueKeySelectSeesUpdate");

			const Order eraseKey = MakeOrder("20261027", "ACC1", "CFFEX", "IF2612", 1, 1, 100);
			report.Check(table.Erase(eraseKey), "uniqueKeyEraseAccepted");
			report.Check(!selectOrder(100).has_value(), "uniqueKeyEraseRemovedEntry");

			report.Check(InsertRecord(table, duplicateOrder), "uniqueKeyReusableAfterErase");
			return report.FailureCount();
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

		PrimaryAccount MakePrimaryAccount(const char* primaryAccountId, std::int32_t offerId)
		{
			PrimaryAccount record{};
			FillPrimaryAccount(record, primaryAccountId, offerId, primaryAccountId);
			return record;
		}

		std::optional<Order> SelectOrderByClientOrderId(OrderTable& table, const Order& key)
		{
			return table.ClientOrderIdUniqueKey->Select(key.TradingDay, key.AccountId, key.ExchangeId,
				key.InstrumentId, key.SessionId, key.ClientOrderId);
		}

		// 复合字符串主键：Product 按 (ExchangeId, ProductId) 定行，Update 只能改非键字段
		int CheckCompositeKeyTable()
		{
			CheckReporter report("LockBasedCompositeKey");
			ProductTable table;

			report.Check(InsertRecord(table, MakeProduct("SHFE", "cu2601", "Copper2601")), "insertAccepted");
			report.Check(SelectProduct(table, "SHFE", "cu2601").has_value(), "selectFound");
			report.Check(!SelectProduct(table, "CFFEX", "cu2601").has_value(), "exchangeDiscriminatesKey");

			report.Check(!InsertRecord(table, MakeProduct("SHFE", "cu2601", "Duplicate")), "duplicateRejected");
			report.Check(InsertRecord(table, MakeProduct("SHFE", "ag2601", "Silver2601")), "siblingKeyAccepted");

			report.Check(table.Update(MakeProduct("SHFE", "cu2601", "Copper2601Renamed")), "updateAccepted");
			const std::optional<Product> renamedRow = SelectProduct(table, "SHFE", "cu2601");
			report.Check(renamedRow.has_value() && std::strcmp(renamedRow->ProductName, "Copper2601Renamed") == 0,
				"updateApplied");

			const Product eraseKey = MakeProduct("SHFE", "cu2601", "");
			report.Check(table.Erase(eraseKey), "eraseAccepted");
			report.Check(!SelectProduct(table, "SHFE", "cu2601").has_value(), "erasedRowGone");
			report.Check(SelectProduct(table, "SHFE", "ag2601").has_value(), "siblingKeyKept");
			report.Check(!table.Erase(eraseKey), "eraseMissingRejected");
			return report.FailureCount();
		}

		// 多列二级索引：重复键全部保留，按索引批量删要连带摘主键
		int CheckPositionAccountIndex()
		{
			CheckReporter report("LockBasedPositionIndex");
			PositionTable table;

			const bool allInserted =
				InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long))
				&& InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Long))
				&& InsertRecord(table, MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Short))
				&& InsertRecord(table, MakePosition("20261030", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long));
			report.Check(allInserted, "insertAllAccepted");
			report.Check(SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long)).has_value(),
				"longDirectionSelectFound");
			report.Check(SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Short)).has_value(),
				"shortDirectionSelectFound");

			report.Check(CountAccountIndex(table, "20261029", "ACC1") == 3, "accountIndexKeepsEveryRow");
			report.Check(CountTradingDayIndex(table, "20261029") == 3, "tradingDayIndexCountsSameDayRows");
			report.Check(CountTradingDayIndex(table, "20261030") == 1, "tradingDayIndexSeparatesDays");

			report.Check(EraseByAccountIndex(table, "20261029", "ACC1") == 3, "eraseByAccountIndexCount");
			report.Check(!SelectPosition(table,
				MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Long)).has_value(),
				"erasedByIndexRowGone");
			report.Check(CountAccountIndex(table, "20261029", "ACC1") == 0, "indexEmptiedByErase");
			report.Check(SelectPosition(table,
				MakePosition("20261030", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long)).has_value(),
				"otherDayRowKept");
			report.Check(EraseByAccountIndex(table, "20261029", "ACC1") == 0, "eraseByIndexIdempotent");
			return report.FailureCount();
		}

		// 索引列不在主键里：Update 改它要把记录在新旧键之间挪动，挪不动二叉搜索就会漏查
		int CheckPrimaryAccountIndexReorder()
		{
			CheckReporter report("LockBasedIndexReorder");
			PrimaryAccountTable table;

			const bool allInserted = InsertRecord(table, MakePrimaryAccount("ACC1", 10))
				&& InsertRecord(table, MakePrimaryAccount("ACC2", 20))
				&& InsertRecord(table, MakePrimaryAccount("ACC3", 30));
			report.Check(allInserted, "insertAllAccepted");
			report.Check(CountOfferId(table, 10) == 1, "offerIdIndexCountBefore");
			report.Check(CountOfferId(table, 20) == 1, "offerIdIndexMiddleKeyCount");
			report.Check(CountOfferId(table, 30) == 1, "offerIdIndexLastKeyCount");

			report.Check(table.Update(MakePrimaryAccount("ACC1", 90)), "indexKeyUpdateAccepted");
			report.Check(CountOfferId(table, 10) == 0, "indexKeyMovedOutOfOldKey");
			report.Check(CountOfferId(table, 90) == 1, "indexKeyMovedIntoNewKey");
			report.Check(CountOfferId(table, 20) == 1, "indexKeyNeighbourKept");

			PrimaryAccount eraseKey{};
			FillFixedString(eraseKey.PrimaryAccountId, "ACC1");
			report.Check(table.Erase(eraseKey), "eraseAccepted");
			report.Check(CountOfferId(table, 90) == 0, "indexKeyReleasedByErase");
			return report.FailureCount();
		}

		// TruncateTable 必须把主键、索引、唯一键三份登记一起清掉，否则同键再插会撞旧登记
		int CheckTruncateClearsRegistrations()
		{
			CheckReporter report("LockBasedTruncate");
			PositionTable positionTable;
			const Position longPosition = MakePosition("20261029", "ACC1", "SHFE", "cu2601", PosiDirectionType::Long);
			const Position shortPosition = MakePosition("20261029", "ACC1", "SHFE", "ag2601", PosiDirectionType::Short);
			report.Check(InsertRecord(positionTable, longPosition) && InsertRecord(positionTable, shortPosition),
				"positionInsertAccepted");
			report.Check(CountAccountIndex(positionTable, "20261029", "ACC1") == 2, "positionIndexPopulated");

			OrderTable orderTable;
			const Order firstOrder = MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 1, 1, 7001);
			report.Check(InsertRecord(orderTable, firstOrder), "orderInsertAccepted");
			report.Check(SelectOrderByClientOrderId(orderTable, firstOrder).has_value(), "orderUniqueKeyPopulated");

			positionTable.TruncateTable();
			orderTable.TruncateTable();
			report.Check(CountPrimaryKeyRows(positionTable) == 0, "truncateClearsPositionRows");
			report.Check(CountAccountIndex(positionTable, "20261029", "ACC1") == 0, "truncateClearsPositionIndex");
			report.Check(CountPrimaryKeyRows(orderTable) == 0, "truncateClearsOrderRows");
			report.Check(!SelectOrderByClientOrderId(orderTable, firstOrder).has_value(), "truncateClearsOrderUniqueKey");

			report.Check(InsertRecord(positionTable, longPosition), "positionReinsertAccepted");
			report.Check(CountAccountIndex(positionTable, "20261029", "ACC1") == 1, "positionIndexUsableAfterTruncate");
			report.Check(InsertRecord(orderTable, firstOrder), "orderReinsertAccepted");
			report.Check(SelectOrderByClientOrderId(orderTable, firstOrder).has_value(),
				"orderUniqueKeyUsableAfterTruncate");
			return report.FailureCount();
		}

		int CheckDumpWritesRows()
		{
			CheckReporter report("LockBasedDump");
			OrderTable table;
			const bool allInserted = InsertRecord(table, MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 1, 1, 7001))
				&& InsertRecord(table, MakeOrder("20261029", "ACC1", "SHFE", "cu2601", 2, 1, 7002));
			report.Check(allInserted, "insertAccepted");

			table.Dump(".");
			FILE* dumpFile = std::fopen(".//Orders.csv", "r");
			report.Check(dumpFile != nullptr, "dumpFileCreated");

			int lineCount = 0;
			bool headerMatched = false;
			if (dumpFile != nullptr)
			{
				char line[4096];
				while (std::fgets(line, sizeof(line), dumpFile) != nullptr)
				{
					headerMatched = headerMatched
						|| (lineCount == 0 && std::strncmp(line, "TradingDay,AccountId", 20) == 0);
					++lineCount;
				}
				std::fclose(dumpFile);
			}
			report.Check(headerMatched, "dumpHeaderMatched");
			report.Check(lineCount == 3, "dumpRowCount");
			return report.FailureCount();
		}
	}
}

int main()
{
	int failureCount = 0;
	failureCount += RunScenario<Mdb::Rcu::PointerSwap::TradingDayTable>("PointerSwap");
	failureCount += RunScenario<Mdb::Rcu::StableRow::TradingDayTable>("StableRow");
	failureCount += StableRow::CheckPrimaryAccountIndex();
	failureCount += StableRow::CheckOrderUniqueKey();
	failureCount += CheckAggregate<Mdb::Rcu::PointerSwap::Mdb>("PointerSwap");
	failureCount += CheckAggregate<Mdb::Rcu::StableRow::Mdb>("StableRow");
	failureCount += LockBased::CheckCompositeKeyTable();
	failureCount += LockBased::CheckPositionAccountIndex();
	failureCount += LockBased::CheckPrimaryAccountIndexReorder();
	failureCount += LockBased::CheckTruncateClearsRegistrations();
	failureCount += LockBased::CheckDumpWritesRows();
	std::printf("failures=%d\n", failureCount);
	return failureCount == 0 ? 0 : 1;
}
