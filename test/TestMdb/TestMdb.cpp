#include "Mdb.h"
#include "InitMdbFromDB.h"
#include "MdbTableRegistry.h"
#include "FullTableList.h"
#include <DBAdapters/DBInterface/TypedTable.h>
#include <DBAdapters/DBInterface/SchemaRegistry.h>
#include <DBAdapters/AsyncDbWriter/AsyncDbWriter.h>
#include <DBAdapters/SqliteWrapper/SqliteWrapper.h>
#include <DBAdapters/DuckdbWrapper/DuckdbWrapper.h>
#include <DBAdapters/MysqlWrapper/MysqlWrapper.h>
#include <DBAdapters/MariadbWrapper/MariadbWrapper.h>
#include <Spark/Core/Core.h>
#include <iostream>

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
        TradingDay* tradingDay = new TradingDay();
        memset(tradingDay, 0, sizeof(TradingDay));
        tradingDay->Pk = 1;
        TimeUtility::GetPreTradingDay(currDate.c_str(), tradingDay->PreTradingDay);
        TimeUtility::GetNextTradingDay(tradingDay->PreTradingDay, tradingDay->CurrTradingDay);
        return tradingDay;
    }
    list<Exchange*>* PrepareExchanges()
    {
        list<Exchange*>* exchanges = new list<Exchange*>();
        Exchange* exchange1 = new Exchange();
        Exchange* exchange2 = new Exchange();
        Exchange* exchange3 = new Exchange();
        Exchange* exchange4 = new Exchange();
        Exchange* exchange5 = new Exchange();
        Exchange* exchange6 = new Exchange();
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
        Account* account = new Account();
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

    static void InitTradingDay(::Mdb::Mdb* mdb)
    {
        auto tradingDay = PrepareTradingDay();
        mdb->TradingDay->Insert(tradingDay);
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

    void Print(TradingDay* tradingDay)
    {
        WriteLog(LogLevel::Info, "%s", tradingDay->GetDebugString());
    }
    void Print(Exchange* exchange)
    {
        WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());
    }
    void Print(Account* account)
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


    static void TestMdb(Db* db)
    {
        ::Mdb::Mdb* mdb = new ::Mdb::Mdb(FullTableList);
        MdbTableRegistry* schemaRegistry = new MdbTableRegistry(FullTableList);
        AsyncDbWriter* dbWriter = new AsyncDbWriter(db, schemaRegistry);
        mdb->Subscribe(dbWriter);
        dbWriter->Subscribe(mdb);
        dbWriter->Start();

        this_thread::sleep_for(chrono::seconds(1));

        InitTradingDay(mdb);
        InitExchange(mdb);
        InitAccount(mdb);

        ExchangeIdType exchangeID("CFFEX");
        auto exchange = mdb->Exchange->PrimaryKey->Select(exchangeID);
        Print(exchange);

        auto tradingDay = mdb->TradingDay->PrimaryKey->Select(1);
        Print(tradingDay);
        mdb->TradingDay->Erase(tradingDay);
        mdb->TradingDay->TruncateTable();
        auto exchangePair = mdb->Exchange->PrimaryKey->SelectAll();
        for (auto& it = exchangePair.first; it != exchangePair.second; ++it)
        {
            Print(*it);
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

    Logger::GetInstance().Stop();
    Logger::GetInstance().Join();
    return 0;
}
