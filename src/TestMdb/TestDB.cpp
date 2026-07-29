#include "Mdb/Mdb.h"
#include "Mdb/InitMdbFromDB.h"
#include "Mdb/MdbTableRegistry.h"
#include <DBAdapters/DBInterface/TypedTable.h>
#include <DBAdapters/DBInterface/SchemaRegistry.h>
#include <DBAdapters/AsyncDBWriter/AsyncDBWriter.h>
#include <DBAdapters/SqliteWrapper/SqliteWrapper.h>
#include <DBAdapters/DuckdbWrapper/DuckdbWrapper.h>
#include <DBAdapters/MysqlWrapper/MysqlWrapper.h>
#include <DBAdapters/MariadbWrapper/MariadbWrapper.h>
#include <Spark/Core/Core.h>
#include <iostream>


using namespace std;
using namespace mdb;

const char* sqliteDBName = "./Test.sqlitedb";
const char* duckdbDBName = "./Test.duckdb";
const char* mysqlHost = "mysqlx://sams:sams@localhost:33060/mdb";
const char* mariadbHost = "tcp://localhost:3306/mdb";
const char* mariadbUser = "sams";
const char* mariadbPassword = "sams";

TradingDay* PrepareTradingDay()
{
	auto currDate = TimeUtility::GetLocalDate();
	TradingDay* tradingDay = new TradingDay();
	memset(tradingDay, 0, sizeof(TradingDay));
	tradingDay->PK = 1;
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
	strcpy(exchange1->ExchangeID, "SHFE");
	strcpy(exchange1->ExchangeName, (const char*)(u8"上海期货交易所"));
	strcpy(exchange2->ExchangeID, "INE");
	strcpy(exchange2->ExchangeName, (const char*)(u8"上海国际能源交易中心"));
	strcpy(exchange3->ExchangeID, "CFFEX");
	strcpy(exchange3->ExchangeName, (const char*)(u8"中国金融期货交易所"));
	strcpy(exchange4->ExchangeID, "CZCE");
	strcpy(exchange4->ExchangeName, (const char*)(u8"郑州商品期货交易所"));
	strcpy(exchange5->ExchangeID, "DCE");
	strcpy(exchange5->ExchangeName, (const char*)(u8"大连商品期货交易所"));
	strcpy(exchange6->ExchangeID, "GFE");
	strcpy(exchange6->ExchangeName, (const char*)(u8"广期所"));

	exchanges->push_back(exchange1);
	exchanges->push_back(exchange2);
	exchanges->push_back(exchange3);
	exchanges->push_back(exchange4);
	exchanges->push_back(exchange5);
	exchanges->push_back(exchange6);
	return exchanges;
}
Account* PrepareAccount(const char* accountID, const char* accountName, const char* password)
{
	Account* account = new Account();
	memset(account, 0, sizeof(Account));
	strcpy(account->AccountID, accountID);
	strcpy(account->AccountName, accountName);
	account->AccountType = AccountTypeType::Primary;
	account->AccountStatus = AccountStatusType::Normal;
	strcpy(account->Password, "123456");
	account->TradeGroupID = 10000;
	account->RiskGroupID = 10000;
	account->CommissionGroupID = 10000;
	return account;
}

static void InitTradingDay(Mdb* mdb)
{
	auto tradingDay = PrepareTradingDay();
	mdb->t_TradingDay->Insert(tradingDay);
}
static void InitExchange(Mdb* mdb)
{
	auto exchanges = PrepareExchanges();
	for (auto exchange : *exchanges)
	{
		mdb->t_Exchange->Insert(exchange);
	}
	exchanges->clear();
	delete exchanges;
}
static void InitAccount(Mdb* mdb)
{
	Account* account = PrepareAccount("Xunmeng01", "Xunmeng01", "123456");
	mdb->t_Account->Insert(account);
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


static void TestMdb(DB* db)
{
	Mdb* mdb = new Mdb();
	SchemaRegistry* schemaRegistry = &mdb::MdbTableRegistry::Instance();
	AsyncDBWriter* dbWriter = new AsyncDBWriter(db, schemaRegistry);
	mdb->Subscribe(dbWriter);
	dbWriter->Subscribe(mdb);
	dbWriter->Start();

	this_thread::sleep_for(chrono::seconds(1));

	InitTradingDay(mdb);
	InitExchange(mdb);
	InitAccount(mdb);

	ExchangeIDType exchangeID("CFFEX");
	auto exchange = mdb->t_Exchange->m_PrimaryKey->Select(exchangeID);
	Print(exchange);

	auto tradingDay = mdb->t_TradingDay->m_PrimaryKey->Select(1);
	Print(tradingDay);
	mdb->t_TradingDay->Erase(tradingDay);
	mdb->t_TradingDay->TruncateTable();
	auto exchangePair = mdb->t_Exchange->m_PrimaryKey->SelectAll();
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
	this_thread::sleep_for(chrono::seconds(1));
}

static void TestDB(DB* db)
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

	TypedTable<TradingDay>    t_tradingDay(db);
	TypedTable<Exchange>      t_exchange(db);
	TypedTable<Account>       t_account(db);

	InitTradingDay(t_tradingDay);
	InitExchange(t_exchange);
	InitAccount(t_account);

	{
		std::vector<Account*> accounts;
		t_account.SelectAll(accounts);
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
			t_account.Update(newAccount);
		}
	}

	{
		std::vector<Exchange*> exchanges;
		t_exchange.SelectAll(exchanges);
		for (auto exchange : exchanges)
		{
			Print(exchange);
		}
		if (!exchanges.empty())
		{
			auto exchange = exchanges.front();
			t_exchange.Delete(*exchange);
		}
	}

    //db->DropAllTables(allSchemas, 11);
	db->DisConnect();
}

static void TestSqlite()
{
	SqliteWrapper* sqlitedb = new SqliteWrapper(sqliteDBName);
	WriteLog(LogLevel::Info, "TestDB with Sqlite");
	TestMdb(sqlitedb);
}
static void TestDuckdb()
{
    DuckdbWrapper* duckdb = new DuckdbWrapper(duckdbDBName);
    WriteLog(LogLevel::Info, "TestDB with Duckdb");
    TestMdb(duckdb);
}
static void TestMysql()
{
    MysqlWrapper* mysql = new MysqlWrapper(mysqlHost);
    WriteLog(LogLevel::Info, "TestDB with Mysql");
    TestMdb(mysql);
}
static void TestMariadb()
{
    MariadbWrapper* mariadb = new MariadbWrapper(mariadbHost, mariadbUser, mariadbPassword);
    WriteLog(LogLevel::Info, "TestDB with Mariadb");
    TestMdb(mariadb);
}

int main(int argc, char* argv[])
{
	Logger::GetInstance().Init(argv[0]);
	Logger::GetInstance().SetLogLevel(LogLevel::Info, LogLevel::Info);
	Logger::GetInstance().Start();

    TestSqlite();
    TestDuckdb();
    TestMysql();
    //TestMariadb();

	Logger::GetInstance().Stop();
	Logger::GetInstance().Join();
	return 0;
}
