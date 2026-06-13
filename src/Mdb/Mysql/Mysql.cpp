#pragma warning(disable: 4311)
#include "Mdb/Mysql/Mysql.h"
#include <PersonalLib/Core/Core.h>
#include <mysqlx/xdevapi.h>
#include <string.h>
#include <cstring>

using namespace mdb;
using namespace std;
using namespace std::chrono;

static void ParseRecord(const mysqlx::Row& row, std::list<mdb::TradingDay*>& records)
{
	auto record = TradingDay::Allocate();
	record->PK = row[1].get<int>();
	Utility::Strcpy(record->CurrTradingDay, row[2].get<std::string>().c_str());
	Utility::Strcpy(record->PreTradingDay, row[3].get<std::string>().c_str());
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Exchange*>& records)
{
	auto record = Exchange::Allocate();
	Utility::Strcpy(record->ExchangeID, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->ExchangeName, row[2].get<std::string>().c_str());
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Product*>& records)
{
	auto record = Product::Allocate();
	Utility::Strcpy(record->ExchangeID, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->ProductID, row[2].get<std::string>().c_str());
	Utility::Strcpy(record->ProductName, row[3].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[4].get<int>());
	record->VolumeMultiple = row[5].get<int>();
	record->PriceTick = row[6].get<double>();
	record->MaxMarketOrderVolume = row[7].get<int64_t>();
	record->MinMarketOrderVolume = row[8].get<int64_t>();
	record->MaxLimitOrderVolume = row[9].get<int64_t>();
	record->MinLimitOrderVolume = row[10].get<int64_t>();
	Utility::Strcpy(record->SessionName, row[11].get<std::string>().c_str());
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Instrument*>& records)
{
	auto record = Instrument::Allocate();
	Utility::Strcpy(record->ExchangeID, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentID, row[2].get<std::string>().c_str());
	Utility::Strcpy(record->ExchangeInstID, row[3].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentName, row[4].get<std::string>().c_str());
	Utility::Strcpy(record->ProductID, row[5].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[6].get<int>());
	record->InstrumentClass = InstrumentClassType(row[7].get<int>());
	record->Rank = row[8].get<int>();
	record->VolumeMultiple = row[9].get<int>();
	record->PriceTick = row[10].get<double>();
	record->MaxMarketOrderVolume = row[11].get<int64_t>();
	record->MinMarketOrderVolume = row[12].get<int64_t>();
	record->MaxLimitOrderVolume = row[13].get<int64_t>();
	record->MinLimitOrderVolume = row[14].get<int64_t>();
	Utility::Strcpy(record->SessionName, row[15].get<std::string>().c_str());
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::PrimaryAccount*>& records)
{
	auto record = PrimaryAccount::Allocate();
	Utility::Strcpy(record->PrimaryAccountID, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->PrimaryAccountName, row[2].get<std::string>().c_str());
	record->AccountClass = AccountClassType(row[3].get<int>());
	Utility::Strcpy(record->BrokerPassword, row[4].get<std::string>().c_str());
	record->OfferID = row[5].get<int>();
	record->IsAllowLogin = row[6].get<bool>();
	record->IsSimulateAccount = row[7].get<bool>();
	record->LoginStatus = LoginStatusType(row[8].get<int>());
	record->InitStatus = InitStatusType(row[9].get<int>());
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Account*>& records)
{
	auto record = Account::Allocate();
	Utility::Strcpy(record->AccountID, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountName, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	record->AccountStatus = AccountStatusType(row[4].get<int>());
	Utility::Strcpy(record->Password, row[5].get<std::string>().c_str());
	record->TradeGroupID = row[6].get<int>();
	record->RiskGroupID = row[7].get<int>();
	record->CommissionGroupID = row[8].get<int>();
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Capital*>& records)
{
	auto record = Capital::Allocate();
	Utility::Strcpy(record->TradingDay, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountID, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	record->Balance = row[4].get<double>();
	record->PreBalance = row[5].get<double>();
	record->Available = row[6].get<double>();
	record->MarketValue = row[7].get<double>();
	record->CashIn = row[8].get<double>();
	record->CashOut = row[9].get<double>();
	record->Margin = row[10].get<double>();
	record->Commission = row[11].get<double>();
	record->FrozenCash = row[12].get<double>();
	record->FrozenMargin = row[13].get<double>();
	record->FrozenCommission = row[14].get<double>();
	record->CloseProfitByDate = row[15].get<double>();
	record->CloseProfitByTrade = row[16].get<double>();
	record->PositionProfitByDate = row[17].get<double>();
	record->PositionProfitByTrade = row[18].get<double>();
	record->Deposit = row[19].get<double>();
	record->Withdraw = row[20].get<double>();
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Position*>& records)
{
	auto record = Position::Allocate();
	Utility::Strcpy(record->TradingDay, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountID, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	Utility::Strcpy(record->ExchangeID, row[4].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentID, row[5].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[6].get<int>());
	record->PosiDirection = PosiDirectionType(row[7].get<int>());
	record->TotalPosition = row[8].get<int64_t>();
	record->PositionFrozen = row[9].get<int64_t>();
	record->TodayPosition = row[10].get<int64_t>();
	record->MarketValue = row[11].get<double>();
	record->CashIn = row[12].get<double>();
	record->CashOut = row[13].get<double>();
	record->Margin = row[14].get<double>();
	record->Commission = row[15].get<double>();
	record->FrozenCash = row[16].get<double>();
	record->FrozenMargin = row[17].get<double>();
	record->FrozenCommission = row[18].get<double>();
	record->VolumeMultiple = row[19].get<int>();
	record->CloseProfitByDate = row[20].get<double>();
	record->CloseProfitByTrade = row[21].get<double>();
	record->PositionProfitByDate = row[22].get<double>();
	record->PositionProfitByTrade = row[23].get<double>();
	record->SettlementPrice = row[24].get<double>();
	record->PreSettlementPrice = row[25].get<double>();
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::PositionDetail*>& records)
{
	auto record = PositionDetail::Allocate();
	Utility::Strcpy(record->TradingDay, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountID, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	Utility::Strcpy(record->ExchangeID, row[4].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentID, row[5].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[6].get<int>());
	record->PosiDirection = PosiDirectionType(row[7].get<int>());
	Utility::Strcpy(record->OpenDate, row[8].get<std::string>().c_str());
	Utility::Strcpy(record->TradeID, row[9].get<std::string>().c_str());
	record->Volume = row[10].get<int64_t>();
	record->OpenPrice = row[11].get<double>();
	record->MarketValue = row[12].get<double>();
	record->CashIn = row[13].get<double>();
	record->CashOut = row[14].get<double>();
	record->Margin = row[15].get<double>();
	record->Commission = row[16].get<double>();
	record->VolumeMultiple = row[17].get<int>();
	record->CloseProfitByDate = row[18].get<double>();
	record->CloseProfitByTrade = row[19].get<double>();
	record->PositionProfitByDate = row[20].get<double>();
	record->PositionProfitByTrade = row[21].get<double>();
	record->SettlementPrice = row[22].get<double>();
	record->PreSettlementPrice = row[23].get<double>();
	record->CloseVolume = row[24].get<int64_t>();
	record->CloseAmount = row[25].get<double>();
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Order*>& records)
{
	auto record = Order::Allocate();
	Utility::Strcpy(record->TradingDay, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountID, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	Utility::Strcpy(record->ExchangeID, row[4].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentID, row[5].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[6].get<int>());
	record->OrderID = row[7].get<int>();
	Utility::Strcpy(record->OrderSysID, row[8].get<std::string>().c_str());
	record->Direction = DirectionType(row[9].get<int>());
	record->OffsetFlag = OffsetFlagType(row[10].get<int>());
	record->OrderPriceType = OrderPriceTypeType(row[11].get<int>());
	record->Price = row[12].get<double>();
	record->Volume = row[13].get<int64_t>();
	record->VolumeTotal = row[14].get<int64_t>();
	record->VolumeTraded = row[15].get<int64_t>();
	record->VolumeMultiple = row[16].get<int>();
	record->OrderStatus = OrderStatusType(row[17].get<int>());
	Utility::Strcpy(record->OrderDate, row[18].get<std::string>().c_str());
	Utility::Strcpy(record->OrderTime, row[19].get<std::string>().c_str());
	Utility::Strcpy(record->CancelDate, row[20].get<std::string>().c_str());
	Utility::Strcpy(record->CancelTime, row[21].get<std::string>().c_str());
	record->SessionID = row[22].get<int64_t>();
	record->ClientOrderID = row[23].get<int>();
	record->RequestID = row[24].get<int>();
	record->OfferID = row[25].get<int>();
	record->TradeGroupID = row[26].get<int>();
	record->RiskGroupID = row[27].get<int>();
	record->CommissionGroupID = row[28].get<int>();
	record->FrozenCash = row[29].get<double>();
	record->FrozenMargin = row[30].get<double>();
	record->FrozenCommission = row[31].get<double>();
	record->RebuildMark = row[32].get<bool>();
	record->IsForceClose = row[33].get<bool>();
	records.push_back(record);
}
static void ParseRecord(const mysqlx::Row& row, std::list<mdb::Trade*>& records)
{
	auto record = Trade::Allocate();
	Utility::Strcpy(record->TradingDay, row[1].get<std::string>().c_str());
	Utility::Strcpy(record->AccountID, row[2].get<std::string>().c_str());
	record->AccountType = AccountTypeType(row[3].get<int>());
	Utility::Strcpy(record->ExchangeID, row[4].get<std::string>().c_str());
	Utility::Strcpy(record->InstrumentID, row[5].get<std::string>().c_str());
	record->ProductClass = ProductClassType(row[6].get<int>());
	record->OrderID = row[7].get<int>();
	Utility::Strcpy(record->OrderSysID, row[8].get<std::string>().c_str());
	Utility::Strcpy(record->TradeID, row[9].get<std::string>().c_str());
	record->Direction = DirectionType(row[10].get<int>());
	record->OffsetFlag = OffsetFlagType(row[11].get<int>());
	record->Price = row[12].get<double>();
	record->Volume = row[13].get<int64_t>();
	record->VolumeMultiple = row[14].get<int>();
	record->TradeAmount = row[15].get<double>();
	record->Commission = row[16].get<double>();
	Utility::Strcpy(record->TradeDate, row[17].get<std::string>().c_str());
	Utility::Strcpy(record->TradeTime, row[18].get<std::string>().c_str());
	records.push_back(record);
}

struct Mysql::Impl
{
	Impl(mysqlx::Session&& session, mysqlx::Schema&& defaultSchema)
		:m_Session(std::move(session)), m_DefaultSchema(std::move(defaultSchema))
	{
	}
	~Impl() = default;

	mysqlx::Session m_Session;
	mysqlx::Schema m_DefaultSchema;
};

Mysql::Mysql(const std::string& host)
	:m_Impl(nullptr), m_Host(host)
{
}
Mysql::~Mysql()
{
	DisConnect();
}
bool Mysql::Connect()
{
	try
	{
		auto session = mysqlx::Session(m_Host);
        auto schema = session.getDefaultSchema();
		m_Impl = std::unique_ptr<Impl>(new Impl(std::move(session), std::move(schema)));
        WriteLog(LogLevel::Info, "Mysql X DevAPI Connect Success");
	}
	catch (std::exception e)
	{
		WriteLog(LogLevel::Warning, "Connect Mysql Failed. Msg:%s", e.what());
		return false;
	}
	return true;
}
void Mysql::DisConnect()
{
	if (m_Impl)
	{
		m_Impl->m_Session.close();
		m_Impl.reset();
	}
}
void Mysql::InitDB()
{
	m_Impl->m_Session.sql("Truncate Table t_TradingDay;").execute();
	m_Impl->m_Session.sql("Insert Into t_TradingDay select * from Init.t_TradingDay;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Exchange;").execute();
	m_Impl->m_Session.sql("Insert Into t_Exchange select * from Init.t_Exchange;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Product;").execute();
	m_Impl->m_Session.sql("Insert Into t_Product select * from Init.t_Product;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Instrument;").execute();
	m_Impl->m_Session.sql("Insert Into t_Instrument select * from Init.t_Instrument;").execute();
	m_Impl->m_Session.sql("Truncate Table t_PrimaryAccount;").execute();
	m_Impl->m_Session.sql("Insert Into t_PrimaryAccount select * from Init.t_PrimaryAccount;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Account;").execute();
	m_Impl->m_Session.sql("Insert Into t_Account select * from Init.t_Account;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Capital;").execute();
	m_Impl->m_Session.sql("Insert Into t_Capital select * from Init.t_Capital;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Position;").execute();
	m_Impl->m_Session.sql("Insert Into t_Position select * from Init.t_Position;").execute();
	m_Impl->m_Session.sql("Truncate Table t_PositionDetail;").execute();
	m_Impl->m_Session.sql("Insert Into t_PositionDetail select * from Init.t_PositionDetail;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Order;").execute();
	m_Impl->m_Session.sql("Insert Into t_Order select * from Init.t_Order;").execute();
	m_Impl->m_Session.sql("Truncate Table t_Trade;").execute();
	m_Impl->m_Session.sql("Insert Into t_Trade select * from Init.t_Trade;").execute();
}
void Mysql::CreateTables()
{
	CreateTradingDay();
	CreateExchange();
	CreateProduct();
	CreateInstrument();
	CreatePrimaryAccount();
	CreateAccount();
	CreateCapital();
	CreatePosition();
	CreatePositionDetail();
	CreateOrder();
	CreateTrade();
}
void Mysql::DropTables()
{
	DropTradingDay();
	DropExchange();
	DropProduct();
	DropInstrument();
	DropPrimaryAccount();
	DropAccount();
	DropCapital();
	DropPosition();
	DropPositionDetail();
	DropOrder();
	DropTrade();
}
void Mysql::TruncateTables()
{
	TruncateTradingDay();
	TruncateExchange();
	TruncateProduct();
	TruncatePrimaryAccount();
	TruncateAccount();
	TruncateCapital();
	TruncatePosition();
	TruncatePositionDetail();
	TruncateOrder();
	TruncateTrade();
}
void Mysql::TruncateSessionTables()
{
	auto start = steady_clock::now();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}

void Mysql::CustomExecuteSql(const char* sql)
{
	m_Impl->m_Session.sql(sql).execute();
}

void Mysql::CreateTradingDay()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_TradingDay(`PK` int, `CurrTradingDay` char(16), `PreTradingDay` char(16), PRIMARY KEY(PK)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropTradingDay()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_TradingDay;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_TradingDay");
	
    table.insert("PK", "CurrTradingDay", "PreTradingDay")
		.values(record->PK, record->CurrTradingDay, record->PreTradingDay)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_TradingDay");
	
    auto insert = table.insert("PK", "CurrTradingDay", "PreTradingDay");
	for (auto record : *records)
	{
        insert.values(record->PK, record->CurrTradingDay, record->PreTradingDay);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_TradingDay");
    table.remove()
		.where("PK = :PK")
		.bind("PK", record->PK)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void Mysql::UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_TradingDay");
    table.update()
		.set("PK", record->PK)
		.set("CurrTradingDay", record->CurrTradingDay)
		.set("PreTradingDay", record->PreTradingDay)
		.where("PK = :PK")
		.bind("PK", record->PK)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void Mysql::SelectTradingDay(std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_TradingDay");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void Mysql::SelectTradingDayWithSql(const char* sql, std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void Mysql::TruncateTradingDay()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_TradingDay").execute();
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateExchange()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Exchange(`ExchangeID` char(8), `ExchangeName` char(64), PRIMARY KEY(ExchangeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateExchange Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropExchange()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Exchange;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropExchange Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertExchange(Exchange* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Exchange");
	
    table.insert("ExchangeID", "ExchangeName")
		.values(record->ExchangeID, record->ExchangeName)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertExchange Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertExchange(std::list<Exchange*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Exchange");
	
    auto insert = table.insert("ExchangeID", "ExchangeName");
	for (auto record : *records)
	{
        insert.values(record->ExchangeID, record->ExchangeName);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Exchange");
    table.remove()
		.where("ExchangeID = :ExchangeID")
		.bind("ExchangeID", record->ExchangeID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void Mysql::UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Exchange");
    table.update()
		.set("ExchangeID", record->ExchangeID)
		.set("ExchangeName", record->ExchangeName)
		.where("ExchangeID = :ExchangeID")
		.bind("ExchangeID", record->ExchangeID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void Mysql::SelectExchange(std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Exchange");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void Mysql::SelectExchangeWithSql(const char* sql, std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void Mysql::TruncateExchange()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Exchange").execute();
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateProduct()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Product(`ExchangeID` char(8), `ProductID` char(32), `ProductName` char(32), `ProductClass` int, `VolumeMultiple` int, `PriceTick` decimal(24,8), `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` char(32), PRIMARY KEY(ExchangeID, ProductID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateProduct Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropProduct()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Product;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropProduct Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertProduct(Product* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Product");
	
    table.insert("ExchangeID", "ProductID", "ProductName", "ProductClass", "VolumeMultiple", "PriceTick", "MaxMarketOrderVolume", "MinMarketOrderVolume", "MaxLimitOrderVolume", "MinLimitOrderVolume", "SessionName")
		.values(record->ExchangeID, record->ProductID, record->ProductName, (int)record->ProductClass, record->VolumeMultiple, record->PriceTick, record->MaxMarketOrderVolume, record->MinMarketOrderVolume, record->MaxLimitOrderVolume, record->MinLimitOrderVolume, record->SessionName)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertProduct Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertProduct(std::list<Product*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Product");
	
    auto insert = table.insert("ExchangeID", "ProductID", "ProductName", "ProductClass", "VolumeMultiple", "PriceTick", "MaxMarketOrderVolume", "MinMarketOrderVolume", "MaxLimitOrderVolume", "MinLimitOrderVolume", "SessionName");
	for (auto record : *records)
	{
        insert.values(record->ExchangeID, record->ProductID, record->ProductName, (int)record->ProductClass, record->VolumeMultiple, record->PriceTick, record->MaxMarketOrderVolume, record->MinMarketOrderVolume, record->MaxLimitOrderVolume, record->MinLimitOrderVolume, record->SessionName);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Product");
    table.remove()
		.where("ExchangeID = :ExchangeID and ProductID = :ProductID")
		.bind("ExchangeID", record->ExchangeID)
		.bind("ProductID", record->ProductID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void Mysql::UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Product");
    table.update()
		.set("ExchangeID", record->ExchangeID)
		.set("ProductID", record->ProductID)
		.set("ProductName", record->ProductName)
		.set("ProductClass", (int)record->ProductClass)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("PriceTick", record->PriceTick)
		.set("MaxMarketOrderVolume", record->MaxMarketOrderVolume)
		.set("MinMarketOrderVolume", record->MinMarketOrderVolume)
		.set("MaxLimitOrderVolume", record->MaxLimitOrderVolume)
		.set("MinLimitOrderVolume", record->MinLimitOrderVolume)
		.set("SessionName", record->SessionName)
		.where("ExchangeID = :ExchangeID and ProductID = :ProductID")
		.bind("ExchangeID", record->ExchangeID)
		.bind("ProductID", record->ProductID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void Mysql::SelectProduct(std::list<Product*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Product");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void Mysql::SelectProductWithSql(const char* sql, std::list<Product*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void Mysql::TruncateProduct()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Product").execute();
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateInstrument()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Instrument(`ExchangeID` char(8), `InstrumentID` char(32), `ExchangeInstID` char(32), `InstrumentName` char(64), `ProductID` char(32), `ProductClass` int, `InstrumentClass` int, `Rank` int, `VolumeMultiple` int, `PriceTick` decimal(24,8), `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` char(32), PRIMARY KEY(ExchangeID, InstrumentID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateInstrument Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropInstrument()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Instrument;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropInstrument Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Instrument");
	
    table.insert("ExchangeID", "InstrumentID", "ExchangeInstID", "InstrumentName", "ProductID", "ProductClass", "InstrumentClass", "Rank", "VolumeMultiple", "PriceTick", "MaxMarketOrderVolume", "MinMarketOrderVolume", "MaxLimitOrderVolume", "MinLimitOrderVolume", "SessionName")
		.values(record->ExchangeID, record->InstrumentID, record->ExchangeInstID, record->InstrumentName, record->ProductID, (int)record->ProductClass, (int)record->InstrumentClass, record->Rank, record->VolumeMultiple, record->PriceTick, record->MaxMarketOrderVolume, record->MinMarketOrderVolume, record->MaxLimitOrderVolume, record->MinLimitOrderVolume, record->SessionName)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertInstrument(std::list<Instrument*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Instrument");
	
    auto insert = table.insert("ExchangeID", "InstrumentID", "ExchangeInstID", "InstrumentName", "ProductID", "ProductClass", "InstrumentClass", "Rank", "VolumeMultiple", "PriceTick", "MaxMarketOrderVolume", "MinMarketOrderVolume", "MaxLimitOrderVolume", "MinLimitOrderVolume", "SessionName");
	for (auto record : *records)
	{
        insert.values(record->ExchangeID, record->InstrumentID, record->ExchangeInstID, record->InstrumentName, record->ProductID, (int)record->ProductClass, (int)record->InstrumentClass, record->Rank, record->VolumeMultiple, record->PriceTick, record->MaxMarketOrderVolume, record->MinMarketOrderVolume, record->MaxLimitOrderVolume, record->MinLimitOrderVolume, record->SessionName);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Instrument");
    table.remove()
		.where("ExchangeID = :ExchangeID and InstrumentID = :InstrumentID")
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void Mysql::UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Instrument");
    table.update()
		.set("ExchangeID", record->ExchangeID)
		.set("InstrumentID", record->InstrumentID)
		.set("ExchangeInstID", record->ExchangeInstID)
		.set("InstrumentName", record->InstrumentName)
		.set("ProductID", record->ProductID)
		.set("ProductClass", (int)record->ProductClass)
		.set("InstrumentClass", (int)record->InstrumentClass)
		.set("Rank", record->Rank)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("PriceTick", record->PriceTick)
		.set("MaxMarketOrderVolume", record->MaxMarketOrderVolume)
		.set("MinMarketOrderVolume", record->MinMarketOrderVolume)
		.set("MaxLimitOrderVolume", record->MaxLimitOrderVolume)
		.set("MinLimitOrderVolume", record->MinLimitOrderVolume)
		.set("SessionName", record->SessionName)
		.where("ExchangeID = :ExchangeID and InstrumentID = :InstrumentID")
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void Mysql::SelectInstrument(std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Instrument");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void Mysql::SelectInstrumentWithSql(const char* sql, std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void Mysql::TruncateInstrument()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Instrument").execute();
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreatePrimaryAccount()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(`PrimaryAccountID` char(32), `PrimaryAccountName` char(64), `AccountClass` int, `BrokerPassword` char(64), `OfferID` int, `IsAllowLogin` bool, `IsSimulateAccount` bool, `LoginStatus` int, `InitStatus` int, INDEX PrimaryAccountOfferID(OfferID), PRIMARY KEY(PrimaryAccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropPrimaryAccount()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_PrimaryAccount;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertPrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
	
    table.insert("PrimaryAccountID", "PrimaryAccountName", "AccountClass", "BrokerPassword", "OfferID", "IsAllowLogin", "IsSimulateAccount", "LoginStatus", "InitStatus")
		.values(record->PrimaryAccountID, record->PrimaryAccountName, (int)record->AccountClass, record->BrokerPassword, record->OfferID, record->IsAllowLogin, record->IsSimulateAccount, (int)record->LoginStatus, (int)record->InitStatus)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPrimaryAccount Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertPrimaryAccount(std::list<PrimaryAccount*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
	
    auto insert = table.insert("PrimaryAccountID", "PrimaryAccountName", "AccountClass", "BrokerPassword", "OfferID", "IsAllowLogin", "IsSimulateAccount", "LoginStatus", "InitStatus");
	for (auto record : *records)
	{
        insert.values(record->PrimaryAccountID, record->PrimaryAccountName, (int)record->AccountClass, record->BrokerPassword, record->OfferID, record->IsAllowLogin, record->IsSimulateAccount, (int)record->LoginStatus, (int)record->InitStatus);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeletePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
    table.remove()
		.where("PrimaryAccountID = :PrimaryAccountID")
		.bind("PrimaryAccountID", record->PrimaryAccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccount Spend:%lldms", duration);
	}
}
void Mysql::DeletePrimaryAccountByOfferIDIndex(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
    table.remove()
		.where("OfferID = :OfferID")
		.bind("OfferID", record->OfferID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccountByOfferIDIndex Spend:%lldms", duration);
	}
}
void Mysql::UpdatePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
    table.update()
		.set("PrimaryAccountID", record->PrimaryAccountID)
		.set("PrimaryAccountName", record->PrimaryAccountName)
		.set("AccountClass", (int)record->AccountClass)
		.set("BrokerPassword", record->BrokerPassword)
		.set("OfferID", record->OfferID)
		.set("IsAllowLogin", record->IsAllowLogin)
		.set("IsSimulateAccount", record->IsSimulateAccount)
		.set("LoginStatus", (int)record->LoginStatus)
		.set("InitStatus", (int)record->InitStatus)
		.where("PrimaryAccountID = :PrimaryAccountID")
		.bind("PrimaryAccountID", record->PrimaryAccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePrimaryAccount Spend:%lldms", duration);
	}
}
void Mysql::SelectPrimaryAccount(std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PrimaryAccount");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccount Spend:%lldms", duration);
	}
}
void Mysql::SelectPrimaryAccountWithSql(const char* sql, std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccount Spend:%lldms", duration);
	}
}
void Mysql::TruncatePrimaryAccount()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_PrimaryAccount").execute();
	WriteLog(LogLevel::Info, "TruncatePrimaryAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateAccount()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Account(`AccountID` char(32), `AccountName` char(64), `AccountType` int, `AccountStatus` int, `Password` char(64), `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, PRIMARY KEY(AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateAccount Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropAccount()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Account;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropAccount Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertAccount(Account* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Account");
	
    table.insert("AccountID", "AccountName", "AccountType", "AccountStatus", "Password", "TradeGroupID", "RiskGroupID", "CommissionGroupID")
		.values(record->AccountID, record->AccountName, (int)record->AccountType, (int)record->AccountStatus, record->Password, record->TradeGroupID, record->RiskGroupID, record->CommissionGroupID)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertAccount Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertAccount(std::list<Account*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Account");
	
    auto insert = table.insert("AccountID", "AccountName", "AccountType", "AccountStatus", "Password", "TradeGroupID", "RiskGroupID", "CommissionGroupID");
	for (auto record : *records)
	{
        insert.values(record->AccountID, record->AccountName, (int)record->AccountType, (int)record->AccountStatus, record->Password, record->TradeGroupID, record->RiskGroupID, record->CommissionGroupID);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Account");
    table.remove()
		.where("AccountID = :AccountID")
		.bind("AccountID", record->AccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void Mysql::UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Account");
    table.update()
		.set("AccountID", record->AccountID)
		.set("AccountName", record->AccountName)
		.set("AccountType", (int)record->AccountType)
		.set("AccountStatus", (int)record->AccountStatus)
		.set("Password", record->Password)
		.set("TradeGroupID", record->TradeGroupID)
		.set("RiskGroupID", record->RiskGroupID)
		.set("CommissionGroupID", record->CommissionGroupID)
		.where("AccountID = :AccountID")
		.bind("AccountID", record->AccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void Mysql::SelectAccount(std::list<Account*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Account");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void Mysql::SelectAccountWithSql(const char* sql, std::list<Account*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void Mysql::TruncateAccount()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Account").execute();
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateCapital()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Capital(`TradingDay` char(16), `AccountID` char(32), `AccountType` int, `Balance` decimal(24,8), `PreBalance` decimal(24,8), `Available` decimal(24,8), `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `Deposit` decimal(24,8), `Withdraw` decimal(24,8), INDEX CapitalTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateCapital Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropCapital()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Capital;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropCapital Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertCapital(Capital* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
	
    table.insert("TradingDay", "AccountID", "AccountType", "Balance", "PreBalance", "Available", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "FrozenCash", "FrozenMargin", "FrozenCommission", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "Deposit", "Withdraw")
		.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->Balance, record->PreBalance, record->Available, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->Deposit, record->Withdraw)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertCapital Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertCapital(std::list<Capital*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
	
    auto insert = table.insert("TradingDay", "AccountID", "AccountType", "Balance", "PreBalance", "Available", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "FrozenCash", "FrozenMargin", "FrozenCommission", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "Deposit", "Withdraw");
	for (auto record : *records)
	{
        insert.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->Balance, record->PreBalance, record->Available, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->Deposit, record->Withdraw);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertCapital RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteCapital(Capital* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapital Spend:%lldms", duration);
	}
}
void Mysql::DeleteCapitalByTradingDayIndex(Capital* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
    table.remove()
		.where("TradingDay = :TradingDay")
		.bind("TradingDay", record->TradingDay)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapitalByTradingDayIndex Spend:%lldms", duration);
	}
}
void Mysql::UpdateCapital(Capital* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
    table.update()
		.set("TradingDay", record->TradingDay)
		.set("AccountID", record->AccountID)
		.set("AccountType", (int)record->AccountType)
		.set("Balance", record->Balance)
		.set("PreBalance", record->PreBalance)
		.set("Available", record->Available)
		.set("MarketValue", record->MarketValue)
		.set("CashIn", record->CashIn)
		.set("CashOut", record->CashOut)
		.set("Margin", record->Margin)
		.set("Commission", record->Commission)
		.set("FrozenCash", record->FrozenCash)
		.set("FrozenMargin", record->FrozenMargin)
		.set("FrozenCommission", record->FrozenCommission)
		.set("CloseProfitByDate", record->CloseProfitByDate)
		.set("CloseProfitByTrade", record->CloseProfitByTrade)
		.set("PositionProfitByDate", record->PositionProfitByDate)
		.set("PositionProfitByTrade", record->PositionProfitByTrade)
		.set("Deposit", record->Deposit)
		.set("Withdraw", record->Withdraw)
		.where("TradingDay = :TradingDay and AccountID = :AccountID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateCapital Spend:%lldms", duration);
	}
}
void Mysql::SelectCapital(std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Capital");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapital Spend:%lldms", duration);
	}
}
void Mysql::SelectCapitalWithSql(const char* sql, std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapital Spend:%lldms", duration);
	}
}
void Mysql::TruncateCapital()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Capital").execute();
	WriteLog(LogLevel::Info, "TruncateCapital Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreatePosition()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Position(`TradingDay` char(16), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `PosiDirection` int, `TotalPosition` bigint, `PositionFrozen` bigint, `TodayPosition` bigint, `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `VolumeMultiple` int, `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `SettlementPrice` decimal(24,8), `PreSettlementPrice` decimal(24,8), INDEX PositionAccount(TradingDay, AccountID), INDEX PositionTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePosition Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropPosition()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Position;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPosition Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertPosition(Position* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
	
    table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "PosiDirection", "TotalPosition", "PositionFrozen", "TodayPosition", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "FrozenCash", "FrozenMargin", "FrozenCommission", "VolumeMultiple", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "SettlementPrice", "PreSettlementPrice")
		.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, (int)record->PosiDirection, record->TotalPosition, record->PositionFrozen, record->TodayPosition, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->VolumeMultiple, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->SettlementPrice, record->PreSettlementPrice)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPosition Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertPosition(std::list<Position*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
	
    auto insert = table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "PosiDirection", "TotalPosition", "PositionFrozen", "TodayPosition", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "FrozenCash", "FrozenMargin", "FrozenCommission", "VolumeMultiple", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "SettlementPrice", "PreSettlementPrice");
	for (auto record : *records)
	{
        insert.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, (int)record->PosiDirection, record->TotalPosition, record->PositionFrozen, record->TodayPosition, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->VolumeMultiple, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->SettlementPrice, record->PreSettlementPrice);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and PosiDirection = :PosiDirection")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("PosiDirection", (int)record->PosiDirection)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void Mysql::DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void Mysql::DeletePositionByTradingDayIndex(Position* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
    table.remove()
		.where("TradingDay = :TradingDay")
		.bind("TradingDay", record->TradingDay)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByTradingDayIndex Spend:%lldms", duration);
	}
}
void Mysql::UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
    table.update()
		.set("TradingDay", record->TradingDay)
		.set("AccountID", record->AccountID)
		.set("AccountType", (int)record->AccountType)
		.set("ExchangeID", record->ExchangeID)
		.set("InstrumentID", record->InstrumentID)
		.set("ProductClass", (int)record->ProductClass)
		.set("PosiDirection", (int)record->PosiDirection)
		.set("TotalPosition", record->TotalPosition)
		.set("PositionFrozen", record->PositionFrozen)
		.set("TodayPosition", record->TodayPosition)
		.set("MarketValue", record->MarketValue)
		.set("CashIn", record->CashIn)
		.set("CashOut", record->CashOut)
		.set("Margin", record->Margin)
		.set("Commission", record->Commission)
		.set("FrozenCash", record->FrozenCash)
		.set("FrozenMargin", record->FrozenMargin)
		.set("FrozenCommission", record->FrozenCommission)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("CloseProfitByDate", record->CloseProfitByDate)
		.set("CloseProfitByTrade", record->CloseProfitByTrade)
		.set("PositionProfitByDate", record->PositionProfitByDate)
		.set("PositionProfitByTrade", record->PositionProfitByTrade)
		.set("SettlementPrice", record->SettlementPrice)
		.set("PreSettlementPrice", record->PreSettlementPrice)
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and PosiDirection = :PosiDirection")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("PosiDirection", (int)record->PosiDirection)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void Mysql::SelectPosition(std::list<Position*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Position");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void Mysql::SelectPositionWithSql(const char* sql, std::list<Position*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void Mysql::TruncatePosition()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Position").execute();
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreatePositionDetail()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_PositionDetail(`TradingDay` char(16), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `PosiDirection` int, `OpenDate` char(16), `TradeID` char(64), `Volume` bigint, `OpenPrice` decimal(24,8), `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `VolumeMultiple` int, `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `SettlementPrice` decimal(24,8), `PreSettlementPrice` decimal(24,8), `CloseVolume` bigint, `CloseAmount` decimal(24,8), INDEX PositionDetailTradeMatch(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection), INDEX PositionDetailTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropPositionDetail()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_PositionDetail;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertPositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
	
    table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "PosiDirection", "OpenDate", "TradeID", "Volume", "OpenPrice", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "VolumeMultiple", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "SettlementPrice", "PreSettlementPrice", "CloseVolume", "CloseAmount")
		.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, (int)record->PosiDirection, record->OpenDate, record->TradeID, record->Volume, record->OpenPrice, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->VolumeMultiple, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->SettlementPrice, record->PreSettlementPrice, record->CloseVolume, record->CloseAmount)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPositionDetail Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertPositionDetail(std::list<PositionDetail*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
	
    auto insert = table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "PosiDirection", "OpenDate", "TradeID", "Volume", "OpenPrice", "MarketValue", "CashIn", "CashOut", "Margin", "Commission", "VolumeMultiple", "CloseProfitByDate", "CloseProfitByTrade", "PositionProfitByDate", "PositionProfitByTrade", "SettlementPrice", "PreSettlementPrice", "CloseVolume", "CloseAmount");
	for (auto record : *records)
	{
        insert.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, (int)record->PosiDirection, record->OpenDate, record->TradeID, record->Volume, record->OpenPrice, record->MarketValue, record->CashIn, record->CashOut, record->Margin, record->Commission, record->VolumeMultiple, record->CloseProfitByDate, record->CloseProfitByTrade, record->PositionProfitByDate, record->PositionProfitByTrade, record->SettlementPrice, record->PreSettlementPrice, record->CloseVolume, record->CloseAmount);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPositionDetail RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeletePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and PosiDirection = :PosiDirection and OpenDate = :OpenDate and TradeID = :TradeID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("PosiDirection", (int)record->PosiDirection)
		.bind("OpenDate", record->OpenDate)
		.bind("TradeID", record->TradeID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetail Spend:%lldms", duration);
	}
}
void Mysql::DeletePositionDetailByTradeMatchIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and PosiDirection = :PosiDirection")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("PosiDirection", (int)record->PosiDirection)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradeMatchIndex Spend:%lldms", duration);
	}
}
void Mysql::DeletePositionDetailByTradingDayIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
    table.remove()
		.where("TradingDay = :TradingDay")
		.bind("TradingDay", record->TradingDay)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradingDayIndex Spend:%lldms", duration);
	}
}
void Mysql::UpdatePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
    table.update()
		.set("TradingDay", record->TradingDay)
		.set("AccountID", record->AccountID)
		.set("AccountType", (int)record->AccountType)
		.set("ExchangeID", record->ExchangeID)
		.set("InstrumentID", record->InstrumentID)
		.set("ProductClass", (int)record->ProductClass)
		.set("PosiDirection", (int)record->PosiDirection)
		.set("OpenDate", record->OpenDate)
		.set("TradeID", record->TradeID)
		.set("Volume", record->Volume)
		.set("OpenPrice", record->OpenPrice)
		.set("MarketValue", record->MarketValue)
		.set("CashIn", record->CashIn)
		.set("CashOut", record->CashOut)
		.set("Margin", record->Margin)
		.set("Commission", record->Commission)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("CloseProfitByDate", record->CloseProfitByDate)
		.set("CloseProfitByTrade", record->CloseProfitByTrade)
		.set("PositionProfitByDate", record->PositionProfitByDate)
		.set("PositionProfitByTrade", record->PositionProfitByTrade)
		.set("SettlementPrice", record->SettlementPrice)
		.set("PreSettlementPrice", record->PreSettlementPrice)
		.set("CloseVolume", record->CloseVolume)
		.set("CloseAmount", record->CloseAmount)
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and PosiDirection = :PosiDirection and OpenDate = :OpenDate and TradeID = :TradeID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("PosiDirection", (int)record->PosiDirection)
		.bind("OpenDate", record->OpenDate)
		.bind("TradeID", record->TradeID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePositionDetail Spend:%lldms", duration);
	}
}
void Mysql::SelectPositionDetail(std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_PositionDetail");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetail Spend:%lldms", duration);
	}
}
void Mysql::SelectPositionDetailWithSql(const char* sql, std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetail Spend:%lldms", duration);
	}
}
void Mysql::TruncatePositionDetail()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_PositionDetail").execute();
	WriteLog(LogLevel::Info, "TruncatePositionDetail Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateOrder()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Order(`TradingDay` char(16), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `OrderID` int, `OrderSysID` char(64), `Direction` int, `OffsetFlag` int, `OrderPriceType` int, `Price` decimal(24,8), `Volume` bigint, `VolumeTotal` bigint, `VolumeTraded` bigint, `VolumeMultiple` int, `OrderStatus` int, `OrderDate` char(16), `OrderTime` char(16), `CancelDate` char(16), `CancelTime` char(16), `SessionID` bigint, `ClientOrderID` int, `RequestID` int, `OfferID` int, `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `RebuildMark` bool, `IsForceClose` bool, UNIQUE ClientOrderID(TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateOrder Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropOrder()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Order;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropOrder Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertOrder(Order* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Order");
	
    table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "OrderID", "OrderSysID", "Direction", "OffsetFlag", "OrderPriceType", "Price", "Volume", "VolumeTotal", "VolumeTraded", "VolumeMultiple", "OrderStatus", "OrderDate", "OrderTime", "CancelDate", "CancelTime", "SessionID", "ClientOrderID", "RequestID", "OfferID", "TradeGroupID", "RiskGroupID", "CommissionGroupID", "FrozenCash", "FrozenMargin", "FrozenCommission", "RebuildMark", "IsForceClose")
		.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, record->OrderID, record->OrderSysID, (int)record->Direction, (int)record->OffsetFlag, (int)record->OrderPriceType, record->Price, record->Volume, record->VolumeTotal, record->VolumeTraded, record->VolumeMultiple, (int)record->OrderStatus, record->OrderDate, record->OrderTime, record->CancelDate, record->CancelTime, record->SessionID, record->ClientOrderID, record->RequestID, record->OfferID, record->TradeGroupID, record->RiskGroupID, record->CommissionGroupID, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->RebuildMark, record->IsForceClose)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertOrder Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertOrder(std::list<Order*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Order");
	
    auto insert = table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "OrderID", "OrderSysID", "Direction", "OffsetFlag", "OrderPriceType", "Price", "Volume", "VolumeTotal", "VolumeTraded", "VolumeMultiple", "OrderStatus", "OrderDate", "OrderTime", "CancelDate", "CancelTime", "SessionID", "ClientOrderID", "RequestID", "OfferID", "TradeGroupID", "RiskGroupID", "CommissionGroupID", "FrozenCash", "FrozenMargin", "FrozenCommission", "RebuildMark", "IsForceClose");
	for (auto record : *records)
	{
        insert.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, record->OrderID, record->OrderSysID, (int)record->Direction, (int)record->OffsetFlag, (int)record->OrderPriceType, record->Price, record->Volume, record->VolumeTotal, record->VolumeTraded, record->VolumeMultiple, (int)record->OrderStatus, record->OrderDate, record->OrderTime, record->CancelDate, record->CancelTime, record->SessionID, record->ClientOrderID, record->RequestID, record->OfferID, record->TradeGroupID, record->RiskGroupID, record->CommissionGroupID, record->FrozenCash, record->FrozenMargin, record->FrozenCommission, record->RebuildMark, record->IsForceClose);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Order");
    table.remove()
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and OrderID = :OrderID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("OrderID", record->OrderID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void Mysql::UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Order");
    table.update()
		.set("TradingDay", record->TradingDay)
		.set("AccountID", record->AccountID)
		.set("AccountType", (int)record->AccountType)
		.set("ExchangeID", record->ExchangeID)
		.set("InstrumentID", record->InstrumentID)
		.set("ProductClass", (int)record->ProductClass)
		.set("OrderID", record->OrderID)
		.set("OrderSysID", record->OrderSysID)
		.set("Direction", (int)record->Direction)
		.set("OffsetFlag", (int)record->OffsetFlag)
		.set("OrderPriceType", (int)record->OrderPriceType)
		.set("Price", record->Price)
		.set("Volume", record->Volume)
		.set("VolumeTotal", record->VolumeTotal)
		.set("VolumeTraded", record->VolumeTraded)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("OrderStatus", (int)record->OrderStatus)
		.set("OrderDate", record->OrderDate)
		.set("OrderTime", record->OrderTime)
		.set("CancelDate", record->CancelDate)
		.set("CancelTime", record->CancelTime)
		.set("SessionID", record->SessionID)
		.set("ClientOrderID", record->ClientOrderID)
		.set("RequestID", record->RequestID)
		.set("OfferID", record->OfferID)
		.set("TradeGroupID", record->TradeGroupID)
		.set("RiskGroupID", record->RiskGroupID)
		.set("CommissionGroupID", record->CommissionGroupID)
		.set("FrozenCash", record->FrozenCash)
		.set("FrozenMargin", record->FrozenMargin)
		.set("FrozenCommission", record->FrozenCommission)
		.set("RebuildMark", record->RebuildMark)
		.set("IsForceClose", record->IsForceClose)
		.where("TradingDay = :TradingDay and AccountID = :AccountID and ExchangeID = :ExchangeID and InstrumentID = :InstrumentID and OrderID = :OrderID")
		.bind("TradingDay", record->TradingDay)
		.bind("AccountID", record->AccountID)
		.bind("ExchangeID", record->ExchangeID)
		.bind("InstrumentID", record->InstrumentID)
		.bind("OrderID", record->OrderID)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void Mysql::SelectOrder(std::list<Order*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Order");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void Mysql::SelectOrderWithSql(const char* sql, std::list<Order*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void Mysql::TruncateOrder()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Order").execute();
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void Mysql::CreateTrade()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Trade(`TradingDay` char(16), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `OrderID` int, `OrderSysID` char(64), `TradeID` char(64), `Direction` int, `OffsetFlag` int, `Price` decimal(24,8), `Volume` bigint, `VolumeMultiple` int, `TradeAmount` decimal(24,8), `Commission` decimal(24,8), `TradeDate` char(16), `TradeTime` char(16), PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTrade Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::DropTrade()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS t_Trade;";
	m_Impl->m_Session.sql(sql).execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTrade Spend:%lldms, sql:%s", duration, sql);
}
void Mysql::InsertTrade(Trade* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Trade");
	
    table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "OrderID", "OrderSysID", "TradeID", "Direction", "OffsetFlag", "Price", "Volume", "VolumeMultiple", "TradeAmount", "Commission", "TradeDate", "TradeTime")
		.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, record->OrderID, record->OrderSysID, record->TradeID, (int)record->Direction, (int)record->OffsetFlag, record->Price, record->Volume, record->VolumeMultiple, record->TradeAmount, record->Commission, record->TradeDate, record->TradeTime)
        .execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTrade Spend:%lldms", duration);
	}
}
void Mysql::BatchInsertTrade(std::list<Trade*>* records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Trade");
	
    auto insert = table.insert("TradingDay", "AccountID", "AccountType", "ExchangeID", "InstrumentID", "ProductClass", "OrderID", "OrderSysID", "TradeID", "Direction", "OffsetFlag", "Price", "Volume", "VolumeMultiple", "TradeAmount", "Commission", "TradeDate", "TradeTime");
	for (auto record : *records)
	{
        insert.values(record->TradingDay, record->AccountID, (int)record->AccountType, record->ExchangeID, record->InstrumentID, (int)record->ProductClass, record->OrderID, record->OrderSysID, record->TradeID, (int)record->Direction, (int)record->OffsetFlag, record->Price, record->Volume, record->VolumeMultiple, record->TradeAmount, record->Commission, record->TradeDate, record->TradeTime);
    }
	insert.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void Mysql::DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Trade");
    table.remove()
		.where("TradingDay = :TradingDay and ExchangeID = :ExchangeID and TradeID = :TradeID and Direction = :Direction")
		.bind("TradingDay", record->TradingDay)
		.bind("ExchangeID", record->ExchangeID)
		.bind("TradeID", record->TradeID)
		.bind("Direction", (int)record->Direction)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void Mysql::UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Trade");
    table.update()
		.set("TradingDay", record->TradingDay)
		.set("AccountID", record->AccountID)
		.set("AccountType", (int)record->AccountType)
		.set("ExchangeID", record->ExchangeID)
		.set("InstrumentID", record->InstrumentID)
		.set("ProductClass", (int)record->ProductClass)
		.set("OrderID", record->OrderID)
		.set("OrderSysID", record->OrderSysID)
		.set("TradeID", record->TradeID)
		.set("Direction", (int)record->Direction)
		.set("OffsetFlag", (int)record->OffsetFlag)
		.set("Price", record->Price)
		.set("Volume", record->Volume)
		.set("VolumeMultiple", record->VolumeMultiple)
		.set("TradeAmount", record->TradeAmount)
		.set("Commission", record->Commission)
		.set("TradeDate", record->TradeDate)
		.set("TradeTime", record->TradeTime)
		.where("TradingDay = :TradingDay and ExchangeID = :ExchangeID and TradeID = :TradeID and Direction = :Direction")
		.bind("TradingDay", record->TradingDay)
		.bind("ExchangeID", record->ExchangeID)
		.bind("TradeID", record->TradeID)
		.bind("Direction", (int)record->Direction)
		.execute();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void Mysql::SelectTrade(std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	auto table = m_Impl->m_DefaultSchema.getTable("t_Trade");
    auto result = table.select("*")
		.execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void Mysql::SelectTradeWithSql(const char* sql, std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	auto result = m_Impl->m_Session.sql(sql).execute();
	while (auto row = result.fetchOne())
	{
		ParseRecord(row, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void Mysql::TruncateTrade()
{
	auto start = steady_clock::now();
	m_Impl->m_Session.sql("TRUNCATE TABLE t_Trade").execute();
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}

