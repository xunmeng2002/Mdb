#include "Mdb.h"
#include "InitMdbFromDb.h"
#include "MdbTableRegistry.h"
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
#include <iostream>
#include <thread>

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
        mdb->TradingDay->Insert(tradingDay);
        return tradingDay;
    }
    static void InitExchange(::Mdb::Mdb* mdb)
    {
        auto exchanges = PrepareExchanges();
        for (auto exchange : *exchanges)
        {
            mdb->Exchange->Insert(exchange);
        }
        exchanges->clear();
        delete exchanges;
    }
    static void InitAccount(::Mdb::Mdb* mdb)
    {
        Account* account = PrepareAccount("Xunmeng01", "Xunmeng01", "123456");
        mdb->Account->Insert(account);
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
        mdb->Exchange->BatchInsert(records);
        const bool callerContainerCleared = records.empty();

        int storedRowCount = 0;
        bool storedHandedInRecord = false;
        {
            auto storedRange = mdb->Exchange->PrimaryKey->SelectAll();
            for (auto it = storedRange.begin(); it != storedRange.end(); ++it)
            {
                ++storedRowCount;
                if (*it == handedInRecord)
                {
                    storedHandedInRecord = true;
                }
            }
        }

        mdb->Exchange->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info, "BatchInsert ownership: storedRowCount=%d storedHandedInRecord=%d callerContainerCleared=%d",
                 storedRowCount, storedHandedInRecord ? 1 : 0, callerContainerCleared ? 1 : 0);
        return storedRowCount == 3 && storedHandedInRecord && callerContainerCleared;
    }

    static bool TestReadLeaseBlocksWriter()
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);

        Exchange* firstExchange = Exchange::Allocate();
        strcpy(firstExchange->ExchangeId, "SHFE");
        mdb->Exchange->Insert(firstExchange);

        std::atomic<bool> writerStarted{ false };
        std::atomic<bool> writerCompleted{ false };
        std::thread writer([mdb, &writerStarted, &writerCompleted]()
        {
            Exchange* secondExchange = Exchange::Allocate();
            strcpy(secondExchange->ExchangeId, "INE");
            writerStarted = true;
            mdb->Exchange->Insert(secondExchange);
            writerCompleted = true;
        });

        bool leaseSawRecord = false;
        bool writerBlockedWhileLeaseHeld = false;
        ExchangeIdType exchangeId = "SHFE";
        {
            auto lease = mdb->Exchange->PrimaryKey->Select(exchangeId);
            leaseSawRecord = static_cast<bool>(lease);
            while (!writerStarted.load())
            {
                std::this_thread::yield();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            writerBlockedWhileLeaseHeld = !writerCompleted.load();
        }
        writer.join();
        const bool writerRanAfterLeaseReleased = writerCompleted.load();

        int rowCount = 0;
        mdb->Exchange->ForEach([&rowCount](const Exchange*)
        {
            ++rowCount;
            return true;
        });

        int visitedBeforeStop = 0;
        mdb->Exchange->ForEach([&visitedBeforeStop](const Exchange*)
        {
            ++visitedBeforeStop;
            return false;
        });

        mdb->Exchange->TruncateTable();
        delete mdb;

        WriteLog(LogLevel::Info,
                 "Read lease: sawRecord=%d writerBlockedWhileLeaseHeld=%d writerRanAfterLeaseReleased=%d "
                 "rowCount=%d visitedBeforeStop=%d",
                 leaseSawRecord ? 1 : 0, writerBlockedWhileLeaseHeld ? 1 : 0,
                 writerRanAfterLeaseReleased ? 1 : 0, rowCount, visitedBeforeStop);
        return leaseSawRecord && writerBlockedWhileLeaseHeld && writerRanAfterLeaseReleased &&
               rowCount == 2 && visitedBeforeStop == 1;
    }

    static void TestMdb(Db* db)
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        MdbTableRegistry* schemaRegistry = new MdbTableRegistry(FullTableList);
        AsyncDbWriter* dbWriter = new AsyncDbWriter(db, schemaRegistry);
        mdb->Subscribe(dbWriter);
        dbWriter->Subscribe(mdb);
        dbWriter->Start();

        this_thread::sleep_for(chrono::seconds(1));

        TradingDay* tradingDayRecord = InitTradingDay(mdb);
        InitExchange(mdb);
        InitAccount(mdb);

        ExchangeIdType exchangeID("CFFEX");
        {
            auto exchange = mdb->Exchange->PrimaryKey->Select(exchangeID);
            Print(exchange.Get());
        }

        {
            auto tradingDay = mdb->TradingDay->PrimaryKey->Select(1);
            Print(tradingDay.Get());
        }
        mdb->TradingDay->Erase(tradingDayRecord);
        mdb->TradingDay->TruncateTable();
        {
            auto exchangeRange = mdb->Exchange->PrimaryKey->SelectAll();
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
    const bool readLeaseHolds = testMdb::TestReadLeaseBlocksWriter();

    Logger::GetInstance().Stop();
    Logger::GetInstance().Join();
    return (batchInsertOwnershipHolds && readLeaseHolds) ? 0 : 1;
}
