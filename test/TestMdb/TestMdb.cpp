#include "LockBased/Mdb.h"
#include "LockBased/InitMdbFromDb.h"
#include "Common/MdbTableRegistry.h"
#include "FullTableList.h"
#include <DbAdapters/DbInterface/TypedTable.h>
#include <DbAdapters/DbInterface/SchemaRegistry.h>
#include <DbAdapters/AsyncDbWriter/AsyncDbWriter.h>
#include <DbAdapters/SqliteWrapper/SqliteWrapper.h>
#include <DbAdapters/DuckdbWrapper/DuckdbWrapper.h>
#include <DbAdapters/MysqlWrapper/MysqlWrapper.h>
#include <DbAdapters/MariadbWrapper/MariadbWrapper.h>
#include <Spark/Core/Core.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std;
using namespace Spark::Core;
using namespace DbAdapters;

namespace testMdb
{
    using namespace Mdb;
    using namespace ::Mdb::Full;


    const char* SqliteDbName = "./Test.sqlitedb";
    const char* DuckdbDbName = "./Test.duckdb";
    const char* MysqlHost = "mysqlx://sams:sams@localhost:33060/mdb";
    const char* MariadbHost = "tcp://localhost:3306/mdb";
    const char* MariadbUser = "sams";
    const char* MariadbPassword = "sams";

    TradingDay* PrepareTradingDay()
    {
        auto currDate = TimeUtility::GetLocalDate();
        TradingDay* tradingDay = TradingDay::Allocate();
        memset(tradingDay, 0, sizeof(TradingDay));
        tradingDay->Pk = 1;
        TimeUtility::GetPreTradingDay(currDate.c_str(), tradingDay->PreTradingDay);
        TimeUtility::GetNextTradingDay(tradingDay->PreTradingDay, tradingDay->CurrTradingDay);
        return tradingDay;
    }
    list<Exchange*>* PrepareExchanges()
    {
        list<Exchange*>* exchanges = new list<Exchange*>();
        Exchange* exchange1 = Exchange::Allocate();
        Exchange* exchange2 = Exchange::Allocate();
        Exchange* exchange3 = Exchange::Allocate();
        Exchange* exchange4 = Exchange::Allocate();
        Exchange* exchange5 = Exchange::Allocate();
        Exchange* exchange6 = Exchange::Allocate();
        strcpy(exchange1->ExchangeId, "SHFE");
        strcpy(exchange1->ExchangeName, (const char*)(u8"上海期货交易所"));
        strcpy(exchange2->ExchangeId, "INE");
        strcpy(exchange2->ExchangeName, (const char*)(u8"上海国际能源交易中心"));
        strcpy(exchange3->ExchangeId, "CFFEX");
        strcpy(exchange3->ExchangeName, (const char*)(u8"中国金融期货交易所"));
        strcpy(exchange4->ExchangeId, "CZCE");
        strcpy(exchange4->ExchangeName, (const char*)(u8"郑州商品期货交易所"));
        strcpy(exchange5->ExchangeId, "DCE");
        strcpy(exchange5->ExchangeName, (const char*)(u8"大连商品期货交易所"));
        strcpy(exchange6->ExchangeId, "GFE");
        strcpy(exchange6->ExchangeName, (const char*)(u8"广期所"));

        exchanges->push_back(exchange1);
        exchanges->push_back(exchange2);
        exchanges->push_back(exchange3);
        exchanges->push_back(exchange4);
        exchanges->push_back(exchange5);
        exchanges->push_back(exchange6);
        return exchanges;
    }
    Account* PrepareAccount(const char* accountId, const char* accountName, const char* password)
    {
        Account* account = Account::Allocate();
        memset(account, 0, sizeof(Account));
        strcpy(account->AccountId, accountId);
        strcpy(account->AccountName, accountName);
        account->AccountType = AccountTypeType::Primary;
        account->AccountStatus = AccountStatusType::Normal;
        strcpy(account->Password, "123456");
        account->TradeGroupId = 10000;
        account->RiskGroupId = 10000;
        account->CommissionGroupId = 10000;
        return account;
    }

    static TradingDay* InitTradingDay(::Mdb::Mdb* mdb)
    {
        auto tradingDay = PrepareTradingDay();
        mdb->TradingDays->Insert(tradingDay);
        return tradingDay;
    }
    static void InitExchange(::Mdb::Mdb* mdb)
    {
        auto exchanges = PrepareExchanges();
        for (auto exchange : *exchanges)
        {
            mdb->Exchanges->Insert(exchange);
        }
        exchanges->clear();
        delete exchanges;
    }
    static void InitAccount(::Mdb::Mdb* mdb)
    {
        Account* account = PrepareAccount("Xunmeng01", "Xunmeng01", "123456");
        mdb->Accounts->Insert(account);
    }

    void Print(const TradingDay* tradingDay)
    {
        WriteLog(LogLevel::Info, "%s", tradingDay->GetDebugString());
    }
    void Print(const Exchange* exchange)
    {
        WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());
    }
    void Print(const Account* account)
    {
        WriteLog(LogLevel::Info, "%s", account->GetDebugString());
    }

    static void InitTradingDay(TypedTable<TradingDay>& table)
    {
        auto tradingDay = PrepareTradingDay();
        table.Insert(*tradingDay);
    }
    static void InitExchange(TypedTable<Exchange>& table)
    {
        auto exchanges = PrepareExchanges();
        for (auto exchange : *exchanges)
        {
            table.Insert(*exchange);
        }
        exchanges->clear();
        delete exchanges;
    }
    static void InitAccount(TypedTable<Account>& table)
    {
        Account* account = PrepareAccount("Xunmeng01", "Xunmeng01", "123456");
        table.Insert(*account);
    }


    static bool TestBatchInsertTakesOwnership()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);

        std::vector<Exchange*> records;
        records.push_back(Exchange::Allocate());
        records.push_back(Exchange::Allocate());
        records.push_back(Exchange::Allocate());
        strcpy(records[0]->ExchangeId, "SHFE");
        strcpy(records[1]->ExchangeId, "INE");
        strcpy(records[2]->ExchangeId, "CFFEX");

        Exchange* handedInRecord = records[0];
        mdb->Exchanges->BatchInsert(records);
        const bool callerContainerCleared = records.empty();

        int storedRowCount = 0;
        bool storedHandedInRecord = false;
        {
            auto storedRange = mdb->Exchanges->PrimaryKey->SelectAll();
            for (auto it = storedRange.begin(); it != storedRange.end(); ++it)
            {
                ++storedRowCount;
                if (*it == handedInRecord)
                {
                    storedHandedInRecord = true;
                }
            }
        }

        mdb->Exchanges->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info, "BatchInsert ownership: storedRowCount=%d storedHandedInRecord=%d callerContainerCleared=%d",
                 storedRowCount, storedHandedInRecord ? 1 : 0, callerContainerCleared ? 1 : 0);
        return storedRowCount == 3 && storedHandedInRecord && callerContainerCleared;
    }

    static bool TestSelectReturnsDetachedCopy()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);

        Exchange* firstExchange = Exchange::Allocate();
        strcpy(firstExchange->ExchangeId, "SHFE");
        strcpy(firstExchange->ExchangeName, "上海期货交易所");
        mdb->Exchanges->Insert(firstExchange);

        ExchangeIdType exchangeId = "SHFE";
        std::optional<Exchange> snapshot = mdb->Exchanges->PrimaryKey->Select(exchangeId);
        const bool selectReturnedCopy = snapshot.has_value() && &*snapshot != firstExchange;

        // 副本与表内活记录脱钩：改活记录不影响早已取走的副本
        strcpy(firstExchange->ExchangeName, "Renamed");
        const bool copyDetachedFromLiveRecord = strcmp(snapshot->ExchangeName, "Renamed") != 0;

        // 副本在手不阻塞写者：Select 返回前已释放共享锁
        std::atomic<bool> writerCompleted{ false };
        std::thread writer([mdb, &writerCompleted]()
        {
            Exchange* secondExchange = Exchange::Allocate();
            strcpy(secondExchange->ExchangeId, "INE");
            mdb->Exchanges->Insert(secondExchange);
            writerCompleted = true;
        });
        for (int waited = 0; waited < 3000 && !writerCompleted.load(); ++waited)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        const bool writerRanWhileCopyHeld = writerCompleted.load();
        writer.join();

        // Erase 按主键定位：传副本即可删掉表内活记录
        const bool erased = mdb->Exchanges->Erase(*snapshot);
        const bool erasedLiveRecord = erased && !mdb->Exchanges->PrimaryKey->Select(exchangeId).has_value();

        int rowCount = 0;
        mdb->Exchanges->ForEach([&rowCount](const Exchange*)
        {
            ++rowCount;
            return true;
        });

        int visitedBeforeStop = 0;
        mdb->Exchanges->ForEach([&visitedBeforeStop](const Exchange*)
        {
            ++visitedBeforeStop;
            return false;
        });

        mdb->Exchanges->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Select copy: selectReturnedCopy=%d copyDetachedFromLiveRecord=%d writerRanWhileCopyHeld=%d "
                 "erasedLiveRecord=%d rowCount=%d visitedBeforeStop=%d",
                 selectReturnedCopy ? 1 : 0, copyDetachedFromLiveRecord ? 1 : 0,
                 writerRanWhileCopyHeld ? 1 : 0, erasedLiveRecord ? 1 : 0, rowCount, visitedBeforeStop);
        return selectReturnedCopy && copyDetachedFromLiveRecord && writerRanWhileCopyHeld &&
               erasedLiveRecord && rowCount == 1 && visitedBeforeStop == 1;
    }

    // 收集行变更事件：验证扇出共享与记录生命周期
    class RowChangeRecordingSubscriber : public MdbSubscriber
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

    // 在快照批量回调里阻塞：把"快照期间写入必须排队"变成可观察的时序
    class BlockingSnapshotReplica : public MdbReplicaSubscriber
    {
    public:
        void OnRowChange(const RowChangeEventPtr& event) override
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (snapshotCompleted_)
            {
                ++rowChangeCountAfterSnapshotComplete_;
            }
            else
            {
                ++rowChangeCountBeforeSnapshotComplete_;
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
        void OnSnapshotBegin(unsigned int tableId) override
        {
            std::lock_guard<std::mutex> guard(mutex_);
            ++snapshotBeginCount_;
        }
        void OnSnapshotComplete(unsigned int tableId) override
        {
            std::lock_guard<std::mutex> guard(mutex_);
            snapshotCompleted_ = true;
        }

        void WaitForBatchEntered()
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

    class BlockingRowChangeReplica : public BlockingSnapshotReplica
    {
    public:
        void OnRowChange(const RowChangeEventPtr& event) override
        {
            BlockingSnapshotReplica::OnRowChange(event);
            {
                std::lock_guard<std::mutex> guard(rowChangeMutex_);
                rowChangeEntered_ = true;
            }
            rowChangeConditionVariable_.notify_all();
            std::unique_lock<std::mutex> lock(rowChangeMutex_);
            rowChangeConditionVariable_.wait(lock, [this] { return rowChangeReleased_; });
        }
        void WaitForRowChangeEntered()
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

    static Exchange* AllocateExchange(const char* exchangeId)
    {
        Exchange* exchange = Exchange::Allocate();
        strcpy(exchange->ExchangeId, exchangeId);
        return exchange;
    }

    // 扇出只共享一份不可变事件；最后一个订阅者释放时记录随之回池
    static bool TestRowChangeFanOutSharesOneEvent()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        RowChangeRecordingSubscriber firstSubscriber;
        RowChangeRecordingSubscriber secondSubscriber;
        mdb->Subscribe(&firstSubscriber);
        mdb->Subscribe(&secondSubscriber);

        Exchange* liveRecord = AllocateExchange("SHFE");
        mdb->Exchanges->Insert(liveRecord);

        std::vector<RowChangeEventPtr> firstEvents = firstSubscriber.TakeReceivedEvents();
        std::vector<RowChangeEventPtr> secondEvents = secondSubscriber.TakeReceivedEvents();

        const bool bothReceivedOnce = firstEvents.size() == 1 && secondEvents.size() == 1;
        if (!bothReceivedOnce)
        {
            WriteLog(LogLevel::Error, "Row change fan-out: firstCount=%d secondCount=%d",
                     static_cast<int>(firstEvents.size()), static_cast<int>(secondEvents.size()));
            return false;
        }

        const long sharedReferenceCount = firstEvents.front().use_count();
        RowChangeEventPtr observedEvent = firstEvents.front();
        const bool oneEventShared = observedEvent.get() == secondEvents.front().get();
        const bool recordIsCopyOfLiveRow = observedEvent->Record.Get() != liveRecord;
        const bool insertDescribed = observedEvent->Type == RowChangeType::Insert
            && observedEvent->TableId == Exchange::TableId
            && observedEvent->Sequence == 1;

        std::weak_ptr<const RowChangeEvent> eventWatcher = observedEvent;
        firstEvents.clear();
        secondEvents.clear();
        observedEvent.reset();
        const bool recordReleasedWithLastReference = eventWatcher.expired();

        mdb->UnSubscribe();
        mdb->Exchanges->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Row change fan-out: oneEventShared=%d sharedReferenceCount=%ld recordIsCopyOfLiveRow=%d "
                 "insertDescribed=%d recordReleasedWithLastReference=%d",
                 oneEventShared ? 1 : 0, sharedReferenceCount, recordIsCopyOfLiveRow ? 1 : 0,
                 insertDescribed ? 1 : 0, recordReleasedWithLastReference ? 1 : 0);
        return oneEventShared && sharedReferenceCount == 2 && recordIsCopyOfLiveRow
            && insertDescribed && recordReleasedWithLastReference;
    }

    // 写入失败的契约：主键不在表内一律 false 且不发事件；唯一键字段不得经 Update 改写
    static bool TestWriteRejectsInvalidRecords()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        RowChangeRecordingSubscriber recorder;
        mdb->Orders->Subscribe(&recorder);

        DateType tradingDay = "20261022";
        AccountIdType accountId = "A001";
        ExchangeIdType exchangeId = "SHFE";
        InstrumentIdType instrumentId = "CU2507";

        auto fillOrderKey = [&](Order& order, OrderIdType orderId)
        {
            strcpy(order.TradingDay, tradingDay);
            strcpy(order.AccountId, accountId);
            strcpy(order.ExchangeId, exchangeId);
            strcpy(order.InstrumentId, instrumentId);
            order.OrderId = orderId;
        };
        auto selectLiveOrder = [&]() -> std::optional<Order>
        {
            return mdb->Orders->PrimaryKey->Select(tradingDay, accountId, exchangeId, instrumentId, 1001);
        };

        Order* liveOrder = Order::Allocate();
        fillOrderKey(*liveOrder, 1001);
        liveOrder->SessionId = 1;
        liveOrder->ClientOrderId = 5001;
        liveOrder->Volume = 1;
        mdb->Orders->Insert(liveOrder);
        recorder.TakeReceivedEvents();                       // 丢掉插入事件，后面只看写入路径

        Order missingOrder{};
        fillOrderKey(missingOrder, 9999);
        const bool updateMissingRejected = !mdb->Orders->Update(missingOrder);
        const bool eraseMissingRejected = !mdb->Orders->Erase(missingOrder);
        const bool missingRecordEmittedNothing = recorder.TakeReceivedEvents().empty();

        std::optional<Order> changedUniqueKey = selectLiveOrder();
        bool uniqueKeyChangeRejected = false;
        if (changedUniqueKey.has_value())
        {
            changedUniqueKey->ClientOrderId = 6001;
            uniqueKeyChangeRejected = !mdb->Orders->Update(*changedUniqueKey);
        }
        const bool uniqueKeyChangeEmittedNothing = recorder.TakeReceivedEvents().empty();

        std::optional<Order> afterRejection = selectLiveOrder();
        const bool liveRecordUnchanged = afterRejection.has_value() && afterRejection->ClientOrderId == 5001;

        // 正对照：非键字段照常可改，证明前面的 false 来自校验而非接口失效
        std::optional<Order> volumeChange = selectLiveOrder();
        bool volumeUpdated = false;
        if (volumeChange.has_value())
        {
            volumeChange->Volume = 7;
            volumeUpdated = mdb->Orders->Update(*volumeChange);
        }
        std::optional<Order> afterUpdate = selectLiveOrder();
        const bool updatedValueStored = afterUpdate.has_value() && afterUpdate->Volume == 7;

        std::vector<RowChangeEventPtr> updateEvents = recorder.TakeReceivedEvents();
        const bool successEmittedOneUpdate = updateEvents.size() == 1
            && updateEvents.front()->Type == RowChangeType::Update
            && updateEvents.front()->TableId == Order::TableId;

        mdb->UnSubscribe();
        mdb->Orders->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Write rejection: updateMissingRejected=%d eraseMissingRejected=%d missingRecordEmittedNothing=%d "
                 "uniqueKeyChangeRejected=%d uniqueKeyChangeEmittedNothing=%d liveRecordUnchanged=%d "
                 "volumeUpdated=%d updatedValueStored=%d successEmittedOneUpdate=%d",
                 updateMissingRejected ? 1 : 0, eraseMissingRejected ? 1 : 0, missingRecordEmittedNothing ? 1 : 0,
                 uniqueKeyChangeRejected ? 1 : 0, uniqueKeyChangeEmittedNothing ? 1 : 0, liveRecordUnchanged ? 1 : 0,
                 volumeUpdated ? 1 : 0, updatedValueStored ? 1 : 0, successEmittedOneUpdate ? 1 : 0);
        return updateMissingRejected && eraseMissingRejected && missingRecordEmittedNothing
            && uniqueKeyChangeRejected && uniqueKeyChangeEmittedNothing && liveRecordUnchanged
            && volumeUpdated && updatedValueStored && successEmittedOneUpdate;
    }

    // 快照全程持有读租约：期间的写入被挡住，其事件必然排在批量之后
    static bool TestSnapshotBatchPrecedesLaterWrites()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        mdb->Exchanges->Insert(AllocateExchange("SHFE"));

        BlockingSnapshotReplica replica;
        mdb->Exchanges->SubscribeReplica(&replica);

        std::thread snapshotThread([mdb]() { mdb->Exchanges->InitDb(); });
        replica.WaitForBatchEntered();

        std::atomic<bool> writerCompleted{ false };
        std::thread writerThread([mdb, &writerCompleted]()
        {
            mdb->Exchanges->Insert(AllocateExchange("INE"));
            writerCompleted = true;
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        const bool writeBlockedWhileSnapshotHeld = !writerCompleted.load();
        const bool snapshotCompletedWhileBatchHeld = replica.IsSnapshotCompleted();

        replica.ReleaseBatch();
        snapshotThread.join();
        writerThread.join();

        const bool snapshotCarriedOnlyPreexistingRow = replica.GetSnapshotRowCount() == 1;
        const bool laterWriteReachedSubscribersAfterSnapshot =
            replica.GetRowChangeCountBeforeSnapshotComplete() == 0
            && replica.GetRowChangeCountAfterSnapshotComplete() == 1;

        mdb->UnSubscribe();
        mdb->Exchanges->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Snapshot ordering: writeBlockedWhileSnapshotHeld=%d snapshotCompletedWhileBatchHeld=%d "
                 "snapshotCarriedOnlyPreexistingRow=%d rowChangesBeforeComplete=%d rowChangesAfterComplete=%d",
                 writeBlockedWhileSnapshotHeld ? 1 : 0, snapshotCompletedWhileBatchHeld ? 1 : 0,
                 snapshotCarriedOnlyPreexistingRow ? 1 : 0,
                 replica.GetRowChangeCountBeforeSnapshotComplete(), replica.GetRowChangeCountAfterSnapshotComplete());
        return writeBlockedWhileSnapshotHeld && !snapshotCompletedWhileBatchHeld
            && snapshotCarriedOnlyPreexistingRow && laterWriteReachedSubscribersAfterSnapshot;
    }

    // 发布屏障：写事务已改表但尚未发布事件时，快照必须等待，不能先把该行纳入批量再把事件补到快照之后
    static bool TestSnapshotWaitsForPendingPublication()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        mdb->Exchanges->Insert(AllocateExchange("SHFE"));

        BlockingRowChangeReplica replica;
        mdb->Exchanges->SubscribeReplica(&replica);

        std::thread writerThread([mdb]() { mdb->Exchanges->Insert(AllocateExchange("INE")); });
        replica.WaitForRowChangeEntered();

        std::thread snapshotThread([mdb]() { mdb->Exchanges->InitDb(); });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        const bool snapshotWaitedForPublication = replica.GetSnapshotBeginCount() == 0;

        replica.ReleaseRowChange();
        replica.WaitForBatchEntered();
        const bool snapshotIncludesPublishedRow = replica.GetSnapshotRowCount() == 2;
        replica.ReleaseBatch();

        writerThread.join();
        snapshotThread.join();

        const bool eventDeliveredBeforeSnapshot = replica.GetRowChangeCountBeforeSnapshotComplete() == 1
            && replica.GetRowChangeCountAfterSnapshotComplete() == 0;

        mdb->UnSubscribe();
        mdb->Exchanges->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Snapshot waits for pending publication: snapshotWaitedForPublication=%d "
                 "snapshotIncludesPublishedRow=%d eventDeliveredBeforeSnapshot=%d",
                 snapshotWaitedForPublication ? 1 : 0, snapshotIncludesPublishedRow ? 1 : 0,
                 eventDeliveredBeforeSnapshot ? 1 : 0);
        return snapshotWaitedForPublication && snapshotIncludesPublishedRow && eventDeliveredBeforeSnapshot;
    }

    // 可控失败的 Db 装饰器：让写线程从自己的 HandleDbOperate 里走到"操作抛异常 -> DisConnect ->
    // 5 秒后重连"这条生产路径。若从测试线程外部调 DisConnect，写线程可能在句柄关闭之前把
    // connected_ 重新置真，此后所有语句都打在已关闭的句柄上，且再也不会重连。
    class FailureInjectingDb : public Db
    {
    public:
        explicit FailureInjectingDb(Db* inner) : inner_(inner) {}
        ~FailureInjectingDb() override { delete inner_; }

        bool Connect() override { return inner_->Connect(); }
        void DisConnect() override
        {
            DisconnectCount.fetch_add(1);
            inner_->DisConnect();
        }
        void Exec(const char* sql) override { inner_->Exec(sql); }
        void CreateTable(const TableSchema* schema) override { inner_->CreateTable(schema); }
        void DropTable(const char* tableName) override { inner_->DropTable(tableName); }
        void TruncateTable(const char* tableName) override { inner_->TruncateTable(tableName); }
        void CreateTables(const TableSchema* const* schemas, int count) override { inner_->CreateTables(schemas, count); }
        void DropTables(const TableSchema* const* schemas, int count) override { inner_->DropTables(schemas, count); }
        void TruncateTables(const TableSchema* const* schemas, int count) override { inner_->TruncateTables(schemas, count); }
        void Insert(const TableSchema* schema, const void* record) override
        {
            ThrowIfRequested();
            inner_->Insert(schema, record);
        }
        void BatchInsert(const TableSchema* schema, const void* const* records, int count) override
        {
            ThrowIfRequested();
            inner_->BatchInsert(schema, records, count);
        }
        void Update(const TableSchema* schema, const void* record) override { inner_->Update(schema, record); }
        void Delete(const TableSchema* schema, const void* record, const int* keyFieldIndices, int keyFieldCount) override
        {
            inner_->Delete(schema, record, keyFieldIndices, keyFieldCount);
        }
        void SelectAll(const TableSchema* schema, void* recordsList, const RecordFactory& factory) override
        {
            inner_->SelectAll(schema, recordsList, factory);
        }
        void SelectWithSql(const char* sql, const TableSchema* schema, void* recordsList, const RecordFactory& factory) override
        {
            inner_->SelectWithSql(sql, schema, recordsList, factory);
        }

        void ThrowOnNextWrite()
        {
            pendingFailure_.fetch_add(1);
        }
        bool WaitForDisconnect(int timeoutMilliseconds)
        {
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMilliseconds);
            while (std::chrono::steady_clock::now() < deadline)
            {
                if (DisconnectCount.load() >= 1)
                {
                    return true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return DisconnectCount.load() >= 1;
        }

        std::atomic<int> DisconnectCount{ 0 };

    private:
        void ThrowIfRequested()
        {
            if (pendingFailure_.load() > 0)
            {
                pendingFailure_.fetch_sub(1);
                throw std::runtime_error("FailureInjectingDb: write fails by request");
            }
        }

        Db* inner_;
        std::atomic<int> pendingFailure_{ 0 };
    };

    // 断连期间的增量就地丢弃；重连靠全量快照补齐，不重复插入
    static bool TestDisconnectDropsIncrementsAndReconnectResyncs()
    {
        const char* reconnectDbName = "./TestReconnect.sqlitedb";
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        MdbTableRegistry* schemaRegistry = new MdbTableRegistry(FullTableList);
        FailureInjectingDb* failingDb = new FailureInjectingDb(new SqliteWrapper(reconnectDbName));
        AsyncDbWriter* dbWriter = new AsyncDbWriter(failingDb, schemaRegistry);
        mdb->SubscribeReplica(dbWriter);
        dbWriter->Subscribe(mdb);
        dbWriter->Start();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        mdb->Exchanges->Insert(AllocateExchange("SHFE"));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        const int pendingWhileConnected = dbWriter->PendingOperateCount();

        failingDb->ThrowOnNextWrite();
        mdb->Exchanges->Insert(AllocateExchange("INE"));
        const bool writerDisconnectedItself = failingDb->WaitForDisconnect(5000);

        mdb->Exchanges->Insert(AllocateExchange("CFFEX"));
        const int pendingAfterDisconnectedInsert = dbWriter->PendingOperateCount();

        // 写线程休眠 5 秒后自行重连：重建表 -> 全表快照 -> 批量落库
        std::this_thread::sleep_for(std::chrono::seconds(8));
        dbWriter->Stop();
        dbWriter->Join();

        int storedRowCount = -1;
        bool storedAllExchangeIds = false;
        {
            SqliteWrapper verifyDb(reconnectDbName);
            if (verifyDb.Connect())
            {
                TypedTable<Exchange> exchangeTable(&verifyDb);
                std::vector<Exchange*> storedExchanges;
                exchangeTable.SelectAll(storedExchanges);
                storedRowCount = static_cast<int>(storedExchanges.size());
                bool storedShfe = false;
                bool storedIne = false;
                bool storedCffex = false;
                for (Exchange* storedExchange : storedExchanges)
                {
                    storedShfe = storedShfe || strcmp(storedExchange->ExchangeId, "SHFE") == 0;
                    storedIne = storedIne || strcmp(storedExchange->ExchangeId, "INE") == 0;
                    storedCffex = storedCffex || strcmp(storedExchange->ExchangeId, "CFFEX") == 0;
                    storedExchange->Deallocate();
                }
                storedAllExchangeIds = storedShfe && storedIne && storedCffex;
                verifyDb.DisConnect();
            }
        }

        mdb->UnSubscribe();
        delete dbWriter;
        delete schemaRegistry;
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Reconnect resync: pendingWhileConnected=%d writerDisconnectedItself=%d "
                 "pendingAfterDisconnectedInsert=%d storedRowCount=%d storedAllExchangeIds=%d",
                 pendingWhileConnected, writerDisconnectedItself ? 1 : 0,
                 pendingAfterDisconnectedInsert, storedRowCount, storedAllExchangeIds ? 1 : 0);
        return pendingWhileConnected == 0 && writerDisconnectedItself
            && pendingAfterDisconnectedInsert == 0 && storedRowCount == 3 && storedAllExchangeIds;
    }

    static void TestMdb(Db* db)
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        MdbTableRegistry* schemaRegistry = new MdbTableRegistry(FullTableList);
        AsyncDbWriter* dbWriter = new AsyncDbWriter(db, schemaRegistry);
        mdb->SubscribeReplica(dbWriter);
        dbWriter->Subscribe(mdb);
        dbWriter->Start();

        this_thread::sleep_for(chrono::seconds(1));

        TradingDay* tradingDayRecord = InitTradingDay(mdb);
        InitExchange(mdb);
        InitAccount(mdb);

        ExchangeIdType exchangeID("CFFEX");
        {
            std::optional<Exchange> exchange = mdb->Exchanges->PrimaryKey->Select(exchangeID);
            if (exchange.has_value())
            {
                Print(&*exchange);
            }
        }

        {
            std::optional<TradingDay> tradingDay = mdb->TradingDays->PrimaryKey->Select(1);
            if (tradingDay.has_value())
            {
                Print(&*tradingDay);
            }
        }
        mdb->TradingDays->Erase(*tradingDayRecord);
        mdb->TradingDays->TruncateTable();
        {
            auto exchangeRange = mdb->Exchanges->PrimaryKey->SelectAll();
            for (auto it = exchangeRange.begin(); it != exchangeRange.end(); ++it)
            {
                Print(*it);
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
        //mdb->DropTables();
        this_thread::sleep_for(chrono::seconds(1));
        dbWriter->Stop();
        dbWriter->Join();
        mdb->UnSubscribe();
        delete mdb;
        delete dbWriter;
        delete schemaRegistry;
        this_thread::sleep_for(chrono::seconds(1));
    }

    static void TestDb(Db* db)
    {
        if (!db->Connect())
        {
            WriteLog(LogLevel::Warning, "Connect Failed.");
            return;
        }

        static const TableSchema* allSchemas[] = {
            &TradingDay::GetSchema(), &Exchange::GetSchema(), &Product::GetSchema(),
            &Instrument::GetSchema(), &PrimaryAccount::GetSchema(), &Account::GetSchema(),
            &Capital::GetSchema(), &Position::GetSchema(), &PositionDetail::GetSchema(),
            &Order::GetSchema(), &Trade::GetSchema(),
        };
        db->CreateTables(allSchemas, 11);
        db->TruncateTables(allSchemas, 11);

        TypedTable<TradingDay>    tradingDayTable(db);
        TypedTable<Exchange>      exchangeTable(db);
        TypedTable<Account>       accountTable(db);

        InitTradingDay(tradingDayTable);
        InitExchange(exchangeTable);
        InitAccount(accountTable);

        {
            std::vector<Account*> accounts;
            accountTable.SelectAll(accounts);
            for (auto account : accounts)
            {
                Print(account);
            }
            if (!accounts.empty())
            {
                auto account = accounts.front();
                Account newAccount;
                memcpy(&newAccount, account, sizeof(Account));
                strcpy(newAccount.AccountName, "Jack01");
                accountTable.Update(newAccount);
            }
        }

        {
            std::vector<Exchange*> exchanges;
            exchangeTable.SelectAll(exchanges);
            for (auto exchange : exchanges)
            {
                Print(exchange);
            }
            if (!exchanges.empty())
            {
                auto exchange = exchanges.front();
                exchangeTable.Delete(*exchange);
            }
        }

        //db->DropAllTables(allSchemas, 11);
        db->DisConnect();
    }

    static void TestSqlite()
    {
        SqliteWrapper* sqlitedb = new SqliteWrapper(SqliteDbName);
        WriteLog(LogLevel::Info, "TestDB with Sqlite");
        TestMdb(sqlitedb);
    }
    static void TestDuckdb()
    {
        DuckdbWrapper* duckdb = new DuckdbWrapper(DuckdbDbName);
        WriteLog(LogLevel::Info, "TestDB with Duckdb");
        TestMdb(duckdb);
    }
    static void TestMysql()
    {
        MysqlWrapper* mysql = new MysqlWrapper(MysqlHost);
        WriteLog(LogLevel::Info, "TestDB with Mysql");
        TestMdb(mysql);
    }
    static void TestMariadb()
    {
        MariadbWrapper* mariadb = new MariadbWrapper(MariadbHost, MariadbUser, MariadbPassword);
        WriteLog(LogLevel::Info, "TestDB with Mariadb");
        TestMdb(mariadb);
    }
}
int main(int argc, char* argv[])
{
    Logger::GetInstance().Init(argv[0]);
    Logger::GetInstance().SetLogLevel(LogLevel::Info, LogLevel::Info);
    Logger::GetInstance().Start();

    testMdb::TestSqlite();
    testMdb::TestDuckdb();
    //testMdb::TestMysql();
    //testMdb::TestMariadb();

    const bool batchInsertOwnershipHolds = testMdb::TestBatchInsertTakesOwnership();
    const bool selectCopyHolds = testMdb::TestSelectReturnsDetachedCopy();
    const bool writeRejectionHolds = testMdb::TestWriteRejectsInvalidRecords();
    const bool rowChangeFanOutHolds = testMdb::TestRowChangeFanOutSharesOneEvent();
    const bool snapshotOrderingHolds = testMdb::TestSnapshotBatchPrecedesLaterWrites();
    const bool snapshotPublicationBarrierHolds = testMdb::TestSnapshotWaitsForPendingPublication();
    const bool reconnectResyncHolds = testMdb::TestDisconnectDropsIncrementsAndReconnectResyncs();

    Logger::GetInstance().Stop();
    Logger::GetInstance().Join();
    return (batchInsertOwnershipHolds && selectCopyHolds && writeRejectionHolds && rowChangeFanOutHolds
        && snapshotOrderingHolds && snapshotPublicationBarrierHolds && reconnectResyncHolds) ? 0 : 1;
}
