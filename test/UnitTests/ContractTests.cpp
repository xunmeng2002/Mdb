// Mdb 聚合层的协作契约：订阅扇出、快照时序、写入校验、副本语义。
// 全部只用内存表，不接物理库；表级用例在 TableTests.cpp。
#include "Common/MdbStructs.h"
#include "LockBased/Mdb.h"

#include "doctest/doctest.h"

#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <DbAdapters/DbInterface/RowChangeEvent.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace
{
	using DbAdapters::MdbReplicaSubscriber;
	using DbAdapters::MdbSubscriber;
	using DbAdapters::RowChangeEvent;
	using DbAdapters::RowChangeEventPtr;
	using DbAdapters::RowChangeType;
	using Mdb::Exchange;
	using Mdb::Order;
	using Mdb::OrderIdType;

	const unsigned int ContractTableIds[] = { Exchange::TableId, Order::TableId };
	const Mdb::TableList ContractTableList{ "contract", ContractTableIds, 2 };

	// 观测「写入被挡住」只需确认在这段时间内没有完成，不是吞吐判据
	constexpr std::chrono::milliseconds BlockedWriteObservationDuration{ 100 };

	Exchange* AllocateExchange(const char* exchangeId)
	{
		Exchange* exchange = Exchange::Allocate();
		std::strcpy(exchange->ExchangeId, exchangeId);
		return exchange;
	}

	class EventRecordingSubscriber : public MdbSubscriber
	{
	public:
		void OnRowChange(const RowChangeEventPtr& event) override
		{
			std::lock_guard<std::mutex> guard(mutex_);
			receivedEvents_.push_back(event);
		}

		std::vector<RowChangeEventPtr> TakeReceivedEvents()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return std::move(receivedEvents_);
		}

	private:
		std::mutex mutex_;
		std::vector<RowChangeEventPtr> receivedEvents_;
	};

	// 在快照批量回调里阻塞：把「快照期间写入必须排队」变成可观察的时序
	class SnapshotGateReplica : public MdbReplicaSubscriber
	{
	public:
		void OnRowChange(const RowChangeEventPtr& /*event*/) override
		{
			std::lock_guard<std::mutex> guard(mutex_);
			if (snapshotCompleted_)
			{
				rowChangeCountAfterSnapshotComplete_ += 1;
			}
			else
			{
				rowChangeCountBeforeSnapshotComplete_ += 1;
			}
		}

		void OnRowBatchChange(const std::vector<RowChangeEventPtr>& events) override
		{
			{
				std::lock_guard<std::mutex> guard(mutex_);
				snapshotRowCount_ = static_cast<int>(events.size());
				batchEntered_ = true;
			}
			conditionVariable_.notify_all();
			std::unique_lock<std::mutex> lock(mutex_);
			conditionVariable_.wait(lock, [this] { return batchReleased_; });
		}

		void OnSnapshotBegin(unsigned int /*tableId*/) override
		{
			std::lock_guard<std::mutex> guard(mutex_);
			snapshotBeginCount_ += 1;
		}

		void OnSnapshotComplete(unsigned int /*tableId*/) override
		{
			std::lock_guard<std::mutex> guard(mutex_);
			snapshotCompleted_ = true;
		}

		void WaitUntilBatchEntered()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			conditionVariable_.wait(lock, [this] { return batchEntered_; });
		}

		void ReleaseBatch()
		{
			{
				std::lock_guard<std::mutex> guard(mutex_);
				batchReleased_ = true;
			}
			conditionVariable_.notify_all();
		}

		bool IsSnapshotCompleted()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return snapshotCompleted_;
		}

		int GetSnapshotRowCount()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return snapshotRowCount_;
		}

		int GetSnapshotBeginCount()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return snapshotBeginCount_;
		}

		int GetRowChangeCountBeforeSnapshotComplete()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return rowChangeCountBeforeSnapshotComplete_;
		}

		int GetRowChangeCountAfterSnapshotComplete()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return rowChangeCountAfterSnapshotComplete_;
		}

	private:
		std::mutex mutex_;
		std::condition_variable conditionVariable_;
		bool batchEntered_ = false;
		bool batchReleased_ = false;
		bool snapshotCompleted_ = false;
		int snapshotRowCount_ = 0;
		int snapshotBeginCount_ = 0;
		int rowChangeCountBeforeSnapshotComplete_ = 0;
		int rowChangeCountAfterSnapshotComplete_ = 0;
	};

	class RowChangeGateReplica : public SnapshotGateReplica
	{
	public:
		void OnRowChange(const RowChangeEventPtr& event) override
		{
			SnapshotGateReplica::OnRowChange(event);
			{
				std::lock_guard<std::mutex> guard(rowChangeMutex_);
				rowChangeEntered_ = true;
			}
			rowChangeConditionVariable_.notify_all();
			std::unique_lock<std::mutex> lock(rowChangeMutex_);
			rowChangeConditionVariable_.wait(lock, [this] { return rowChangeReleased_; });
		}

		void WaitUntilRowChangeEntered()
		{
			std::unique_lock<std::mutex> lock(rowChangeMutex_);
			rowChangeConditionVariable_.wait(lock, [this] { return rowChangeEntered_; });
		}

		void ReleaseRowChange()
		{
			{
				std::lock_guard<std::mutex> guard(rowChangeMutex_);
				rowChangeReleased_ = true;
			}
			rowChangeConditionVariable_.notify_all();
		}

	private:
		std::mutex rowChangeMutex_;
		std::condition_variable rowChangeConditionVariable_;
		bool rowChangeEntered_ = false;
		bool rowChangeReleased_ = false;
	};
}

TEST_CASE("订阅扇出：一份不可变事件共享给全部订阅者，最后引用释放时记录回池")
{
	Mdb::Mdb mdb(ContractTableList);
	EventRecordingSubscriber firstSubscriber;
	EventRecordingSubscriber secondSubscriber;
	mdb.Subscribe(&firstSubscriber);
	mdb.Subscribe(&secondSubscriber);

	Exchange* liveRecord = AllocateExchange("SHFE");
	mdb.Exchanges->Insert(liveRecord);

	std::vector<RowChangeEventPtr> firstEvents = firstSubscriber.TakeReceivedEvents();
	std::vector<RowChangeEventPtr> secondEvents = secondSubscriber.TakeReceivedEvents();
	REQUIRE(firstEvents.size() == 1);
	REQUIRE(secondEvents.size() == 1);

	const long sharedReferenceCount = firstEvents.front().use_count();
	std::weak_ptr<const RowChangeEvent> eventWatcher;
	{
		const RowChangeEventPtr observedEvent = firstEvents.front();
		CHECK(observedEvent.get() == secondEvents.front().get());
		CHECK(sharedReferenceCount == 2);
		CHECK(observedEvent->Record.Get() != liveRecord);
		CHECK(observedEvent->Type == RowChangeType::Insert);
		CHECK(observedEvent->TableId == Exchange::TableId);
		CHECK(observedEvent->Sequence == 1);

		eventWatcher = observedEvent;
		firstEvents.clear();
		secondEvents.clear();
		CHECK(!eventWatcher.expired());
	}
	CHECK(eventWatcher.expired());

	mdb.UnSubscribe();
	mdb.Exchanges->TruncateTable();
}

TEST_CASE("快照持读租约：期间写入被挡，变更事件排在批量之后")
{
	Mdb::Mdb mdb(ContractTableList);
	mdb.Exchanges->Insert(AllocateExchange("SHFE"));

	SnapshotGateReplica replica;
	mdb.Exchanges->SubscribeReplica(&replica);

	std::thread snapshotThread([&mdb]() { mdb.Exchanges->InitDb(); });
	replica.WaitUntilBatchEntered();

	std::atomic<bool> writerCompleted{ false };
	std::thread writerThread([&mdb, &writerCompleted]()
	{
		mdb.Exchanges->Insert(AllocateExchange("INE"));
		writerCompleted = true;
	});

	std::this_thread::sleep_for(BlockedWriteObservationDuration);

	CHECK(!writerCompleted.load());
	CHECK(!replica.IsSnapshotCompleted());

	replica.ReleaseBatch();
	snapshotThread.join();
	writerThread.join();

	CHECK(replica.GetSnapshotRowCount() == 1);
	CHECK(replica.GetRowChangeCountBeforeSnapshotComplete() == 0);
	CHECK(replica.GetRowChangeCountAfterSnapshotComplete() == 1);

	mdb.UnSubscribe();
	mdb.Exchanges->TruncateTable();
}

TEST_CASE("发布屏障：写事务尚未发布时快照等待，不得把该行重复纳入批量")
{
	Mdb::Mdb mdb(ContractTableList);
	mdb.Exchanges->Insert(AllocateExchange("SHFE"));

	RowChangeGateReplica replica;
	mdb.Exchanges->SubscribeReplica(&replica);

	std::thread writerThread([&mdb]() { mdb.Exchanges->Insert(AllocateExchange("INE")); });
	replica.WaitUntilRowChangeEntered();

	std::thread snapshotThread([&mdb]() { mdb.Exchanges->InitDb(); });
	std::this_thread::sleep_for(BlockedWriteObservationDuration);
	CHECK(replica.GetSnapshotBeginCount() == 0);

	replica.ReleaseRowChange();
	replica.WaitUntilBatchEntered();
	CHECK(replica.GetSnapshotRowCount() == 2);
	replica.ReleaseBatch();

	writerThread.join();
	snapshotThread.join();

	CHECK(replica.GetRowChangeCountBeforeSnapshotComplete() == 1);
	CHECK(replica.GetRowChangeCountAfterSnapshotComplete() == 0);

	mdb.UnSubscribe();
	mdb.Exchanges->TruncateTable();
}

TEST_CASE("写入校验：缺行与唯一键改键都被拒，且不发事件")
{
	Mdb::Mdb mdb(ContractTableList);
	EventRecordingSubscriber recorder;
	mdb.Orders->Subscribe(&recorder);

	Mdb::DateType tradingDay = "20261022";
	Mdb::AccountIdType accountId = "A001";
	Mdb::ExchangeIdType exchangeId = "SHFE";
	Mdb::InstrumentIdType instrumentId = "CU2507";
	auto fillOrderKey = [&](Order& order, OrderIdType orderId)
	{
		std::strcpy(order.TradingDay, tradingDay);
		std::strcpy(order.AccountId, accountId);
		std::strcpy(order.ExchangeId, exchangeId);
		std::strcpy(order.InstrumentId, instrumentId);
		order.OrderId = orderId;
	};
	auto selectLiveOrder = [&]() -> std::optional<Order>
	{
		return mdb.Orders->PrimaryKey->Select(tradingDay, accountId, exchangeId, instrumentId, 1001);
	};

	Order* liveOrder = Order::Allocate();
	fillOrderKey(*liveOrder, 1001);
	liveOrder->SessionId = 1;
	liveOrder->ClientOrderId = 5001;
	liveOrder->Volume = 1;
	mdb.Orders->Insert(liveOrder);
	recorder.TakeReceivedEvents();

	Order missingOrder{};
	fillOrderKey(missingOrder, 9999);
	CHECK(!mdb.Orders->Update(missingOrder));
	CHECK(!mdb.Orders->Erase(missingOrder));
	CHECK(recorder.TakeReceivedEvents().empty());

	std::optional<Order> changedUniqueKey = selectLiveOrder();
	REQUIRE(changedUniqueKey.has_value());
	changedUniqueKey->ClientOrderId = 6001;
	CHECK(!mdb.Orders->Update(*changedUniqueKey));
	CHECK(recorder.TakeReceivedEvents().empty());
	const std::optional<Order> afterRejection = selectLiveOrder();
	CHECK(afterRejection.has_value());
	CHECK(afterRejection->ClientOrderId == 5001);

	// 正对照：非键字段照常可改，证明前面的拒绝来自校验而非接口失效
	std::optional<Order> volumeChange = selectLiveOrder();
	REQUIRE(volumeChange.has_value());
	volumeChange->Volume = 7;
	CHECK(mdb.Orders->Update(*volumeChange));
	const std::optional<Order> afterUpdate = selectLiveOrder();
	CHECK(afterUpdate.has_value());
	CHECK(afterUpdate->Volume == 7);

	const std::vector<RowChangeEventPtr> updateEvents = recorder.TakeReceivedEvents();
	REQUIRE(updateEvents.size() == 1);
	CHECK(updateEvents.front()->Type == RowChangeType::Update);
	CHECK(updateEvents.front()->TableId == Order::TableId);

	mdb.UnSubscribe();
	mdb.Orders->TruncateTable();
}

TEST_CASE("副本语义：持副本时写者可进，Erase 传副本删活行，ForEach 可提前中止")
{
	Mdb::Mdb mdb(ContractTableList);
	Exchange* firstExchange = AllocateExchange("SHFE");
	std::strcpy(firstExchange->ExchangeName, "上海期货交易所");
	mdb.Exchanges->Insert(firstExchange);

	Mdb::ExchangeIdType exchangeId = "SHFE";
	const std::optional<Exchange> snapshot = mdb.Exchanges->PrimaryKey->Select(exchangeId);
	REQUIRE(snapshot.has_value());
	CHECK(&*snapshot != firstExchange);

	// 副本与活记录脱钩：改活记录不影响早已取走的副本
	std::strcpy(firstExchange->ExchangeName, "Renamed");
	CHECK(std::strcmp(snapshot->ExchangeName, "Renamed") != 0);

	// 副本在手不阻塞写者：Select 返回前已释放共享锁
	std::atomic<bool> writerCompleted{ false };
	std::thread writer([&mdb, &writerCompleted]()
	{
		mdb.Exchanges->Insert(AllocateExchange("INE"));
		writerCompleted = true;
	});
	const auto writerDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
	while (!writerCompleted.load() && std::chrono::steady_clock::now() < writerDeadline)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	CHECK(writerCompleted.load());
	writer.join();

	// Erase 按主键定位：传副本即可删掉表内活记录
	CHECK(mdb.Exchanges->Erase(*snapshot));
	CHECK(!mdb.Exchanges->PrimaryKey->Select(exchangeId).has_value());

	int visitedRowCount = 0;
	mdb.Exchanges->ForEach([&visitedRowCount](const Exchange*)
	{
		visitedRowCount += 1;
		return false;
	});
	CHECK(visitedRowCount == 1);

	mdb.Exchanges->TruncateTable();
}
