#include <iostream>
#include "Mdb.h"
#include "DB.h"
#include "MysqlDB.h"
#include "SqliteDB.h"
#include "DuckDB.h"
#include "DBWriter.h"
#include "InitMdbFromDB.h"
#include "TimeUtility.h"
#include "Logger.h"

using namespace std;
using namespace mdb;

//const char* dbHost = "tcp://172.21.59.169:3306/SimExchange";
const char* dbHost = "tcp://localhost:3306/SimExchange";
const char* dbUser = "root";
const char* dbPasswd = "root";
const char* sqliteDBName = "../../../../../Sql/Sqlite/Test.sqlitedb";
const char* duckDBName = "../../../../../Sql/Duckdb/Test.duckdb";

static const char* InitTradingDay(Mdb* mdb)
{
	auto currDate = GetLocalDate();
	TradingDay* tradingday = new TradingDay();
	memset(tradingday, 0, sizeof(TradingDay));
	tradingday->PK = 1;
	GetPreTradingDay(currDate.c_str(), tradingday->PreTradingDay);
	GetNextTradingDay(tradingday->PreTradingDay, tradingday->CurrTradingDay);
	mdb->t_TradingDay->Insert(tradingday);
	return tradingday->CurrTradingDay;
}
static void InitExchange(Mdb* mdb)
{
	Exchange* exchange1 = new Exchange();
	Exchange* exchange2 = new Exchange();
	Exchange* exchange3 = new Exchange();
	Exchange* exchange4 = new Exchange();
	Exchange* exchange5 = new Exchange();
	Exchange* exchange6 = new Exchange();
	strcpy(exchange1->ExchangeID, "SHFE");
	strcpy(exchange1->ExchangeName, (const char*)(u8"上海期货交易所"));
	strcpy(exchange2->ExchangeID, "INE");
	strcpy(exchange2->ExchangeName, (const char*)(u8"上海能源中心"));
	strcpy(exchange3->ExchangeID, "CFFEX");
	strcpy(exchange3->ExchangeName, (const char*)(u8"中国金融期货交易所"));
	strcpy(exchange4->ExchangeID, "CZCE");
	strcpy(exchange4->ExchangeName, (const char*)(u8"郑州商品期货交易所"));
	strcpy(exchange5->ExchangeID, "DCE");
	strcpy(exchange5->ExchangeName, (const char*)(u8"大连商品期货交易所"));
	strcpy(exchange6->ExchangeID, "GFE");
	strcpy(exchange6->ExchangeName, (const char*)(u8"广州期货交易所"));

	mdb->t_Exchange->Insert(exchange1);
	mdb->t_Exchange->Insert(exchange2);
	mdb->t_Exchange->Insert(exchange3);
	mdb->t_Exchange->Insert(exchange4);
	mdb->t_Exchange->Insert(exchange5);
	mdb->t_Exchange->Insert(exchange6);
}
static void InitAccount(Mdb* mdb, const DateType tradingDay)
{
	Account* account = new Account();
	memset(account, 0, sizeof(Account));
	strcpy(account->TradingDay, tradingDay);
	strcpy(account->AccountID, "Xunmeng01");
	strcpy(account->AccountName, "Xunmeng01");
	strcpy(account->Password, "123456");
	account->PreBalance = 1000000;
	account->Balance = 1000000;
	account->CloseProfitByDate = 0;
	account->PositionProfitByDate = 0;
	account->PositionProfitByTrade = 0;
	account->PremiumIn = 0;
	account->PremiumOut = 0;
	account->MarketValue = 0;
	mdb->t_Account->Insert(account);
}

static const char* InitTradingDay(DB* db)
{
	auto currDate = GetLocalDate();
	TradingDay* tradingday = new TradingDay();
	memset(tradingday, 0, sizeof(TradingDay));
	tradingday->PK = 1;
	GetPreTradingDay(currDate.c_str(), tradingday->PreTradingDay);
	GetNextTradingDay(tradingday->PreTradingDay, tradingday->CurrTradingDay);
	db->InsertTradingDay(tradingday);
	return tradingday->CurrTradingDay;
}
static void InitExchange(DB* db)
{
	Exchange* exchange1 = new Exchange();
	Exchange* exchange2 = new Exchange();
	Exchange* exchange3 = new Exchange();
	Exchange* exchange4 = new Exchange();
	Exchange* exchange5 = new Exchange();
	Exchange* exchange6 = new Exchange();
	strcpy(exchange1->ExchangeID, "SHFE");
	strcpy(exchange1->ExchangeName, (const char*)(u8"上海期货交易所"));
	strcpy(exchange2->ExchangeID, "INE");
	strcpy(exchange2->ExchangeName, (const char*)(u8"上海能源中心"));
	strcpy(exchange3->ExchangeID, "CFFEX");
	strcpy(exchange3->ExchangeName, (const char*)(u8"中国金融期货交易所"));
	strcpy(exchange4->ExchangeID, "CZCE");
	strcpy(exchange4->ExchangeName, (const char*)(u8"郑州商品期货交易所"));
	strcpy(exchange5->ExchangeID, "DCE");
	strcpy(exchange5->ExchangeName, (const char*)(u8"大连商品期货交易所"));
	strcpy(exchange6->ExchangeID, "GFE");
	strcpy(exchange6->ExchangeName, (const char*)(u8"广州期货交易所"));

	std::list<Exchange*>* exchanges = new std::list<Exchange*>();
	exchanges->push_back(exchange1);
	exchanges->push_back(exchange2);
	exchanges->push_back(exchange3);
	exchanges->push_back(exchange4);
	exchanges->push_back(exchange5);
	exchanges->push_back(exchange6);
	db->BatchInsertExchange(exchanges);
}
static void InitAccount(DB* db, const DateType tradingDay)
{
	Account* account = new Account();
	memset(account, 0, sizeof(Account));
	strcpy(account->TradingDay, tradingDay);
	strcpy(account->AccountID, "Xunmeng01");
	strcpy(account->AccountName, "Xunmeng01");
	strcpy(account->Password, "123456");
	account->PreBalance = 1000000;
	account->Balance = 1000000;
	account->CloseProfitByDate = 0;
	account->PositionProfitByDate = 0;
	account->PositionProfitByTrade = 0;
	account->PremiumIn = 0;
	account->PremiumOut = 0;
	account->MarketValue = 0;
	db->InsertAccount(account);
}

static void TestMdb(DB* db)
{
	Mdb* mdb = new Mdb();
	DBWriter* dbWriter = new DBWriter(db);
	mdb->Subscribe(dbWriter);
	mdb->SetInitStatus(true);

	dbWriter->Start();
	mdb->TruncateTables();

	auto tradingDay = InitTradingDay(mdb);
	InitExchange(mdb);
	InitAccount(mdb, tradingDay);

	this_thread::sleep_for(chrono::seconds(1));
	dbWriter->Stop();
	dbWriter->Join();
}
static void TestDB(DB* db)
{
	if (!db->Connect())
	{
		WriteLog(LogLevel::Warning, "Connect Failed.");
		return;
	}
	db->TruncateTables();

	auto tradingDay = InitTradingDay(db);
	InitExchange(db);
	InitAccount(db, tradingDay);

	std::vector<Account*> accounts;
	db->SelectAccount(accounts);
	for (auto account : accounts)
	{
		WriteLog(LogLevel::Info, "%s", account->GetDebugString());
	}
	if (!accounts.empty())
	{
		auto account = accounts.front();
		auto newAccount = new Account();
		memcpy(newAccount, account, sizeof(Account));
		strcpy(newAccount->AccountName, "Jack01");
		newAccount->PremiumIn = 100;
		db->UpdateAccount(newAccount);
	}
	accounts.clear();

	vector<TradingDay*> tradingDays;
	db->SelectTradingDay(tradingDays);
	for (auto tradingDay : tradingDays)
	{
		WriteLog(LogLevel::Info, "%s", tradingDay->GetDebugString());

		db->DeleteTradingDay(tradingDay);
	}
	db->DisConnect();
}
static void Test()
{
	MysqlDB* mysqldb = new MysqlDB(dbHost, dbUser, dbPasswd);
	DuckDB* duckdb = new DuckDB(duckDBName);
	SqliteDB* sqlitedb = new SqliteDB(sqliteDBName);
	TestDB(mysqldb);
	TestDB(duckdb);
	TestDB(sqlitedb);
}

int main(int argc, char* argv[])
{
	Logger::GetInstance().Init(argv[0]);
	Logger::GetInstance().SetLogLevel(LogLevel::Info, LogLevel::Info);
	Logger::GetInstance().Start();

	Test();
	
	Logger::GetInstance().Stop();
	Logger::GetInstance().Join();
	return 0;
}
