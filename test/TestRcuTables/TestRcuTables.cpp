#include "Model/MdbStructs.h"
#include "Rcu/PointerSwap/Mdb.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/Mdb.h"
#include "Rcu/StableRow/StableRowTables.h"
#include "TestSupport/TestSupport.h"

#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <atomic>
#include <chrono>
#include <cstddef>
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
	using Mdb::PrimaryAccount;
	using Mdb::TradingDay;
	using Mdb::Rcu::StableRow::OrderTable;
	using Mdb::Rcu::StableRow::PrimaryAccountTable;
	using Mdb::Rcu::StableRow::PrimaryAccountTableIndexOfferId;
	using TestSupport::CheckReporter;
	using TestSupport::FillTradingDay;
	using TestSupport::HasPairedDates;

	constexpr std::int32_t PairedPrimaryKey = 1;
	constexpr std::int32_t SharedOfferId = 10;
	constexpr std::int32_t UniqueOfferId = 20;
	constexpr std::int32_t MovedOfferId = 30;
	constexpr int ReaderThreadCount = 3;
	constexpr int ConcurrentUpdateCount = 20000;
	constexpr int BulkRowCount = 2000;

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

	template <std::size_t Size>
	void FillFixedString(char (&fixedString)[Size], const char* value)
	{
		std::snprintf(fixedString, Size, "%s", value);
	}

	void FillPrimaryAccount(PrimaryAccount& record, const char* primaryAccountId, std::int32_t offerId, const char* name)
	{
		FillFixedString(record.PrimaryAccountId, primaryAccountId);
		FillFixedString(record.PrimaryAccountName, name);
		record.OfferId = offerId;
	}

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

	int CountOfferId(PrimaryAccountTable& table, std::int32_t offerId)
	{
		auto rangeLease = table.OfferIdIndex->EqualRange(offerId);
		return CountIterators(rangeLease.begin(), rangeLease.end());
	}

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
			PrimaryAccount* row = PrimaryAccount::Allocate();
			FillPrimaryAccount(*row, accountIds[index], offerIds[index], accountIds[index]);
			allInserted = table.Insert(row) && allInserted;
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

	void FillOrder(Order& record, const char* tradingDay, const char* accountId, int orderId, long long sessionId, std::int32_t clientOrderId)
	{
		FillFixedString(record.TradingDay, tradingDay);
		FillFixedString(record.AccountId, accountId);
		FillFixedString(record.ExchangeId, "CFFEX");
		FillFixedString(record.InstrumentId, "IF2612");
		record.OrderId = orderId;
		record.SessionId = sessionId;
		record.ClientOrderId = clientOrderId;
	}

	// 唯一键只在 StableRow 生成：B 形态不维护任何辅助结构
	int CheckOrderUniqueKey()
	{
		CheckReporter report("StableRowUniqueKey");
		OrderTable table;

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

		Order firstOrder{};
		FillOrder(firstOrder, "20261027", "ACC1", 1, 1, 100);
		Order* firstRow = Order::Allocate();
		*firstRow = firstOrder;
		report.Check(table.Insert(firstRow), "uniqueKeyInsertAccepted");
		report.Check(selectOrder(100).has_value(), "uniqueKeySelectFound");

		// 主键不同，六个唯一键字段全同
		Order duplicateOrder{};
		FillOrder(duplicateOrder, "20261027", "ACC1", 2, 1, 100);
		Order* duplicateRow = Order::Allocate();
		*duplicateRow = duplicateOrder;
		report.Check(!table.Insert(duplicateRow), "duplicateUniqueKeyRejected");

		Order changedKey{};
		FillOrder(changedKey, "20261027", "ACC1", 1, 1, 200);
		report.Check(!table.Update(changedKey), "uniqueKeyUpdateRejected");
		report.Check(selectOrder(100).has_value(), "uniqueKeyKeptAfterRejectedUpdate");

		Order changedPrice = firstOrder;
		changedPrice.Price = 3800.5;
		report.Check(table.Update(changedPrice), "nonKeyUpdateAccepted");
		const std::optional<Order> updatedOrder = selectOrder(100);
		report.Check(updatedOrder.has_value() && updatedOrder->Price == 3800.5, "uniqueKeySelectSeesUpdate");

		Order eraseKey{};
		FillOrder(eraseKey, "20261027", "ACC1", 1, 1, 0);
		report.Check(table.Erase(eraseKey), "uniqueKeyEraseAccepted");
		report.Check(!selectOrder(100).has_value(), "uniqueKeyEraseRemovedEntry");

		Order* reusedRow = Order::Allocate();
		*reusedRow = duplicateOrder;
		report.Check(table.Insert(reusedRow), "uniqueKeyReusableAfterErase");
		return report.FailureCount();
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
}

int main()
{
	int failureCount = 0;
	failureCount += RunScenario<Mdb::Rcu::PointerSwap::TradingDayTable>("PointerSwap");
	failureCount += RunScenario<Mdb::Rcu::StableRow::TradingDayTable>("StableRow");
	failureCount += CheckPrimaryAccountIndex();
	failureCount += CheckOrderUniqueKey();
	failureCount += CheckAggregate<Mdb::Rcu::PointerSwap::Mdb>("PointerSwap");
	failureCount += CheckAggregate<Mdb::Rcu::StableRow::Mdb>("StableRow");
	std::printf("failures=%d\n", failureCount);
	return failureCount == 0 ? 0 : 1;
}
