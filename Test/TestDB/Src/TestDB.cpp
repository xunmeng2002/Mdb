#include <iostream>
#include "Mdb.h"
#include "DB.h"
#include "DuckDB.h"
//#include "MariaDB.h"
#include "MysqlDB.h"
#include "SqliteDB.h"
#include "DBWriter.h"
#include "InitMdbFromDB.h"
#include "TimeUtility.h"
#include "Logger.h"

using namespace std;
using namespace mdb;

const char* dbUser = "ams";
const char* dbPasswd = "ams";
const char* mariadbHost = "tcp://172.24.5.87:3306/Quant";
const char* mysqldbHost = "tcp://172.24.5.87:3306/Quant";
const char* sqliteDBName = "./Test.sqlitedb";
const char* duckDBName = "./Test.duckdb";

TradingDay* PrepareTradingDay()
{
	auto currDate = GetLocalDate();
	TradingDay* tradingDay = new TradingDay();
	memset(tradingDay, 0, sizeof(TradingDay));
	tradingDay->PK = 1;
	GetPreTradingDay(currDate.c_str(), tradingDay->PreTradingDay);
	GetNextTradingDay(tradingDay->PreTradingDay, tradingDay->CurrTradingDay);
	return tradingDay;
}
std::list<Exchange*>* PrepareExchanges()
{
	std::list<Exchange*>* exchanges = new std::list<Exchange*>();
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

void Print(Exchange* exchange)
{
	WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());
}
void Print(Account* account)
{
	WriteLog(LogLevel::Info, "%s", account->GetDebugString());
}

static void InitTradingDay(DB* db)
{
	auto tradingDay = PrepareTradingDay();
	db->InsertTradingDay(tradingDay);
}
static void InitExchange(DB* db)
{
	auto exchanges = PrepareExchanges();
	db->BatchInsertExchange(exchanges);
}
static void InitAccount(DB* db)
{
	Account* account = PrepareAccount("Xunmeng01", "Xunmeng01", "123456");
	db->InsertAccount(account);
}


static void TestMdb(DB* db)
{
	Mdb* mdb = new Mdb();
	DBWriter* dbWriter = new DBWriter(db);
	mdb->Subscribe(dbWriter);
	dbWriter->Subscribe(mdb);
	mdb->SetInitStatus(true);
	
	dbWriter->Start();
	mdb->CreateTables();
	mdb->InitDB();

	InitTradingDay(mdb);
	InitExchange(mdb);
	InitAccount(mdb);

	ExchangeIDType exchangeID("CFFEX");
	auto exchange = mdb->t_Exchange->m_PrimaryKey->Select(exchangeID);
	Print(exchange);
	
	auto exchangePair = mdb->t_Exchange->m_PrimaryKey->SelectAll();
	for (auto& it = exchangePair.first; it != exchangePair.second; ++it)
	{
		Print(*it);
	}

	this_thread::sleep_for(chrono::seconds(1));
	mdb->DropTables();
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
	db->CreateTables();
	db->TruncateTables();

	InitTradingDay(db);
	InitExchange(db);
	InitAccount(db);

	std::list<Account*> accounts;
	db->SelectAccount(accounts);
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
		db->UpdateAccount(&newAccount);
	}
	accounts.clear();

	list<Exchange*> exchanges;
	db->SelectExchange(exchanges);
	for (auto exchange : exchanges)
	{
		Print(exchange);
	}
	db->DeleteExchange(exchanges.front());
	db->DropTables();
	db->DisConnect();
}
static void Test()
{
	MysqlDB* mysqldb = new MysqlDB(mysqldbHost, dbUser, dbPasswd);
	//MariaDB* mariadb = new MariaDB(mariadbHost, dbUser, dbPasswd);
	DuckDB* duckdb = new DuckDB(duckDBName);
	SqliteDB* sqlitedb = new SqliteDB(sqliteDBName);

	//TestDB(mysqldb);
	//TestMdb(mariadb);
	//TestDB(duckdb);
	//TestDB(sqlitedb);

	//TestMdb(mysqldb);
	//TestMdb(mariadb);
	TestMdb(duckdb);
	//TestMdb(sqlitedb);
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
