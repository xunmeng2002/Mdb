#include "DuckDB.h"
#include "Constant.h"
#include "Logger.h"
#include "Utility.h"
#include "TimeUtility.h"
#include <string.h>
#include <cstring>

using namespace mdb;
using namespace std;
using namespace std::chrono;

DuckDB::DuckDB(const std::string& dbName)
	:m_DBName(dbName), m_DB(nullptr), m_Connection(nullptr)
{
	m_SqlBuff = new char[BuffSize];

	m_TradingDayDeleteStatement = nullptr;
	m_TradingDayUpdateStatement = nullptr;
	m_TradingDaySelectStatement = nullptr;
	m_TradingDayTruncateStatement = nullptr;

	m_ExchangeDeleteStatement = nullptr;
	m_ExchangeUpdateStatement = nullptr;
	m_ExchangeSelectStatement = nullptr;
	m_ExchangeTruncateStatement = nullptr;

	m_ProductDeleteStatement = nullptr;
	m_ProductUpdateStatement = nullptr;
	m_ProductSelectStatement = nullptr;
	m_ProductTruncateStatement = nullptr;

	m_InstrumentDeleteStatement = nullptr;
	m_InstrumentUpdateStatement = nullptr;
	m_InstrumentSelectStatement = nullptr;
	m_InstrumentTruncateStatement = nullptr;

	m_AccountDeleteStatement = nullptr;
	m_AccountUpdateStatement = nullptr;
	m_AccountSelectStatement = nullptr;
	m_AccountTruncateStatement = nullptr;

	m_PositionDeleteStatement = nullptr;
	m_PositionDeleteByAccountIndexStatement = nullptr;
	m_PositionUpdateStatement = nullptr;
	m_PositionSelectStatement = nullptr;
	m_PositionTruncateStatement = nullptr;

	m_OrderDeleteStatement = nullptr;
	m_OrderUpdateStatement = nullptr;
	m_OrderSelectStatement = nullptr;
	m_OrderTruncateStatement = nullptr;

	m_TradeDeleteStatement = nullptr;
	m_TradeUpdateStatement = nullptr;
	m_TradeSelectStatement = nullptr;
	m_TradeTruncateStatement = nullptr;

	m_MdTickDeleteStatement = nullptr;
	m_MdTickUpdateStatement = nullptr;
	m_MdTickSelectStatement = nullptr;
	m_MdTickTruncateStatement = nullptr;

}
DuckDB::~DuckDB()
{
	delete[] m_SqlBuff;
	DisConnect();
}
bool DuckDB::Connect()
{
	int result = duckdb_open(m_DBName.c_str(), &m_DB);
	if (result != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "Open DuckDB Failed.");
		return false;
	}
	if (duckdb_connect(m_DB, &m_Connection) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Error, "duckdb_connect Error.");
		return false;
	}
	return true;
}
void DuckDB::DisConnect()
{
	if (m_Connection != nullptr)
	{
		duckdb_disconnect(&m_Connection);
		m_Connection = nullptr;
	}
	if (m_DB != nullptr)
	{
		duckdb_close(&m_DB);
		m_DB = nullptr;
	}
	if (m_TradingDayDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradingDayDeleteStatement);
		m_TradingDayDeleteStatement = nullptr;
	}
	if (m_TradingDayUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradingDayUpdateStatement);
		m_TradingDayUpdateStatement = nullptr;
	}
	if (m_TradingDaySelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradingDaySelectStatement);
		m_TradingDaySelectStatement = nullptr;
	}
	if (m_TradingDayTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradingDayTruncateStatement);
		m_TradingDayTruncateStatement = nullptr;
	}
	if (m_ExchangeDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ExchangeDeleteStatement);
		m_ExchangeDeleteStatement = nullptr;
	}
	if (m_ExchangeUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ExchangeUpdateStatement);
		m_ExchangeUpdateStatement = nullptr;
	}
	if (m_ExchangeSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ExchangeSelectStatement);
		m_ExchangeSelectStatement = nullptr;
	}
	if (m_ExchangeTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ExchangeTruncateStatement);
		m_ExchangeTruncateStatement = nullptr;
	}
	if (m_ProductDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ProductDeleteStatement);
		m_ProductDeleteStatement = nullptr;
	}
	if (m_ProductUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ProductUpdateStatement);
		m_ProductUpdateStatement = nullptr;
	}
	if (m_ProductSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ProductSelectStatement);
		m_ProductSelectStatement = nullptr;
	}
	if (m_ProductTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_ProductTruncateStatement);
		m_ProductTruncateStatement = nullptr;
	}
	if (m_InstrumentDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_InstrumentDeleteStatement);
		m_InstrumentDeleteStatement = nullptr;
	}
	if (m_InstrumentUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_InstrumentUpdateStatement);
		m_InstrumentUpdateStatement = nullptr;
	}
	if (m_InstrumentSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_InstrumentSelectStatement);
		m_InstrumentSelectStatement = nullptr;
	}
	if (m_InstrumentTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_InstrumentTruncateStatement);
		m_InstrumentTruncateStatement = nullptr;
	}
	if (m_AccountDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_AccountDeleteStatement);
		m_AccountDeleteStatement = nullptr;
	}
	if (m_AccountUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_AccountUpdateStatement);
		m_AccountUpdateStatement = nullptr;
	}
	if (m_AccountSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_AccountSelectStatement);
		m_AccountSelectStatement = nullptr;
	}
	if (m_AccountTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_AccountTruncateStatement);
		m_AccountTruncateStatement = nullptr;
	}
	if (m_PositionDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_PositionDeleteStatement);
		m_PositionDeleteStatement = nullptr;
	}
	if (m_PositionDeleteByAccountIndexStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_PositionDeleteByAccountIndexStatement);
		m_PositionDeleteByAccountIndexStatement = nullptr;
	}
	if (m_PositionUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_PositionUpdateStatement);
		m_PositionUpdateStatement = nullptr;
	}
	if (m_PositionSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_PositionSelectStatement);
		m_PositionSelectStatement = nullptr;
	}
	if (m_PositionTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_PositionTruncateStatement);
		m_PositionTruncateStatement = nullptr;
	}
	if (m_OrderDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_OrderDeleteStatement);
		m_OrderDeleteStatement = nullptr;
	}
	if (m_OrderUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_OrderUpdateStatement);
		m_OrderUpdateStatement = nullptr;
	}
	if (m_OrderSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_OrderSelectStatement);
		m_OrderSelectStatement = nullptr;
	}
	if (m_OrderTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_OrderTruncateStatement);
		m_OrderTruncateStatement = nullptr;
	}
	if (m_TradeDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradeDeleteStatement);
		m_TradeDeleteStatement = nullptr;
	}
	if (m_TradeUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradeUpdateStatement);
		m_TradeUpdateStatement = nullptr;
	}
	if (m_TradeSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradeSelectStatement);
		m_TradeSelectStatement = nullptr;
	}
	if (m_TradeTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_TradeTruncateStatement);
		m_TradeTruncateStatement = nullptr;
	}
	if (m_MdTickDeleteStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_MdTickDeleteStatement);
		m_MdTickDeleteStatement = nullptr;
	}
	if (m_MdTickUpdateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_MdTickUpdateStatement);
		m_MdTickUpdateStatement = nullptr;
	}
	if (m_MdTickSelectStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_MdTickSelectStatement);
		m_MdTickSelectStatement = nullptr;
	}
	if (m_MdTickTruncateStatement != nullptr)
	{
		duckdb_destroy_prepare(&m_MdTickTruncateStatement);
		m_MdTickTruncateStatement = nullptr;
	}
}
void DuckDB::InitDB()
{
	Exec("Delete From t_TradingDay;");
	Exec("Insert Into t_TradingDay select * from Init.t_TradingDay;");
	Exec("Delete From t_Exchange;");
	Exec("Insert Into t_Exchange select * from Init.t_Exchange;");
	Exec("Delete From t_Product;");
	Exec("Insert Into t_Product select * from Init.t_Product;");
	Exec("Delete From t_Instrument;");
	Exec("Insert Into t_Instrument select * from Init.t_Instrument;");
	Exec("Delete From t_Account;");
	Exec("Insert Into t_Account select * from Init.t_Account;");
	Exec("Delete From t_Position;");
	Exec("Insert Into t_Position select * from Init.t_Position;");
	Exec("Delete From t_Order;");
	Exec("Insert Into t_Order select * from Init.t_Order;");
	Exec("Delete From t_Trade;");
	Exec("Insert Into t_Trade select * from Init.t_Trade;");
	Exec("Delete From t_MdTick;");
	Exec("Insert Into t_MdTick select * from Init.t_MdTick;");
}
void DuckDB::TruncateSessionTables()
{
	auto start = steady_clock::now();
	TruncateTradingDay();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::TruncateTables()
{
	TruncateTradingDay();
	TruncateExchange();
	TruncateProduct();
	TruncateAccount();
	TruncatePosition();
	TruncateOrder();
	TruncateTrade();
	TruncateMdTick();
}
bool DuckDB::Exec(const char* sql) const
{
	duckdb_prepared_statement stmt;
	if (duckdb_prepare(m_Connection, sql, &stmt) != DuckDBSuccess) 
	{
		WriteLog(LogLevel::Warning, "duckdb_prepare Failed While Exec Sql:%s, ErrorMsg:%s", sql, duckdb_prepare_error(stmt));
		duckdb_destroy_prepare(&stmt);
		return false;
	}
	duckdb_result result;
	auto ret = duckdb_execute_prepared(stmt, &result);
	if (ret != DuckDBSuccess) 
	{
		WriteLog(LogLevel::Warning, "duckdb_execute_prepared Failed While Exec Sql:%s, ErrorMsg:%s", sql, duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	duckdb_destroy_prepare(&stmt);
	return ret == DuckDBSuccess;
}

void DuckDB::InsertTradingDay(TradingDay* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_TradingDay", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_TradingDay Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForTradingDayRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_TradingDay", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_TradingDay Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForTradingDayRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_TradingDay where PK = ?;", &m_TradingDayDeleteStatement);
	}
	SetStatementForTradingDayPrimaryKey(m_TradingDayDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradingDayDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void DuckDB::UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_TradingDay set CurrTradingDay = ?, PreTradingDay = ? where PK = ?;", &m_TradingDayUpdateStatement);
	}
	SetStatementForTradingDayRecordUpdate(m_TradingDayUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradingDayUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void DuckDB::SelectTradingDay(std::vector<TradingDay*>& records)
{
	auto start = steady_clock::now();
	if (m_TradingDaySelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_TradingDay;", &m_TradingDaySelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradingDaySelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void DuckDB::TruncateTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_TradingDay;", &m_TradingDayTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_TradingDayTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateTradingDay failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertExchange(Exchange* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Exchange", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Exchange Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForExchangeRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertExchange(std::list<Exchange*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Exchange", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Exchange Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForExchangeRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Exchange where ExchangeID = ?;", &m_ExchangeDeleteStatement);
	}
	SetStatementForExchangePrimaryKey(m_ExchangeDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ExchangeDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void DuckDB::UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Exchange set ExchangeName = ? where ExchangeID = ?;", &m_ExchangeUpdateStatement);
	}
	SetStatementForExchangeRecordUpdate(m_ExchangeUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ExchangeUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void DuckDB::SelectExchange(std::vector<Exchange*>& records)
{
	auto start = steady_clock::now();
	if (m_ExchangeSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Exchange;", &m_ExchangeSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ExchangeSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectExchange ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void DuckDB::TruncateExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Exchange;", &m_ExchangeTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_ExchangeTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateExchange failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertProduct(Product* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Product", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Product Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForProductRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertProduct(std::list<Product*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Product", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Product Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForProductRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Product where ExchangeID = ? and ProductID = ?;", &m_ProductDeleteStatement);
	}
	SetStatementForProductPrimaryKey(m_ProductDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ProductDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void DuckDB::UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Product set ProductName = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and ProductID = ?;", &m_ProductUpdateStatement);
	}
	SetStatementForProductRecordUpdate(m_ProductUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ProductUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void DuckDB::SelectProduct(std::vector<Product*>& records)
{
	auto start = steady_clock::now();
	if (m_ProductSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Product;", &m_ProductSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_ProductSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectProduct ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void DuckDB::TruncateProduct()
{
	auto start = steady_clock::now();
	if (m_ProductTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Product;", &m_ProductTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_ProductTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateProduct failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertInstrument(Instrument* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Instrument", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Instrument Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForInstrumentRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertInstrument(std::list<Instrument*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Instrument", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Instrument Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForInstrumentRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Instrument where ExchangeID = ? and InstrumentID = ?;", &m_InstrumentDeleteStatement);
	}
	SetStatementForInstrumentPrimaryKey(m_InstrumentDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_InstrumentDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void DuckDB::UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Instrument set InstrumentName = ?, ProductID = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ?, DeliveryYear = ?, DeliveryMonth = ? where ExchangeID = ? and InstrumentID = ?;", &m_InstrumentUpdateStatement);
	}
	SetStatementForInstrumentRecordUpdate(m_InstrumentUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_InstrumentUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void DuckDB::SelectInstrument(std::vector<Instrument*>& records)
{
	auto start = steady_clock::now();
	if (m_InstrumentSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Instrument;", &m_InstrumentSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_InstrumentSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void DuckDB::TruncateInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Instrument;", &m_InstrumentTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_InstrumentTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateInstrument failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertAccount(Account* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Account", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Account Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForAccountRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertAccount(std::list<Account*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Account", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Account Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForAccountRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Account where AccountID = ?;", &m_AccountDeleteStatement);
	}
	SetStatementForAccountPrimaryKey(m_AccountDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_AccountDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void DuckDB::UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Account set TradingDay = ?, AccountName = ?, Password = ?, PreBalance = ?, Balance = ?, CloseProfitByDate = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, PremiumIn = ?, PremiumOut = ?, MarketValue = ? where AccountID = ?;", &m_AccountUpdateStatement);
	}
	SetStatementForAccountRecordUpdate(m_AccountUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_AccountUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void DuckDB::SelectAccount(std::vector<Account*>& records)
{
	auto start = steady_clock::now();
	if (m_AccountSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Account;", &m_AccountSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_AccountSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectAccount ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void DuckDB::TruncateAccount()
{
	auto start = steady_clock::now();
	if (m_AccountTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Account;", &m_AccountTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_AccountTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateAccount failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertPosition(Position* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Position", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Position Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForPositionRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertPosition(std::list<Position*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Position", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Position Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForPositionRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Position where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", &m_PositionDeleteStatement);
	}
	SetStatementForPositionPrimaryKey(m_PositionDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_PositionDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeletePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void DuckDB::DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByAccountIndexStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Position where TradingDay = ? and AccountID = ?;", &m_PositionDeleteByAccountIndexStatement);
	}
	SetStatementForPositionIndexAccount(m_PositionDeleteByAccountIndexStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_PositionDeleteByAccountIndexStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void DuckDB::UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Position set ProductClass = ?, TotalPosition = ?, TodayPosition = ?, FrozenPosition = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, PremiumIn = ?, PremiumOut = ?, MarketValue = ?, VolumeMultiple = ?, SettlementPrice = ?, PreSettlementPrice = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", &m_PositionUpdateStatement);
	}
	SetStatementForPositionRecordUpdate(m_PositionUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_PositionUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void DuckDB::SelectPosition(std::vector<Position*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Position;", &m_PositionSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_PositionSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectPosition ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void DuckDB::TruncatePosition()
{
	auto start = steady_clock::now();
	if (m_PositionTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Position;", &m_PositionTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_PositionTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncatePosition failed");
	}
	
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertOrder(Order* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Order", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Order Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForOrderRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertOrder(std::list<Order*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Order", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Order Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForOrderRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Order where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", &m_OrderDeleteStatement);
	}
	SetStatementForOrderPrimaryKey(m_OrderDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_OrderDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void DuckDB::UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Order set ClientOrderID = ?, Direction = ?, OffsetFlag = ?, OrderPriceType = ?, Price = ?, Volume = ?, VolumeRemain = ?, VolumeTraded = ?, VolumeMultiple = ?, OrderStatus = ?, OrderDate = ?, OrderTime = ?, CancelDate = ?, CancelTime = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", &m_OrderUpdateStatement);
	}
	SetStatementForOrderRecordUpdate(m_OrderUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_OrderUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void DuckDB::SelectOrder(std::vector<Order*>& records)
{
	auto start = steady_clock::now();
	if (m_OrderSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Order;", &m_OrderSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_OrderSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectOrder ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void DuckDB::TruncateOrder()
{
	auto start = steady_clock::now();
	if (m_OrderTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Order;", &m_OrderTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_OrderTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateOrder failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertTrade(Trade* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Trade", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Trade Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForTradeRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertTrade(std::list<Trade*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_Trade", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_Trade Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForTradeRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Trade where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", &m_TradeDeleteStatement);
	}
	SetStatementForTradePrimaryKey(m_TradeDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradeDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void DuckDB::UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_Trade set AccountID = ?, InstrumentID = ?, OrderID = ?, OffsetFlag = ?, Price = ?, Volume = ?, TradeAmount = ?, PremiumIn = ?, PremiumOut = ?, TradeDate = ?, TradeTime = ? where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", &m_TradeUpdateStatement);
	}
	SetStatementForTradeRecordUpdate(m_TradeUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradeUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void DuckDB::SelectTrade(std::vector<Trade*>& records)
{
	auto start = steady_clock::now();
	if (m_TradeSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_Trade;", &m_TradeSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_TradeSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectTrade ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void DuckDB::TruncateTrade()
{
	auto start = steady_clock::now();
	if (m_TradeTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_Trade;", &m_TradeTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_TradeTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateTrade failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void DuckDB::InsertMdTick(MdTick* record)
{
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_MdTick", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_MdTick Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	AppendForMdTickRecord(appender, record);
	duckdb_appender_destroy(&appender);
}
void DuckDB::BatchInsertMdTick(std::list<MdTick*>* records)
{
	auto start = steady_clock::now();
	duckdb_appender appender;
	if (duckdb_appender_create(m_Connection, nullptr, "t_MdTick", &appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "duckdb_appender_create For Table:t_MdTick Failed. ErrorMsg:%s", duckdb_appender_error(appender));
		duckdb_appender_destroy(&appender);
		return;
	}
	for (auto record : *records)
	{
		AppendForMdTickRecord(appender, record);
	}
	duckdb_appender_destroy(&appender);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertMdTick RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DuckDB::DeleteMdTick(MdTick* record)
{
	auto start = steady_clock::now();
	if (m_MdTickDeleteStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_MdTick where TradingDay = ? and ExchangeID = ? and InstrumentID = ?;", &m_MdTickDeleteStatement);
	}
	SetStatementForMdTickPrimaryKey(m_MdTickDeleteStatement, record);

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_MdTickDeleteStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "DeleteMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteMdTick Spend:%lldms", duration);
	}
}
void DuckDB::UpdateMdTick(MdTick* record)
{
	auto start = steady_clock::now();
	if (m_MdTickUpdateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "update t_MdTick set LastPrice = ?, PreSettlementPrice = ?, PreClosePrice = ?, PreOpenInterest = ?, OpenPrice = ?, HighestPrice = ?, LowestPrice = ?, Volume = ?, Turnover = ?, OpenInterest = ?, UpperLimitPrice = ?, LowerLimitPrice = ?, UpdateTime = ?, UpdateMillisec = ?, AskPrice1 = ?, AskPrice2 = ?, AskPrice3 = ?, AskPrice4 = ?, AskPrice5 = ?, AskVolume1 = ?, AskVolume2 = ?, AskVolume3 = ?, AskVolume4 = ?, AskVolume5 = ?, BidPrice1 = ?, BidPrice2 = ?, BidPrice3 = ?, BidPrice4 = ?, BidPrice5 = ?, BidVolume1 = ?, BidVolume2 = ?, BidVolume3 = ?, BidVolume4 = ?, BidVolume5 = ?, AveragePrice = ? where TradingDay = ? and ExchangeID = ? and InstrumentID = ?;", &m_MdTickUpdateStatement);
	}
	SetStatementForMdTickRecordUpdate(m_MdTickUpdateStatement, record);
	
	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_MdTickUpdateStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "UpdateMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_result_error(&result));
	}
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateMdTick Spend:%lldms", duration);
	}
}
void DuckDB::SelectMdTick(std::vector<MdTick*>& records)
{
	auto start = steady_clock::now();
	if (m_MdTickSelectStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "select * from t_MdTick;", &m_MdTickSelectStatement);
	}

	duckdb_result result;
	auto rc = duckdb_execute_prepared(m_MdTickSelectStatement, &result);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "SelectMdTick ErrorMsg:%s", duckdb_result_error(&result));
		duckdb_destroy_result(&result);
		return;
	}

	ParseRecord(result, records);
	duckdb_destroy_result(&result);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectMdTick Spend:%lldms", duration);
	}
}
void DuckDB::TruncateMdTick()
{
	auto start = steady_clock::now();
	if (m_MdTickTruncateStatement == nullptr)
	{
		duckdb_prepare(m_Connection, "delete from t_MdTick;", &m_MdTickTruncateStatement);
	}

	auto rc = duckdb_execute_prepared(m_MdTickTruncateStatement, nullptr);
	if (rc != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "TruncateMdTick failed");
	}
	
	WriteLog(LogLevel::Info, "TruncateMdTick Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}


bool DuckDB::AppendForTradingDayRecord(duckdb_appender appender, TradingDay* record)
{
	duckdb_append_int32(appender, record->PK);
	duckdb_append_varchar(appender, record->CurrTradingDay);
	duckdb_append_varchar(appender, record->PreTradingDay);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForTradingDayRecord(duckdb_prepared_statement statement, TradingDay* record)
{
	duckdb_bind_int32(statement, 1, record->PK);
	duckdb_bind_varchar(statement, 2, record->CurrTradingDay);
	duckdb_bind_varchar(statement, 3, record->PreTradingDay);
}
void DuckDB::SetStatementForTradingDayRecordUpdate(duckdb_prepared_statement statement, TradingDay* record)
{
	duckdb_bind_varchar(statement, 1, record->CurrTradingDay);
	duckdb_bind_varchar(statement, 2, record->PreTradingDay);
	duckdb_bind_int32(statement, 3, record->PK);
}
void DuckDB::SetStatementForTradingDayPrimaryKey(duckdb_prepared_statement statement, TradingDay* record)
{
	duckdb_bind_int32(statement, 1, record->PK);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<TradingDay*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);

		int* dataColumn0 = (int*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			TradingDay* record = TradingDay::Allocate();
			memset(record, 0, sizeof(TradingDay));
			if (duckdb_validity_row_is_valid(validityColumn0, row)) record->PK = dataColumn0[row];
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->CurrTradingDay, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->CurrTradingDay, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->PreTradingDay, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->PreTradingDay, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForExchangeRecord(duckdb_appender appender, Exchange* record)
{
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->ExchangeName);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForExchangeRecord(duckdb_prepared_statement statement, Exchange* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
	duckdb_bind_varchar(statement, 2, record->ExchangeName);
}
void DuckDB::SetStatementForExchangeRecordUpdate(duckdb_prepared_statement statement, Exchange* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeName);
	duckdb_bind_varchar(statement, 2, record->ExchangeID);
}
void DuckDB::SetStatementForExchangePrimaryKey(duckdb_prepared_statement statement, Exchange* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Exchange*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Exchange* record = Exchange::Allocate();
			memset(record, 0, sizeof(Exchange));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->ExchangeID, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->ExchangeName, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeName, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForProductRecord(duckdb_appender appender, Product* record)
{
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->ProductID);
	duckdb_append_varchar(appender, record->ProductName);
	duckdb_append_int32(appender, int(record->ProductClass));
	duckdb_append_int32(appender, record->VolumeMultiple);
	duckdb_append_double(appender, record->PriceTick);
	duckdb_append_int32(appender, record->MaxMarketOrderVolume);
	duckdb_append_int32(appender, record->MinMarketOrderVolume);
	duckdb_append_int32(appender, record->MaxLimitOrderVolume);
	duckdb_append_int32(appender, record->MinLimitOrderVolume);
	duckdb_append_varchar(appender, record->SessionName);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForProductRecord(duckdb_prepared_statement statement, Product* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
	duckdb_bind_varchar(statement, 2, record->ProductID);
	duckdb_bind_varchar(statement, 3, record->ProductName);
	duckdb_bind_int32(statement, 4, int(record->ProductClass));
	duckdb_bind_int32(statement, 5, record->VolumeMultiple);
	duckdb_bind_double(statement, 6, record->PriceTick);
	duckdb_bind_int32(statement, 7, record->MaxMarketOrderVolume);
	duckdb_bind_int32(statement, 8, record->MinMarketOrderVolume);
	duckdb_bind_int32(statement, 9, record->MaxLimitOrderVolume);
	duckdb_bind_int32(statement, 10, record->MinLimitOrderVolume);
	duckdb_bind_varchar(statement, 11, record->SessionName);
}
void DuckDB::SetStatementForProductRecordUpdate(duckdb_prepared_statement statement, Product* record)
{
	duckdb_bind_varchar(statement, 1, record->ProductName);
	duckdb_bind_int32(statement, 2, int(record->ProductClass));
	duckdb_bind_int32(statement, 3, record->VolumeMultiple);
	duckdb_bind_double(statement, 4, record->PriceTick);
	duckdb_bind_int32(statement, 5, record->MaxMarketOrderVolume);
	duckdb_bind_int32(statement, 6, record->MinMarketOrderVolume);
	duckdb_bind_int32(statement, 7, record->MaxLimitOrderVolume);
	duckdb_bind_int32(statement, 8, record->MinLimitOrderVolume);
	duckdb_bind_varchar(statement, 9, record->SessionName);
	duckdb_bind_varchar(statement, 10, record->ExchangeID);
	duckdb_bind_varchar(statement, 11, record->ProductID);
}
void DuckDB::SetStatementForProductPrimaryKey(duckdb_prepared_statement statement, Product* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
	duckdb_bind_varchar(statement, 2, record->ProductID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Product*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		int* dataColumn3 = (int*)duckdb_vector_get_data(column3);
		int* dataColumn4 = (int*)duckdb_vector_get_data(column4);
		double* dataColumn5 = (double*)duckdb_vector_get_data(column5);
		int* dataColumn6 = (int*)duckdb_vector_get_data(column6);
		int* dataColumn7 = (int*)duckdb_vector_get_data(column7);
		int* dataColumn8 = (int*)duckdb_vector_get_data(column8);
		int* dataColumn9 = (int*)duckdb_vector_get_data(column9);
		duckdb_string_t* dataColumn10 = (duckdb_string_t*)duckdb_vector_get_data(column10);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Product* record = Product::Allocate();
			memset(record, 0, sizeof(Product));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->ExchangeID, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->ProductID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ProductID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->ProductName, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ProductName, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row)) record->ProductClass = ProductClassType(dataColumn3[row]);
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->VolumeMultiple = dataColumn4[row];
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->PriceTick = dataColumn5[row];
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->MaxMarketOrderVolume = dataColumn6[row];
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->MinMarketOrderVolume = dataColumn7[row];
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->MaxLimitOrderVolume = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->MinLimitOrderVolume = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row))
			{
				if (duckdb_string_is_inlined(dataColumn10[row]))
				{
					Strcpy(record->SessionName, dataColumn10[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->SessionName, dataColumn10[row].value.pointer.ptr, dataColumn10[row].value.pointer.length);
				}
			}
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForInstrumentRecord(duckdb_appender appender, Instrument* record)
{
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->InstrumentID);
	duckdb_append_varchar(appender, record->InstrumentName);
	duckdb_append_varchar(appender, record->ProductID);
	duckdb_append_int32(appender, int(record->ProductClass));
	duckdb_append_int32(appender, record->VolumeMultiple);
	duckdb_append_double(appender, record->PriceTick);
	duckdb_append_int32(appender, record->MaxMarketOrderVolume);
	duckdb_append_int32(appender, record->MinMarketOrderVolume);
	duckdb_append_int32(appender, record->MaxLimitOrderVolume);
	duckdb_append_int32(appender, record->MinLimitOrderVolume);
	duckdb_append_varchar(appender, record->SessionName);
	duckdb_append_int32(appender, record->DeliveryYear);
	duckdb_append_int32(appender, record->DeliveryMonth);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForInstrumentRecord(duckdb_prepared_statement statement, Instrument* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
	duckdb_bind_varchar(statement, 2, record->InstrumentID);
	duckdb_bind_varchar(statement, 3, record->InstrumentName);
	duckdb_bind_varchar(statement, 4, record->ProductID);
	duckdb_bind_int32(statement, 5, int(record->ProductClass));
	duckdb_bind_int32(statement, 6, record->VolumeMultiple);
	duckdb_bind_double(statement, 7, record->PriceTick);
	duckdb_bind_int32(statement, 8, record->MaxMarketOrderVolume);
	duckdb_bind_int32(statement, 9, record->MinMarketOrderVolume);
	duckdb_bind_int32(statement, 10, record->MaxLimitOrderVolume);
	duckdb_bind_int32(statement, 11, record->MinLimitOrderVolume);
	duckdb_bind_varchar(statement, 12, record->SessionName);
	duckdb_bind_int32(statement, 13, record->DeliveryYear);
	duckdb_bind_int32(statement, 14, record->DeliveryMonth);
}
void DuckDB::SetStatementForInstrumentRecordUpdate(duckdb_prepared_statement statement, Instrument* record)
{
	duckdb_bind_varchar(statement, 1, record->InstrumentName);
	duckdb_bind_varchar(statement, 2, record->ProductID);
	duckdb_bind_int32(statement, 3, int(record->ProductClass));
	duckdb_bind_int32(statement, 4, record->VolumeMultiple);
	duckdb_bind_double(statement, 5, record->PriceTick);
	duckdb_bind_int32(statement, 6, record->MaxMarketOrderVolume);
	duckdb_bind_int32(statement, 7, record->MinMarketOrderVolume);
	duckdb_bind_int32(statement, 8, record->MaxLimitOrderVolume);
	duckdb_bind_int32(statement, 9, record->MinLimitOrderVolume);
	duckdb_bind_varchar(statement, 10, record->SessionName);
	duckdb_bind_int32(statement, 11, record->DeliveryYear);
	duckdb_bind_int32(statement, 12, record->DeliveryMonth);
	duckdb_bind_varchar(statement, 13, record->ExchangeID);
	duckdb_bind_varchar(statement, 14, record->InstrumentID);
}
void DuckDB::SetStatementForInstrumentPrimaryKey(duckdb_prepared_statement statement, Instrument* record)
{
	duckdb_bind_varchar(statement, 1, record->ExchangeID);
	duckdb_bind_varchar(statement, 2, record->InstrumentID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Instrument*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);
		duckdb_vector column12 = duckdb_data_chunk_get_vector(dataChunk, 12);
		duckdb_vector column13 = duckdb_data_chunk_get_vector(dataChunk, 13);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		duckdb_string_t* dataColumn3 = (duckdb_string_t*)duckdb_vector_get_data(column3);
		int* dataColumn4 = (int*)duckdb_vector_get_data(column4);
		int* dataColumn5 = (int*)duckdb_vector_get_data(column5);
		double* dataColumn6 = (double*)duckdb_vector_get_data(column6);
		int* dataColumn7 = (int*)duckdb_vector_get_data(column7);
		int* dataColumn8 = (int*)duckdb_vector_get_data(column8);
		int* dataColumn9 = (int*)duckdb_vector_get_data(column9);
		int* dataColumn10 = (int*)duckdb_vector_get_data(column10);
		duckdb_string_t* dataColumn11 = (duckdb_string_t*)duckdb_vector_get_data(column11);
		int* dataColumn12 = (int*)duckdb_vector_get_data(column12);
		int* dataColumn13 = (int*)duckdb_vector_get_data(column13);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);
		uint64_t* validityColumn12 = duckdb_vector_get_validity(column12);
		uint64_t* validityColumn13 = duckdb_vector_get_validity(column13);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Instrument* record = Instrument::Allocate();
			memset(record, 0, sizeof(Instrument));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->ExchangeID, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->InstrumentID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->InstrumentName, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentName, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row))
			{
				if (duckdb_string_is_inlined(dataColumn3[row]))
				{
					Strcpy(record->ProductID, dataColumn3[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ProductID, dataColumn3[row].value.pointer.ptr, dataColumn3[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->ProductClass = ProductClassType(dataColumn4[row]);
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->VolumeMultiple = dataColumn5[row];
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->PriceTick = dataColumn6[row];
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->MaxMarketOrderVolume = dataColumn7[row];
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->MinMarketOrderVolume = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->MaxLimitOrderVolume = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->MinLimitOrderVolume = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row))
			{
				if (duckdb_string_is_inlined(dataColumn11[row]))
				{
					Strcpy(record->SessionName, dataColumn11[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->SessionName, dataColumn11[row].value.pointer.ptr, dataColumn11[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn12, row)) record->DeliveryYear = dataColumn12[row];
			if (duckdb_validity_row_is_valid(validityColumn13, row)) record->DeliveryMonth = dataColumn13[row];
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForAccountRecord(duckdb_appender appender, Account* record)
{
	duckdb_append_varchar(appender, record->TradingDay);
	duckdb_append_varchar(appender, record->AccountID);
	duckdb_append_varchar(appender, record->AccountName);
	duckdb_append_varchar(appender, record->Password);
	duckdb_append_double(appender, record->PreBalance);
	duckdb_append_double(appender, record->Balance);
	duckdb_append_double(appender, record->CloseProfitByDate);
	duckdb_append_double(appender, record->PositionProfitByDate);
	duckdb_append_double(appender, record->PositionProfitByTrade);
	duckdb_append_double(appender, record->PremiumIn);
	duckdb_append_double(appender, record->PremiumOut);
	duckdb_append_double(appender, record->MarketValue);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForAccountRecord(duckdb_prepared_statement statement, Account* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->AccountName);
	duckdb_bind_varchar(statement, 4, record->Password);
	duckdb_bind_double(statement, 5, record->PreBalance);
	duckdb_bind_double(statement, 6, record->Balance);
	duckdb_bind_double(statement, 7, record->CloseProfitByDate);
	duckdb_bind_double(statement, 8, record->PositionProfitByDate);
	duckdb_bind_double(statement, 9, record->PositionProfitByTrade);
	duckdb_bind_double(statement, 10, record->PremiumIn);
	duckdb_bind_double(statement, 11, record->PremiumOut);
	duckdb_bind_double(statement, 12, record->MarketValue);
}
void DuckDB::SetStatementForAccountRecordUpdate(duckdb_prepared_statement statement, Account* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountName);
	duckdb_bind_varchar(statement, 3, record->Password);
	duckdb_bind_double(statement, 4, record->PreBalance);
	duckdb_bind_double(statement, 5, record->Balance);
	duckdb_bind_double(statement, 6, record->CloseProfitByDate);
	duckdb_bind_double(statement, 7, record->PositionProfitByDate);
	duckdb_bind_double(statement, 8, record->PositionProfitByTrade);
	duckdb_bind_double(statement, 9, record->PremiumIn);
	duckdb_bind_double(statement, 10, record->PremiumOut);
	duckdb_bind_double(statement, 11, record->MarketValue);
	duckdb_bind_varchar(statement, 12, record->AccountID);
}
void DuckDB::SetStatementForAccountPrimaryKey(duckdb_prepared_statement statement, Account* record)
{
	duckdb_bind_varchar(statement, 1, record->AccountID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Account*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		duckdb_string_t* dataColumn3 = (duckdb_string_t*)duckdb_vector_get_data(column3);
		double* dataColumn4 = (double*)duckdb_vector_get_data(column4);
		double* dataColumn5 = (double*)duckdb_vector_get_data(column5);
		double* dataColumn6 = (double*)duckdb_vector_get_data(column6);
		double* dataColumn7 = (double*)duckdb_vector_get_data(column7);
		double* dataColumn8 = (double*)duckdb_vector_get_data(column8);
		double* dataColumn9 = (double*)duckdb_vector_get_data(column9);
		double* dataColumn10 = (double*)duckdb_vector_get_data(column10);
		double* dataColumn11 = (double*)duckdb_vector_get_data(column11);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Account* record = Account::Allocate();
			memset(record, 0, sizeof(Account));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->TradingDay, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradingDay, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->AccountID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->AccountID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->AccountName, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->AccountName, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row))
			{
				if (duckdb_string_is_inlined(dataColumn3[row]))
				{
					Strcpy(record->Password, dataColumn3[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->Password, dataColumn3[row].value.pointer.ptr, dataColumn3[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->PreBalance = dataColumn4[row];
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->Balance = dataColumn5[row];
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->CloseProfitByDate = dataColumn6[row];
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->PositionProfitByDate = dataColumn7[row];
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->PositionProfitByTrade = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->PremiumIn = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->PremiumOut = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row)) record->MarketValue = dataColumn11[row];
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForPositionRecord(duckdb_appender appender, Position* record)
{
	duckdb_append_varchar(appender, record->TradingDay);
	duckdb_append_varchar(appender, record->AccountID);
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->InstrumentID);
	duckdb_append_int32(appender, int(record->ProductClass));
	duckdb_append_int32(appender, int(record->PosiDirection));
	duckdb_append_int32(appender, record->TotalPosition);
	duckdb_append_int32(appender, record->TodayPosition);
	duckdb_append_int32(appender, record->FrozenPosition);
	duckdb_append_double(appender, record->CloseProfitByDate);
	duckdb_append_double(appender, record->CloseProfitByTrade);
	duckdb_append_double(appender, record->PositionProfitByDate);
	duckdb_append_double(appender, record->PositionProfitByTrade);
	duckdb_append_double(appender, record->PremiumIn);
	duckdb_append_double(appender, record->PremiumOut);
	duckdb_append_double(appender, record->MarketValue);
	duckdb_append_int32(appender, record->VolumeMultiple);
	duckdb_append_double(appender, record->SettlementPrice);
	duckdb_append_double(appender, record->PreSettlementPrice);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertPosition failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForPositionRecord(duckdb_prepared_statement statement, Position* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->ExchangeID);
	duckdb_bind_varchar(statement, 4, record->InstrumentID);
	duckdb_bind_int32(statement, 5, int(record->ProductClass));
	duckdb_bind_int32(statement, 6, int(record->PosiDirection));
	duckdb_bind_int32(statement, 7, record->TotalPosition);
	duckdb_bind_int32(statement, 8, record->TodayPosition);
	duckdb_bind_int32(statement, 9, record->FrozenPosition);
	duckdb_bind_double(statement, 10, record->CloseProfitByDate);
	duckdb_bind_double(statement, 11, record->CloseProfitByTrade);
	duckdb_bind_double(statement, 12, record->PositionProfitByDate);
	duckdb_bind_double(statement, 13, record->PositionProfitByTrade);
	duckdb_bind_double(statement, 14, record->PremiumIn);
	duckdb_bind_double(statement, 15, record->PremiumOut);
	duckdb_bind_double(statement, 16, record->MarketValue);
	duckdb_bind_int32(statement, 17, record->VolumeMultiple);
	duckdb_bind_double(statement, 18, record->SettlementPrice);
	duckdb_bind_double(statement, 19, record->PreSettlementPrice);
}
void DuckDB::SetStatementForPositionRecordUpdate(duckdb_prepared_statement statement, Position* record)
{
	duckdb_bind_int32(statement, 1, int(record->ProductClass));
	duckdb_bind_int32(statement, 2, record->TotalPosition);
	duckdb_bind_int32(statement, 3, record->TodayPosition);
	duckdb_bind_int32(statement, 4, record->FrozenPosition);
	duckdb_bind_double(statement, 5, record->CloseProfitByDate);
	duckdb_bind_double(statement, 6, record->CloseProfitByTrade);
	duckdb_bind_double(statement, 7, record->PositionProfitByDate);
	duckdb_bind_double(statement, 8, record->PositionProfitByTrade);
	duckdb_bind_double(statement, 9, record->PremiumIn);
	duckdb_bind_double(statement, 10, record->PremiumOut);
	duckdb_bind_double(statement, 11, record->MarketValue);
	duckdb_bind_int32(statement, 12, record->VolumeMultiple);
	duckdb_bind_double(statement, 13, record->SettlementPrice);
	duckdb_bind_double(statement, 14, record->PreSettlementPrice);
	duckdb_bind_varchar(statement, 15, record->TradingDay);
	duckdb_bind_varchar(statement, 16, record->AccountID);
	duckdb_bind_varchar(statement, 17, record->ExchangeID);
	duckdb_bind_varchar(statement, 18, record->InstrumentID);
	duckdb_bind_int32(statement, 19, int(record->PosiDirection));
}
void DuckDB::SetStatementForPositionPrimaryKey(duckdb_prepared_statement statement, Position* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->ExchangeID);
	duckdb_bind_varchar(statement, 4, record->InstrumentID);
	duckdb_bind_int32(statement, 5, int(record->PosiDirection));
}
void DuckDB::SetStatementForPositionIndexAccount(duckdb_prepared_statement statement, Position* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Position*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);
		duckdb_vector column12 = duckdb_data_chunk_get_vector(dataChunk, 12);
		duckdb_vector column13 = duckdb_data_chunk_get_vector(dataChunk, 13);
		duckdb_vector column14 = duckdb_data_chunk_get_vector(dataChunk, 14);
		duckdb_vector column15 = duckdb_data_chunk_get_vector(dataChunk, 15);
		duckdb_vector column16 = duckdb_data_chunk_get_vector(dataChunk, 16);
		duckdb_vector column17 = duckdb_data_chunk_get_vector(dataChunk, 17);
		duckdb_vector column18 = duckdb_data_chunk_get_vector(dataChunk, 18);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		duckdb_string_t* dataColumn3 = (duckdb_string_t*)duckdb_vector_get_data(column3);
		int* dataColumn4 = (int*)duckdb_vector_get_data(column4);
		int* dataColumn5 = (int*)duckdb_vector_get_data(column5);
		int* dataColumn6 = (int*)duckdb_vector_get_data(column6);
		int* dataColumn7 = (int*)duckdb_vector_get_data(column7);
		int* dataColumn8 = (int*)duckdb_vector_get_data(column8);
		double* dataColumn9 = (double*)duckdb_vector_get_data(column9);
		double* dataColumn10 = (double*)duckdb_vector_get_data(column10);
		double* dataColumn11 = (double*)duckdb_vector_get_data(column11);
		double* dataColumn12 = (double*)duckdb_vector_get_data(column12);
		double* dataColumn13 = (double*)duckdb_vector_get_data(column13);
		double* dataColumn14 = (double*)duckdb_vector_get_data(column14);
		double* dataColumn15 = (double*)duckdb_vector_get_data(column15);
		int* dataColumn16 = (int*)duckdb_vector_get_data(column16);
		double* dataColumn17 = (double*)duckdb_vector_get_data(column17);
		double* dataColumn18 = (double*)duckdb_vector_get_data(column18);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);
		uint64_t* validityColumn12 = duckdb_vector_get_validity(column12);
		uint64_t* validityColumn13 = duckdb_vector_get_validity(column13);
		uint64_t* validityColumn14 = duckdb_vector_get_validity(column14);
		uint64_t* validityColumn15 = duckdb_vector_get_validity(column15);
		uint64_t* validityColumn16 = duckdb_vector_get_validity(column16);
		uint64_t* validityColumn17 = duckdb_vector_get_validity(column17);
		uint64_t* validityColumn18 = duckdb_vector_get_validity(column18);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Position* record = Position::Allocate();
			memset(record, 0, sizeof(Position));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->TradingDay, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradingDay, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->AccountID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->AccountID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->ExchangeID, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row))
			{
				if (duckdb_string_is_inlined(dataColumn3[row]))
				{
					Strcpy(record->InstrumentID, dataColumn3[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentID, dataColumn3[row].value.pointer.ptr, dataColumn3[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->ProductClass = ProductClassType(dataColumn4[row]);
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->PosiDirection = PosiDirectionType(dataColumn5[row]);
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->TotalPosition = dataColumn6[row];
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->TodayPosition = dataColumn7[row];
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->FrozenPosition = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->CloseProfitByDate = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->CloseProfitByTrade = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row)) record->PositionProfitByDate = dataColumn11[row];
			if (duckdb_validity_row_is_valid(validityColumn12, row)) record->PositionProfitByTrade = dataColumn12[row];
			if (duckdb_validity_row_is_valid(validityColumn13, row)) record->PremiumIn = dataColumn13[row];
			if (duckdb_validity_row_is_valid(validityColumn14, row)) record->PremiumOut = dataColumn14[row];
			if (duckdb_validity_row_is_valid(validityColumn15, row)) record->MarketValue = dataColumn15[row];
			if (duckdb_validity_row_is_valid(validityColumn16, row)) record->VolumeMultiple = dataColumn16[row];
			if (duckdb_validity_row_is_valid(validityColumn17, row)) record->SettlementPrice = dataColumn17[row];
			if (duckdb_validity_row_is_valid(validityColumn18, row)) record->PreSettlementPrice = dataColumn18[row];
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForOrderRecord(duckdb_appender appender, Order* record)
{
	duckdb_append_varchar(appender, record->TradingDay);
	duckdb_append_varchar(appender, record->AccountID);
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->InstrumentID);
	duckdb_append_int32(appender, record->OrderID);
	duckdb_append_int32(appender, record->ClientOrderID);
	duckdb_append_int32(appender, int(record->Direction));
	duckdb_append_int32(appender, int(record->OffsetFlag));
	duckdb_append_int32(appender, int(record->OrderPriceType));
	duckdb_append_double(appender, record->Price);
	duckdb_append_int32(appender, record->Volume);
	duckdb_append_int32(appender, record->VolumeRemain);
	duckdb_append_int32(appender, record->VolumeTraded);
	duckdb_append_int32(appender, record->VolumeMultiple);
	duckdb_append_int32(appender, int(record->OrderStatus));
	duckdb_append_varchar(appender, record->OrderDate);
	duckdb_append_varchar(appender, record->OrderTime);
	duckdb_append_varchar(appender, record->CancelDate);
	duckdb_append_varchar(appender, record->CancelTime);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForOrderRecord(duckdb_prepared_statement statement, Order* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->ExchangeID);
	duckdb_bind_varchar(statement, 4, record->InstrumentID);
	duckdb_bind_int32(statement, 5, record->OrderID);
	duckdb_bind_int32(statement, 6, record->ClientOrderID);
	duckdb_bind_int32(statement, 7, int(record->Direction));
	duckdb_bind_int32(statement, 8, int(record->OffsetFlag));
	duckdb_bind_int32(statement, 9, int(record->OrderPriceType));
	duckdb_bind_double(statement, 10, record->Price);
	duckdb_bind_int32(statement, 11, record->Volume);
	duckdb_bind_int32(statement, 12, record->VolumeRemain);
	duckdb_bind_int32(statement, 13, record->VolumeTraded);
	duckdb_bind_int32(statement, 14, record->VolumeMultiple);
	duckdb_bind_int32(statement, 15, int(record->OrderStatus));
	duckdb_bind_varchar(statement, 16, record->OrderDate);
	duckdb_bind_varchar(statement, 17, record->OrderTime);
	duckdb_bind_varchar(statement, 18, record->CancelDate);
	duckdb_bind_varchar(statement, 19, record->CancelTime);
}
void DuckDB::SetStatementForOrderRecordUpdate(duckdb_prepared_statement statement, Order* record)
{
	duckdb_bind_int32(statement, 1, record->ClientOrderID);
	duckdb_bind_int32(statement, 2, int(record->Direction));
	duckdb_bind_int32(statement, 3, int(record->OffsetFlag));
	duckdb_bind_int32(statement, 4, int(record->OrderPriceType));
	duckdb_bind_double(statement, 5, record->Price);
	duckdb_bind_int32(statement, 6, record->Volume);
	duckdb_bind_int32(statement, 7, record->VolumeRemain);
	duckdb_bind_int32(statement, 8, record->VolumeTraded);
	duckdb_bind_int32(statement, 9, record->VolumeMultiple);
	duckdb_bind_int32(statement, 10, int(record->OrderStatus));
	duckdb_bind_varchar(statement, 11, record->OrderDate);
	duckdb_bind_varchar(statement, 12, record->OrderTime);
	duckdb_bind_varchar(statement, 13, record->CancelDate);
	duckdb_bind_varchar(statement, 14, record->CancelTime);
	duckdb_bind_varchar(statement, 15, record->TradingDay);
	duckdb_bind_varchar(statement, 16, record->AccountID);
	duckdb_bind_varchar(statement, 17, record->ExchangeID);
	duckdb_bind_varchar(statement, 18, record->InstrumentID);
	duckdb_bind_int32(statement, 19, record->OrderID);
}
void DuckDB::SetStatementForOrderPrimaryKey(duckdb_prepared_statement statement, Order* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->ExchangeID);
	duckdb_bind_varchar(statement, 4, record->InstrumentID);
	duckdb_bind_int32(statement, 5, record->OrderID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Order*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);
		duckdb_vector column12 = duckdb_data_chunk_get_vector(dataChunk, 12);
		duckdb_vector column13 = duckdb_data_chunk_get_vector(dataChunk, 13);
		duckdb_vector column14 = duckdb_data_chunk_get_vector(dataChunk, 14);
		duckdb_vector column15 = duckdb_data_chunk_get_vector(dataChunk, 15);
		duckdb_vector column16 = duckdb_data_chunk_get_vector(dataChunk, 16);
		duckdb_vector column17 = duckdb_data_chunk_get_vector(dataChunk, 17);
		duckdb_vector column18 = duckdb_data_chunk_get_vector(dataChunk, 18);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		duckdb_string_t* dataColumn3 = (duckdb_string_t*)duckdb_vector_get_data(column3);
		int* dataColumn4 = (int*)duckdb_vector_get_data(column4);
		int* dataColumn5 = (int*)duckdb_vector_get_data(column5);
		int* dataColumn6 = (int*)duckdb_vector_get_data(column6);
		int* dataColumn7 = (int*)duckdb_vector_get_data(column7);
		int* dataColumn8 = (int*)duckdb_vector_get_data(column8);
		double* dataColumn9 = (double*)duckdb_vector_get_data(column9);
		int* dataColumn10 = (int*)duckdb_vector_get_data(column10);
		int* dataColumn11 = (int*)duckdb_vector_get_data(column11);
		int* dataColumn12 = (int*)duckdb_vector_get_data(column12);
		int* dataColumn13 = (int*)duckdb_vector_get_data(column13);
		int* dataColumn14 = (int*)duckdb_vector_get_data(column14);
		duckdb_string_t* dataColumn15 = (duckdb_string_t*)duckdb_vector_get_data(column15);
		duckdb_string_t* dataColumn16 = (duckdb_string_t*)duckdb_vector_get_data(column16);
		duckdb_string_t* dataColumn17 = (duckdb_string_t*)duckdb_vector_get_data(column17);
		duckdb_string_t* dataColumn18 = (duckdb_string_t*)duckdb_vector_get_data(column18);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);
		uint64_t* validityColumn12 = duckdb_vector_get_validity(column12);
		uint64_t* validityColumn13 = duckdb_vector_get_validity(column13);
		uint64_t* validityColumn14 = duckdb_vector_get_validity(column14);
		uint64_t* validityColumn15 = duckdb_vector_get_validity(column15);
		uint64_t* validityColumn16 = duckdb_vector_get_validity(column16);
		uint64_t* validityColumn17 = duckdb_vector_get_validity(column17);
		uint64_t* validityColumn18 = duckdb_vector_get_validity(column18);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Order* record = Order::Allocate();
			memset(record, 0, sizeof(Order));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->TradingDay, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradingDay, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->AccountID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->AccountID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->ExchangeID, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row))
			{
				if (duckdb_string_is_inlined(dataColumn3[row]))
				{
					Strcpy(record->InstrumentID, dataColumn3[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentID, dataColumn3[row].value.pointer.ptr, dataColumn3[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->OrderID = dataColumn4[row];
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->ClientOrderID = dataColumn5[row];
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->Direction = DirectionType(dataColumn6[row]);
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->OffsetFlag = OffsetFlagType(dataColumn7[row]);
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->OrderPriceType = OrderPriceTypeType(dataColumn8[row]);
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->Price = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->Volume = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row)) record->VolumeRemain = dataColumn11[row];
			if (duckdb_validity_row_is_valid(validityColumn12, row)) record->VolumeTraded = dataColumn12[row];
			if (duckdb_validity_row_is_valid(validityColumn13, row)) record->VolumeMultiple = dataColumn13[row];
			if (duckdb_validity_row_is_valid(validityColumn14, row)) record->OrderStatus = OrderStatusType(dataColumn14[row]);
			if (duckdb_validity_row_is_valid(validityColumn15, row))
			{
				if (duckdb_string_is_inlined(dataColumn15[row]))
				{
					Strcpy(record->OrderDate, dataColumn15[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->OrderDate, dataColumn15[row].value.pointer.ptr, dataColumn15[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn16, row))
			{
				if (duckdb_string_is_inlined(dataColumn16[row]))
				{
					Strcpy(record->OrderTime, dataColumn16[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->OrderTime, dataColumn16[row].value.pointer.ptr, dataColumn16[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn17, row))
			{
				if (duckdb_string_is_inlined(dataColumn17[row]))
				{
					Strcpy(record->CancelDate, dataColumn17[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->CancelDate, dataColumn17[row].value.pointer.ptr, dataColumn17[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn18, row))
			{
				if (duckdb_string_is_inlined(dataColumn18[row]))
				{
					Strcpy(record->CancelTime, dataColumn18[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->CancelTime, dataColumn18[row].value.pointer.ptr, dataColumn18[row].value.pointer.length);
				}
			}
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForTradeRecord(duckdb_appender appender, Trade* record)
{
	duckdb_append_varchar(appender, record->TradingDay);
	duckdb_append_varchar(appender, record->AccountID);
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->InstrumentID);
	duckdb_append_int32(appender, record->OrderID);
	duckdb_append_varchar(appender, record->TradeID);
	duckdb_append_int32(appender, int(record->Direction));
	duckdb_append_int32(appender, int(record->OffsetFlag));
	duckdb_append_double(appender, record->Price);
	duckdb_append_int32(appender, record->Volume);
	duckdb_append_double(appender, record->TradeAmount);
	duckdb_append_double(appender, record->PremiumIn);
	duckdb_append_double(appender, record->PremiumOut);
	duckdb_append_varchar(appender, record->TradeDate);
	duckdb_append_varchar(appender, record->TradeTime);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForTradeRecord(duckdb_prepared_statement statement, Trade* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->AccountID);
	duckdb_bind_varchar(statement, 3, record->ExchangeID);
	duckdb_bind_varchar(statement, 4, record->InstrumentID);
	duckdb_bind_int32(statement, 5, record->OrderID);
	duckdb_bind_varchar(statement, 6, record->TradeID);
	duckdb_bind_int32(statement, 7, int(record->Direction));
	duckdb_bind_int32(statement, 8, int(record->OffsetFlag));
	duckdb_bind_double(statement, 9, record->Price);
	duckdb_bind_int32(statement, 10, record->Volume);
	duckdb_bind_double(statement, 11, record->TradeAmount);
	duckdb_bind_double(statement, 12, record->PremiumIn);
	duckdb_bind_double(statement, 13, record->PremiumOut);
	duckdb_bind_varchar(statement, 14, record->TradeDate);
	duckdb_bind_varchar(statement, 15, record->TradeTime);
}
void DuckDB::SetStatementForTradeRecordUpdate(duckdb_prepared_statement statement, Trade* record)
{
	duckdb_bind_varchar(statement, 1, record->AccountID);
	duckdb_bind_varchar(statement, 2, record->InstrumentID);
	duckdb_bind_int32(statement, 3, record->OrderID);
	duckdb_bind_int32(statement, 4, int(record->OffsetFlag));
	duckdb_bind_double(statement, 5, record->Price);
	duckdb_bind_int32(statement, 6, record->Volume);
	duckdb_bind_double(statement, 7, record->TradeAmount);
	duckdb_bind_double(statement, 8, record->PremiumIn);
	duckdb_bind_double(statement, 9, record->PremiumOut);
	duckdb_bind_varchar(statement, 10, record->TradeDate);
	duckdb_bind_varchar(statement, 11, record->TradeTime);
	duckdb_bind_varchar(statement, 12, record->TradingDay);
	duckdb_bind_varchar(statement, 13, record->ExchangeID);
	duckdb_bind_varchar(statement, 14, record->TradeID);
	duckdb_bind_int32(statement, 15, int(record->Direction));
}
void DuckDB::SetStatementForTradePrimaryKey(duckdb_prepared_statement statement, Trade* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->ExchangeID);
	duckdb_bind_varchar(statement, 3, record->TradeID);
	duckdb_bind_int32(statement, 4, int(record->Direction));
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<Trade*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);
		duckdb_vector column12 = duckdb_data_chunk_get_vector(dataChunk, 12);
		duckdb_vector column13 = duckdb_data_chunk_get_vector(dataChunk, 13);
		duckdb_vector column14 = duckdb_data_chunk_get_vector(dataChunk, 14);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		duckdb_string_t* dataColumn3 = (duckdb_string_t*)duckdb_vector_get_data(column3);
		int* dataColumn4 = (int*)duckdb_vector_get_data(column4);
		duckdb_string_t* dataColumn5 = (duckdb_string_t*)duckdb_vector_get_data(column5);
		int* dataColumn6 = (int*)duckdb_vector_get_data(column6);
		int* dataColumn7 = (int*)duckdb_vector_get_data(column7);
		double* dataColumn8 = (double*)duckdb_vector_get_data(column8);
		int* dataColumn9 = (int*)duckdb_vector_get_data(column9);
		double* dataColumn10 = (double*)duckdb_vector_get_data(column10);
		double* dataColumn11 = (double*)duckdb_vector_get_data(column11);
		double* dataColumn12 = (double*)duckdb_vector_get_data(column12);
		duckdb_string_t* dataColumn13 = (duckdb_string_t*)duckdb_vector_get_data(column13);
		duckdb_string_t* dataColumn14 = (duckdb_string_t*)duckdb_vector_get_data(column14);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);
		uint64_t* validityColumn12 = duckdb_vector_get_validity(column12);
		uint64_t* validityColumn13 = duckdb_vector_get_validity(column13);
		uint64_t* validityColumn14 = duckdb_vector_get_validity(column14);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			Trade* record = Trade::Allocate();
			memset(record, 0, sizeof(Trade));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->TradingDay, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradingDay, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->AccountID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->AccountID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->ExchangeID, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row))
			{
				if (duckdb_string_is_inlined(dataColumn3[row]))
				{
					Strcpy(record->InstrumentID, dataColumn3[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentID, dataColumn3[row].value.pointer.ptr, dataColumn3[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->OrderID = dataColumn4[row];
			if (duckdb_validity_row_is_valid(validityColumn5, row))
			{
				if (duckdb_string_is_inlined(dataColumn5[row]))
				{
					Strcpy(record->TradeID, dataColumn5[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradeID, dataColumn5[row].value.pointer.ptr, dataColumn5[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->Direction = DirectionType(dataColumn6[row]);
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->OffsetFlag = OffsetFlagType(dataColumn7[row]);
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->Price = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->Volume = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->TradeAmount = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row)) record->PremiumIn = dataColumn11[row];
			if (duckdb_validity_row_is_valid(validityColumn12, row)) record->PremiumOut = dataColumn12[row];
			if (duckdb_validity_row_is_valid(validityColumn13, row))
			{
				if (duckdb_string_is_inlined(dataColumn13[row]))
				{
					Strcpy(record->TradeDate, dataColumn13[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradeDate, dataColumn13[row].value.pointer.ptr, dataColumn13[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn14, row))
			{
				if (duckdb_string_is_inlined(dataColumn14[row]))
				{
					Strcpy(record->TradeTime, dataColumn14[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradeTime, dataColumn14[row].value.pointer.ptr, dataColumn14[row].value.pointer.length);
				}
			}
			records.push_back(record);
		}
	}
}
bool DuckDB::AppendForMdTickRecord(duckdb_appender appender, MdTick* record)
{
	duckdb_append_varchar(appender, record->TradingDay);
	duckdb_append_varchar(appender, record->ExchangeID);
	duckdb_append_varchar(appender, record->InstrumentID);
	duckdb_append_double(appender, record->LastPrice);
	duckdb_append_double(appender, record->PreSettlementPrice);
	duckdb_append_double(appender, record->PreClosePrice);
	duckdb_append_int32(appender, record->PreOpenInterest);
	duckdb_append_double(appender, record->OpenPrice);
	duckdb_append_double(appender, record->HighestPrice);
	duckdb_append_double(appender, record->LowestPrice);
	duckdb_append_int32(appender, record->Volume);
	duckdb_append_double(appender, record->Turnover);
	duckdb_append_int32(appender, record->OpenInterest);
	duckdb_append_double(appender, record->UpperLimitPrice);
	duckdb_append_double(appender, record->LowerLimitPrice);
	duckdb_append_varchar(appender, record->UpdateTime);
	duckdb_append_int32(appender, record->UpdateMillisec);
	duckdb_append_double(appender, record->AskPrice1);
	duckdb_append_double(appender, record->AskPrice2);
	duckdb_append_double(appender, record->AskPrice3);
	duckdb_append_double(appender, record->AskPrice4);
	duckdb_append_double(appender, record->AskPrice5);
	duckdb_append_int32(appender, record->AskVolume1);
	duckdb_append_int32(appender, record->AskVolume2);
	duckdb_append_int32(appender, record->AskVolume3);
	duckdb_append_int32(appender, record->AskVolume4);
	duckdb_append_int32(appender, record->AskVolume5);
	duckdb_append_double(appender, record->BidPrice1);
	duckdb_append_double(appender, record->BidPrice2);
	duckdb_append_double(appender, record->BidPrice3);
	duckdb_append_double(appender, record->BidPrice4);
	duckdb_append_double(appender, record->BidPrice5);
	duckdb_append_int32(appender, record->BidVolume1);
	duckdb_append_int32(appender, record->BidVolume2);
	duckdb_append_int32(appender, record->BidVolume3);
	duckdb_append_int32(appender, record->BidVolume4);
	duckdb_append_int32(appender, record->BidVolume5);
	duckdb_append_double(appender, record->AveragePrice);
	if (duckdb_appender_end_row(appender) != DuckDBSuccess)
	{
		WriteLog(LogLevel::Warning, "InsertMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), duckdb_appender_error(appender));
		return false;
	}
	return true;
}
void DuckDB::SetStatementForMdTickRecord(duckdb_prepared_statement statement, MdTick* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->ExchangeID);
	duckdb_bind_varchar(statement, 3, record->InstrumentID);
	duckdb_bind_double(statement, 4, record->LastPrice);
	duckdb_bind_double(statement, 5, record->PreSettlementPrice);
	duckdb_bind_double(statement, 6, record->PreClosePrice);
	duckdb_bind_int32(statement, 7, record->PreOpenInterest);
	duckdb_bind_double(statement, 8, record->OpenPrice);
	duckdb_bind_double(statement, 9, record->HighestPrice);
	duckdb_bind_double(statement, 10, record->LowestPrice);
	duckdb_bind_int32(statement, 11, record->Volume);
	duckdb_bind_double(statement, 12, record->Turnover);
	duckdb_bind_int32(statement, 13, record->OpenInterest);
	duckdb_bind_double(statement, 14, record->UpperLimitPrice);
	duckdb_bind_double(statement, 15, record->LowerLimitPrice);
	duckdb_bind_varchar(statement, 16, record->UpdateTime);
	duckdb_bind_int32(statement, 17, record->UpdateMillisec);
	duckdb_bind_double(statement, 18, record->AskPrice1);
	duckdb_bind_double(statement, 19, record->AskPrice2);
	duckdb_bind_double(statement, 20, record->AskPrice3);
	duckdb_bind_double(statement, 21, record->AskPrice4);
	duckdb_bind_double(statement, 22, record->AskPrice5);
	duckdb_bind_int32(statement, 23, record->AskVolume1);
	duckdb_bind_int32(statement, 24, record->AskVolume2);
	duckdb_bind_int32(statement, 25, record->AskVolume3);
	duckdb_bind_int32(statement, 26, record->AskVolume4);
	duckdb_bind_int32(statement, 27, record->AskVolume5);
	duckdb_bind_double(statement, 28, record->BidPrice1);
	duckdb_bind_double(statement, 29, record->BidPrice2);
	duckdb_bind_double(statement, 30, record->BidPrice3);
	duckdb_bind_double(statement, 31, record->BidPrice4);
	duckdb_bind_double(statement, 32, record->BidPrice5);
	duckdb_bind_int32(statement, 33, record->BidVolume1);
	duckdb_bind_int32(statement, 34, record->BidVolume2);
	duckdb_bind_int32(statement, 35, record->BidVolume3);
	duckdb_bind_int32(statement, 36, record->BidVolume4);
	duckdb_bind_int32(statement, 37, record->BidVolume5);
	duckdb_bind_double(statement, 38, record->AveragePrice);
}
void DuckDB::SetStatementForMdTickRecordUpdate(duckdb_prepared_statement statement, MdTick* record)
{
	duckdb_bind_double(statement, 1, record->LastPrice);
	duckdb_bind_double(statement, 2, record->PreSettlementPrice);
	duckdb_bind_double(statement, 3, record->PreClosePrice);
	duckdb_bind_int32(statement, 4, record->PreOpenInterest);
	duckdb_bind_double(statement, 5, record->OpenPrice);
	duckdb_bind_double(statement, 6, record->HighestPrice);
	duckdb_bind_double(statement, 7, record->LowestPrice);
	duckdb_bind_int32(statement, 8, record->Volume);
	duckdb_bind_double(statement, 9, record->Turnover);
	duckdb_bind_int32(statement, 10, record->OpenInterest);
	duckdb_bind_double(statement, 11, record->UpperLimitPrice);
	duckdb_bind_double(statement, 12, record->LowerLimitPrice);
	duckdb_bind_varchar(statement, 13, record->UpdateTime);
	duckdb_bind_int32(statement, 14, record->UpdateMillisec);
	duckdb_bind_double(statement, 15, record->AskPrice1);
	duckdb_bind_double(statement, 16, record->AskPrice2);
	duckdb_bind_double(statement, 17, record->AskPrice3);
	duckdb_bind_double(statement, 18, record->AskPrice4);
	duckdb_bind_double(statement, 19, record->AskPrice5);
	duckdb_bind_int32(statement, 20, record->AskVolume1);
	duckdb_bind_int32(statement, 21, record->AskVolume2);
	duckdb_bind_int32(statement, 22, record->AskVolume3);
	duckdb_bind_int32(statement, 23, record->AskVolume4);
	duckdb_bind_int32(statement, 24, record->AskVolume5);
	duckdb_bind_double(statement, 25, record->BidPrice1);
	duckdb_bind_double(statement, 26, record->BidPrice2);
	duckdb_bind_double(statement, 27, record->BidPrice3);
	duckdb_bind_double(statement, 28, record->BidPrice4);
	duckdb_bind_double(statement, 29, record->BidPrice5);
	duckdb_bind_int32(statement, 30, record->BidVolume1);
	duckdb_bind_int32(statement, 31, record->BidVolume2);
	duckdb_bind_int32(statement, 32, record->BidVolume3);
	duckdb_bind_int32(statement, 33, record->BidVolume4);
	duckdb_bind_int32(statement, 34, record->BidVolume5);
	duckdb_bind_double(statement, 35, record->AveragePrice);
	duckdb_bind_varchar(statement, 36, record->TradingDay);
	duckdb_bind_varchar(statement, 37, record->ExchangeID);
	duckdb_bind_varchar(statement, 38, record->InstrumentID);
}
void DuckDB::SetStatementForMdTickPrimaryKey(duckdb_prepared_statement statement, MdTick* record)
{
	duckdb_bind_varchar(statement, 1, record->TradingDay);
	duckdb_bind_varchar(statement, 2, record->ExchangeID);
	duckdb_bind_varchar(statement, 3, record->InstrumentID);
}
void DuckDB::ParseRecord(duckdb_result& result, std::vector<MdTick*>& records)
{
	while (true)
	{
		duckdb_data_chunk dataChunk = duckdb_fetch_chunk(result);
		if (dataChunk == nullptr)
		{
			break;
		}
		duckdb_vector column0 = duckdb_data_chunk_get_vector(dataChunk, 0);
		duckdb_vector column1 = duckdb_data_chunk_get_vector(dataChunk, 1);
		duckdb_vector column2 = duckdb_data_chunk_get_vector(dataChunk, 2);
		duckdb_vector column3 = duckdb_data_chunk_get_vector(dataChunk, 3);
		duckdb_vector column4 = duckdb_data_chunk_get_vector(dataChunk, 4);
		duckdb_vector column5 = duckdb_data_chunk_get_vector(dataChunk, 5);
		duckdb_vector column6 = duckdb_data_chunk_get_vector(dataChunk, 6);
		duckdb_vector column7 = duckdb_data_chunk_get_vector(dataChunk, 7);
		duckdb_vector column8 = duckdb_data_chunk_get_vector(dataChunk, 8);
		duckdb_vector column9 = duckdb_data_chunk_get_vector(dataChunk, 9);
		duckdb_vector column10 = duckdb_data_chunk_get_vector(dataChunk, 10);
		duckdb_vector column11 = duckdb_data_chunk_get_vector(dataChunk, 11);
		duckdb_vector column12 = duckdb_data_chunk_get_vector(dataChunk, 12);
		duckdb_vector column13 = duckdb_data_chunk_get_vector(dataChunk, 13);
		duckdb_vector column14 = duckdb_data_chunk_get_vector(dataChunk, 14);
		duckdb_vector column15 = duckdb_data_chunk_get_vector(dataChunk, 15);
		duckdb_vector column16 = duckdb_data_chunk_get_vector(dataChunk, 16);
		duckdb_vector column17 = duckdb_data_chunk_get_vector(dataChunk, 17);
		duckdb_vector column18 = duckdb_data_chunk_get_vector(dataChunk, 18);
		duckdb_vector column19 = duckdb_data_chunk_get_vector(dataChunk, 19);
		duckdb_vector column20 = duckdb_data_chunk_get_vector(dataChunk, 20);
		duckdb_vector column21 = duckdb_data_chunk_get_vector(dataChunk, 21);
		duckdb_vector column22 = duckdb_data_chunk_get_vector(dataChunk, 22);
		duckdb_vector column23 = duckdb_data_chunk_get_vector(dataChunk, 23);
		duckdb_vector column24 = duckdb_data_chunk_get_vector(dataChunk, 24);
		duckdb_vector column25 = duckdb_data_chunk_get_vector(dataChunk, 25);
		duckdb_vector column26 = duckdb_data_chunk_get_vector(dataChunk, 26);
		duckdb_vector column27 = duckdb_data_chunk_get_vector(dataChunk, 27);
		duckdb_vector column28 = duckdb_data_chunk_get_vector(dataChunk, 28);
		duckdb_vector column29 = duckdb_data_chunk_get_vector(dataChunk, 29);
		duckdb_vector column30 = duckdb_data_chunk_get_vector(dataChunk, 30);
		duckdb_vector column31 = duckdb_data_chunk_get_vector(dataChunk, 31);
		duckdb_vector column32 = duckdb_data_chunk_get_vector(dataChunk, 32);
		duckdb_vector column33 = duckdb_data_chunk_get_vector(dataChunk, 33);
		duckdb_vector column34 = duckdb_data_chunk_get_vector(dataChunk, 34);
		duckdb_vector column35 = duckdb_data_chunk_get_vector(dataChunk, 35);
		duckdb_vector column36 = duckdb_data_chunk_get_vector(dataChunk, 36);
		duckdb_vector column37 = duckdb_data_chunk_get_vector(dataChunk, 37);

		duckdb_string_t* dataColumn0 = (duckdb_string_t*)duckdb_vector_get_data(column0);
		duckdb_string_t* dataColumn1 = (duckdb_string_t*)duckdb_vector_get_data(column1);
		duckdb_string_t* dataColumn2 = (duckdb_string_t*)duckdb_vector_get_data(column2);
		double* dataColumn3 = (double*)duckdb_vector_get_data(column3);
		double* dataColumn4 = (double*)duckdb_vector_get_data(column4);
		double* dataColumn5 = (double*)duckdb_vector_get_data(column5);
		int* dataColumn6 = (int*)duckdb_vector_get_data(column6);
		double* dataColumn7 = (double*)duckdb_vector_get_data(column7);
		double* dataColumn8 = (double*)duckdb_vector_get_data(column8);
		double* dataColumn9 = (double*)duckdb_vector_get_data(column9);
		int* dataColumn10 = (int*)duckdb_vector_get_data(column10);
		double* dataColumn11 = (double*)duckdb_vector_get_data(column11);
		int* dataColumn12 = (int*)duckdb_vector_get_data(column12);
		double* dataColumn13 = (double*)duckdb_vector_get_data(column13);
		double* dataColumn14 = (double*)duckdb_vector_get_data(column14);
		duckdb_string_t* dataColumn15 = (duckdb_string_t*)duckdb_vector_get_data(column15);
		int* dataColumn16 = (int*)duckdb_vector_get_data(column16);
		double* dataColumn17 = (double*)duckdb_vector_get_data(column17);
		double* dataColumn18 = (double*)duckdb_vector_get_data(column18);
		double* dataColumn19 = (double*)duckdb_vector_get_data(column19);
		double* dataColumn20 = (double*)duckdb_vector_get_data(column20);
		double* dataColumn21 = (double*)duckdb_vector_get_data(column21);
		int* dataColumn22 = (int*)duckdb_vector_get_data(column22);
		int* dataColumn23 = (int*)duckdb_vector_get_data(column23);
		int* dataColumn24 = (int*)duckdb_vector_get_data(column24);
		int* dataColumn25 = (int*)duckdb_vector_get_data(column25);
		int* dataColumn26 = (int*)duckdb_vector_get_data(column26);
		double* dataColumn27 = (double*)duckdb_vector_get_data(column27);
		double* dataColumn28 = (double*)duckdb_vector_get_data(column28);
		double* dataColumn29 = (double*)duckdb_vector_get_data(column29);
		double* dataColumn30 = (double*)duckdb_vector_get_data(column30);
		double* dataColumn31 = (double*)duckdb_vector_get_data(column31);
		int* dataColumn32 = (int*)duckdb_vector_get_data(column32);
		int* dataColumn33 = (int*)duckdb_vector_get_data(column33);
		int* dataColumn34 = (int*)duckdb_vector_get_data(column34);
		int* dataColumn35 = (int*)duckdb_vector_get_data(column35);
		int* dataColumn36 = (int*)duckdb_vector_get_data(column36);
		double* dataColumn37 = (double*)duckdb_vector_get_data(column37);

		uint64_t* validityColumn0 = duckdb_vector_get_validity(column0);
		uint64_t* validityColumn1 = duckdb_vector_get_validity(column1);
		uint64_t* validityColumn2 = duckdb_vector_get_validity(column2);
		uint64_t* validityColumn3 = duckdb_vector_get_validity(column3);
		uint64_t* validityColumn4 = duckdb_vector_get_validity(column4);
		uint64_t* validityColumn5 = duckdb_vector_get_validity(column5);
		uint64_t* validityColumn6 = duckdb_vector_get_validity(column6);
		uint64_t* validityColumn7 = duckdb_vector_get_validity(column7);
		uint64_t* validityColumn8 = duckdb_vector_get_validity(column8);
		uint64_t* validityColumn9 = duckdb_vector_get_validity(column9);
		uint64_t* validityColumn10 = duckdb_vector_get_validity(column10);
		uint64_t* validityColumn11 = duckdb_vector_get_validity(column11);
		uint64_t* validityColumn12 = duckdb_vector_get_validity(column12);
		uint64_t* validityColumn13 = duckdb_vector_get_validity(column13);
		uint64_t* validityColumn14 = duckdb_vector_get_validity(column14);
		uint64_t* validityColumn15 = duckdb_vector_get_validity(column15);
		uint64_t* validityColumn16 = duckdb_vector_get_validity(column16);
		uint64_t* validityColumn17 = duckdb_vector_get_validity(column17);
		uint64_t* validityColumn18 = duckdb_vector_get_validity(column18);
		uint64_t* validityColumn19 = duckdb_vector_get_validity(column19);
		uint64_t* validityColumn20 = duckdb_vector_get_validity(column20);
		uint64_t* validityColumn21 = duckdb_vector_get_validity(column21);
		uint64_t* validityColumn22 = duckdb_vector_get_validity(column22);
		uint64_t* validityColumn23 = duckdb_vector_get_validity(column23);
		uint64_t* validityColumn24 = duckdb_vector_get_validity(column24);
		uint64_t* validityColumn25 = duckdb_vector_get_validity(column25);
		uint64_t* validityColumn26 = duckdb_vector_get_validity(column26);
		uint64_t* validityColumn27 = duckdb_vector_get_validity(column27);
		uint64_t* validityColumn28 = duckdb_vector_get_validity(column28);
		uint64_t* validityColumn29 = duckdb_vector_get_validity(column29);
		uint64_t* validityColumn30 = duckdb_vector_get_validity(column30);
		uint64_t* validityColumn31 = duckdb_vector_get_validity(column31);
		uint64_t* validityColumn32 = duckdb_vector_get_validity(column32);
		uint64_t* validityColumn33 = duckdb_vector_get_validity(column33);
		uint64_t* validityColumn34 = duckdb_vector_get_validity(column34);
		uint64_t* validityColumn35 = duckdb_vector_get_validity(column35);
		uint64_t* validityColumn36 = duckdb_vector_get_validity(column36);
		uint64_t* validityColumn37 = duckdb_vector_get_validity(column37);

		idx_t rowCount = duckdb_data_chunk_get_size(dataChunk);
		for (idx_t row = 0LL; row < rowCount; ++row)
		{
			MdTick* record = MdTick::Allocate();
			memset(record, 0, sizeof(MdTick));
			if (duckdb_validity_row_is_valid(validityColumn0, row))
			{
				if (duckdb_string_is_inlined(dataColumn0[row]))
				{
					Strcpy(record->TradingDay, dataColumn0[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->TradingDay, dataColumn0[row].value.pointer.ptr, dataColumn0[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn1, row))
			{
				if (duckdb_string_is_inlined(dataColumn1[row]))
				{
					Strcpy(record->ExchangeID, dataColumn1[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->ExchangeID, dataColumn1[row].value.pointer.ptr, dataColumn1[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn2, row))
			{
				if (duckdb_string_is_inlined(dataColumn2[row]))
				{
					Strcpy(record->InstrumentID, dataColumn2[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->InstrumentID, dataColumn2[row].value.pointer.ptr, dataColumn2[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn3, row)) record->LastPrice = dataColumn3[row];
			if (duckdb_validity_row_is_valid(validityColumn4, row)) record->PreSettlementPrice = dataColumn4[row];
			if (duckdb_validity_row_is_valid(validityColumn5, row)) record->PreClosePrice = dataColumn5[row];
			if (duckdb_validity_row_is_valid(validityColumn6, row)) record->PreOpenInterest = dataColumn6[row];
			if (duckdb_validity_row_is_valid(validityColumn7, row)) record->OpenPrice = dataColumn7[row];
			if (duckdb_validity_row_is_valid(validityColumn8, row)) record->HighestPrice = dataColumn8[row];
			if (duckdb_validity_row_is_valid(validityColumn9, row)) record->LowestPrice = dataColumn9[row];
			if (duckdb_validity_row_is_valid(validityColumn10, row)) record->Volume = dataColumn10[row];
			if (duckdb_validity_row_is_valid(validityColumn11, row)) record->Turnover = dataColumn11[row];
			if (duckdb_validity_row_is_valid(validityColumn12, row)) record->OpenInterest = dataColumn12[row];
			if (duckdb_validity_row_is_valid(validityColumn13, row)) record->UpperLimitPrice = dataColumn13[row];
			if (duckdb_validity_row_is_valid(validityColumn14, row)) record->LowerLimitPrice = dataColumn14[row];
			if (duckdb_validity_row_is_valid(validityColumn15, row))
			{
				if (duckdb_string_is_inlined(dataColumn15[row]))
				{
					Strcpy(record->UpdateTime, dataColumn15[row].value.inlined.inlined);
				}
				else
				{
					memcpy(record->UpdateTime, dataColumn15[row].value.pointer.ptr, dataColumn15[row].value.pointer.length);
				}
			}
			if (duckdb_validity_row_is_valid(validityColumn16, row)) record->UpdateMillisec = dataColumn16[row];
			if (duckdb_validity_row_is_valid(validityColumn17, row)) record->AskPrice1 = dataColumn17[row];
			if (duckdb_validity_row_is_valid(validityColumn18, row)) record->AskPrice2 = dataColumn18[row];
			if (duckdb_validity_row_is_valid(validityColumn19, row)) record->AskPrice3 = dataColumn19[row];
			if (duckdb_validity_row_is_valid(validityColumn20, row)) record->AskPrice4 = dataColumn20[row];
			if (duckdb_validity_row_is_valid(validityColumn21, row)) record->AskPrice5 = dataColumn21[row];
			if (duckdb_validity_row_is_valid(validityColumn22, row)) record->AskVolume1 = dataColumn22[row];
			if (duckdb_validity_row_is_valid(validityColumn23, row)) record->AskVolume2 = dataColumn23[row];
			if (duckdb_validity_row_is_valid(validityColumn24, row)) record->AskVolume3 = dataColumn24[row];
			if (duckdb_validity_row_is_valid(validityColumn25, row)) record->AskVolume4 = dataColumn25[row];
			if (duckdb_validity_row_is_valid(validityColumn26, row)) record->AskVolume5 = dataColumn26[row];
			if (duckdb_validity_row_is_valid(validityColumn27, row)) record->BidPrice1 = dataColumn27[row];
			if (duckdb_validity_row_is_valid(validityColumn28, row)) record->BidPrice2 = dataColumn28[row];
			if (duckdb_validity_row_is_valid(validityColumn29, row)) record->BidPrice3 = dataColumn29[row];
			if (duckdb_validity_row_is_valid(validityColumn30, row)) record->BidPrice4 = dataColumn30[row];
			if (duckdb_validity_row_is_valid(validityColumn31, row)) record->BidPrice5 = dataColumn31[row];
			if (duckdb_validity_row_is_valid(validityColumn32, row)) record->BidVolume1 = dataColumn32[row];
			if (duckdb_validity_row_is_valid(validityColumn33, row)) record->BidVolume2 = dataColumn33[row];
			if (duckdb_validity_row_is_valid(validityColumn34, row)) record->BidVolume3 = dataColumn34[row];
			if (duckdb_validity_row_is_valid(validityColumn35, row)) record->BidVolume4 = dataColumn35[row];
			if (duckdb_validity_row_is_valid(validityColumn36, row)) record->BidVolume5 = dataColumn36[row];
			if (duckdb_validity_row_is_valid(validityColumn37, row)) record->AveragePrice = dataColumn37[row];
			records.push_back(record);
		}
	}
}
