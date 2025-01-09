#include "SqliteDB.h"
#include "Constant.h"
#include "Logger.h"
#include "Utility.h"
#include "TimeUtility.h"
#include <string.h>
#include <cstring>

using namespace mdb;
using namespace std;
using namespace std::chrono;

SqliteDB::SqliteDB(const std::string& dbName)
	:m_DBName(dbName), m_DB(nullptr)
{
	m_SqlBuff = new char[BuffSize];

	m_TradingDayInsertStatement = nullptr;
	m_TradingDayDeleteStatement = nullptr;
	m_TradingDayUpdateStatement = nullptr;
	m_TradingDaySelectStatement = nullptr;
	m_TradingDayTruncateStatement = nullptr;

	m_ExchangeInsertStatement = nullptr;
	m_ExchangeDeleteStatement = nullptr;
	m_ExchangeUpdateStatement = nullptr;
	m_ExchangeSelectStatement = nullptr;
	m_ExchangeTruncateStatement = nullptr;

	m_ProductInsertStatement = nullptr;
	m_ProductDeleteStatement = nullptr;
	m_ProductUpdateStatement = nullptr;
	m_ProductSelectStatement = nullptr;
	m_ProductTruncateStatement = nullptr;

	m_InstrumentInsertStatement = nullptr;
	m_InstrumentDeleteStatement = nullptr;
	m_InstrumentUpdateStatement = nullptr;
	m_InstrumentSelectStatement = nullptr;
	m_InstrumentTruncateStatement = nullptr;

	m_AccountInsertStatement = nullptr;
	m_AccountDeleteStatement = nullptr;
	m_AccountUpdateStatement = nullptr;
	m_AccountSelectStatement = nullptr;
	m_AccountTruncateStatement = nullptr;

	m_PositionInsertStatement = nullptr;
	m_PositionDeleteStatement = nullptr;
	m_PositionDeleteByAccountIndexStatement = nullptr;
	m_PositionUpdateStatement = nullptr;
	m_PositionSelectStatement = nullptr;
	m_PositionTruncateStatement = nullptr;

	m_OrderInsertStatement = nullptr;
	m_OrderDeleteStatement = nullptr;
	m_OrderUpdateStatement = nullptr;
	m_OrderSelectStatement = nullptr;
	m_OrderTruncateStatement = nullptr;

	m_TradeInsertStatement = nullptr;
	m_TradeDeleteStatement = nullptr;
	m_TradeUpdateStatement = nullptr;
	m_TradeSelectStatement = nullptr;
	m_TradeTruncateStatement = nullptr;

	m_MdTickInsertStatement = nullptr;
	m_MdTickDeleteStatement = nullptr;
	m_MdTickUpdateStatement = nullptr;
	m_MdTickSelectStatement = nullptr;
	m_MdTickTruncateStatement = nullptr;

}
SqliteDB::~SqliteDB()
{
	delete[] m_SqlBuff;
	DisConnect();
	if (m_DB != nullptr)
	{
		sqlite3_close(m_DB);
	}
}
bool SqliteDB::Connect()
{
	int result = sqlite3_open(m_DBName.c_str(), &m_DB);
	if (result != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "Open SqliteDB Failed.");
		return false;
	}
	return true;
}
void SqliteDB::DisConnect()
{
	if (m_DB != nullptr)
	{
		sqlite3_close(m_DB);
		m_DB = nullptr;
	}
	if (m_TradingDayInsertStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayInsertStatement);
		m_TradingDayInsertStatement = nullptr;
	}
	if (m_TradingDayDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayDeleteStatement);
		m_TradingDayDeleteStatement = nullptr;
	}
	if (m_TradingDayUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayUpdateStatement);
		m_TradingDayUpdateStatement = nullptr;
	}
	if (m_TradingDaySelectStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDaySelectStatement);
		m_TradingDaySelectStatement = nullptr;
	}
	if (m_TradingDayTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayTruncateStatement);
		m_TradingDayTruncateStatement = nullptr;
	}
	if (m_ExchangeInsertStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeInsertStatement);
		m_ExchangeInsertStatement = nullptr;
	}
	if (m_ExchangeDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeDeleteStatement);
		m_ExchangeDeleteStatement = nullptr;
	}
	if (m_ExchangeUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeUpdateStatement);
		m_ExchangeUpdateStatement = nullptr;
	}
	if (m_ExchangeSelectStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeSelectStatement);
		m_ExchangeSelectStatement = nullptr;
	}
	if (m_ExchangeTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeTruncateStatement);
		m_ExchangeTruncateStatement = nullptr;
	}
	if (m_ProductInsertStatement != nullptr)
	{
		sqlite3_finalize(m_ProductInsertStatement);
		m_ProductInsertStatement = nullptr;
	}
	if (m_ProductDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_ProductDeleteStatement);
		m_ProductDeleteStatement = nullptr;
	}
	if (m_ProductUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_ProductUpdateStatement);
		m_ProductUpdateStatement = nullptr;
	}
	if (m_ProductSelectStatement != nullptr)
	{
		sqlite3_finalize(m_ProductSelectStatement);
		m_ProductSelectStatement = nullptr;
	}
	if (m_ProductTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_ProductTruncateStatement);
		m_ProductTruncateStatement = nullptr;
	}
	if (m_InstrumentInsertStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentInsertStatement);
		m_InstrumentInsertStatement = nullptr;
	}
	if (m_InstrumentDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentDeleteStatement);
		m_InstrumentDeleteStatement = nullptr;
	}
	if (m_InstrumentUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentUpdateStatement);
		m_InstrumentUpdateStatement = nullptr;
	}
	if (m_InstrumentSelectStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentSelectStatement);
		m_InstrumentSelectStatement = nullptr;
	}
	if (m_InstrumentTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentTruncateStatement);
		m_InstrumentTruncateStatement = nullptr;
	}
	if (m_AccountInsertStatement != nullptr)
	{
		sqlite3_finalize(m_AccountInsertStatement);
		m_AccountInsertStatement = nullptr;
	}
	if (m_AccountDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_AccountDeleteStatement);
		m_AccountDeleteStatement = nullptr;
	}
	if (m_AccountUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_AccountUpdateStatement);
		m_AccountUpdateStatement = nullptr;
	}
	if (m_AccountSelectStatement != nullptr)
	{
		sqlite3_finalize(m_AccountSelectStatement);
		m_AccountSelectStatement = nullptr;
	}
	if (m_AccountTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_AccountTruncateStatement);
		m_AccountTruncateStatement = nullptr;
	}
	if (m_PositionInsertStatement != nullptr)
	{
		sqlite3_finalize(m_PositionInsertStatement);
		m_PositionInsertStatement = nullptr;
	}
	if (m_PositionDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDeleteStatement);
		m_PositionDeleteStatement = nullptr;
	}
	if (m_PositionDeleteByAccountIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDeleteByAccountIndexStatement);
		m_PositionDeleteByAccountIndexStatement = nullptr;
	}
	if (m_PositionUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionUpdateStatement);
		m_PositionUpdateStatement = nullptr;
	}
	if (m_PositionSelectStatement != nullptr)
	{
		sqlite3_finalize(m_PositionSelectStatement);
		m_PositionSelectStatement = nullptr;
	}
	if (m_PositionTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionTruncateStatement);
		m_PositionTruncateStatement = nullptr;
	}
	if (m_OrderInsertStatement != nullptr)
	{
		sqlite3_finalize(m_OrderInsertStatement);
		m_OrderInsertStatement = nullptr;
	}
	if (m_OrderDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_OrderDeleteStatement);
		m_OrderDeleteStatement = nullptr;
	}
	if (m_OrderUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_OrderUpdateStatement);
		m_OrderUpdateStatement = nullptr;
	}
	if (m_OrderSelectStatement != nullptr)
	{
		sqlite3_finalize(m_OrderSelectStatement);
		m_OrderSelectStatement = nullptr;
	}
	if (m_OrderTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_OrderTruncateStatement);
		m_OrderTruncateStatement = nullptr;
	}
	if (m_TradeInsertStatement != nullptr)
	{
		sqlite3_finalize(m_TradeInsertStatement);
		m_TradeInsertStatement = nullptr;
	}
	if (m_TradeDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_TradeDeleteStatement);
		m_TradeDeleteStatement = nullptr;
	}
	if (m_TradeUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_TradeUpdateStatement);
		m_TradeUpdateStatement = nullptr;
	}
	if (m_TradeSelectStatement != nullptr)
	{
		sqlite3_finalize(m_TradeSelectStatement);
		m_TradeSelectStatement = nullptr;
	}
	if (m_TradeTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_TradeTruncateStatement);
		m_TradeTruncateStatement = nullptr;
	}
	if (m_MdTickInsertStatement != nullptr)
	{
		sqlite3_finalize(m_MdTickInsertStatement);
		m_MdTickInsertStatement = nullptr;
	}
	if (m_MdTickDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_MdTickDeleteStatement);
		m_MdTickDeleteStatement = nullptr;
	}
	if (m_MdTickUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_MdTickUpdateStatement);
		m_MdTickUpdateStatement = nullptr;
	}
	if (m_MdTickSelectStatement != nullptr)
	{
		sqlite3_finalize(m_MdTickSelectStatement);
		m_MdTickSelectStatement = nullptr;
	}
	if (m_MdTickTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_MdTickTruncateStatement);
		m_MdTickTruncateStatement = nullptr;
	}
}
void SqliteDB::InitDB()
{
	Exec("Truncate Table t_TradingDay;");
	Exec("Insert Into t_TradingDay select * from Init.t_TradingDay;");
	Exec("Truncate Table t_Exchange;");
	Exec("Insert Into t_Exchange select * from Init.t_Exchange;");
	Exec("Truncate Table t_Product;");
	Exec("Insert Into t_Product select * from Init.t_Product;");
	Exec("Truncate Table t_Instrument;");
	Exec("Insert Into t_Instrument select * from Init.t_Instrument;");
	Exec("Truncate Table t_Account;");
	Exec("Insert Into t_Account select * from Init.t_Account;");
	Exec("Truncate Table t_Position;");
	Exec("Insert Into t_Position select * from Init.t_Position;");
	Exec("Truncate Table t_Order;");
	Exec("Insert Into t_Order select * from Init.t_Order;");
	Exec("Truncate Table t_Trade;");
	Exec("Insert Into t_Trade select * from Init.t_Trade;");
	Exec("Truncate Table t_MdTick;");
	Exec("Insert Into t_MdTick select * from Init.t_MdTick;");
}
void SqliteDB::TruncateSessionTables()
{
	auto start = steady_clock::now();
	TruncateTradingDay();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::TruncateTables()
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
bool SqliteDB::Exec(const char* sql)
{
	char* t_ErrorMsg;
	auto ret = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "Sqlitedb::Exec Failed. Sql:%s, Error: %s\n", sql, t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
		return false;
	}
	return true;
}

void SqliteDB::InsertTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_TradingDay Values(?, ?, ?);", -1, &m_TradingDayInsertStatement, nullptr);
	}
	SetStatementForTradingDayRecord(m_TradingDayInsertStatement, record);
	
	auto rc = sqlite3_step(m_TradingDayInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_TradingDay where PK = ?;", -1, &m_TradingDayDeleteStatement, nullptr);
	}
	SetStatementForTradingDayPrimaryKey(m_TradingDayDeleteStatement, record->PK);

	auto rc = sqlite3_step(m_TradingDayDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_TradingDay set CurrTradingDay = ?, PreTradingDay = ? where PK = ?;", -1, &m_TradingDayUpdateStatement, nullptr);
	}
	SetStatementForTradingDayRecordUpdate(m_TradingDayUpdateStatement, record);
	
	auto rc = sqlite3_step(m_TradingDayUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void SqliteDB::SelectTradingDay(std::vector<TradingDay*>& records)
{
	auto start = steady_clock::now();
	if (m_TradingDaySelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_TradingDay;", -1, &m_TradingDaySelectStatement, nullptr);
	}

	while (sqlite3_step(m_TradingDaySelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_TradingDaySelectStatement, records);
	}
	sqlite3_reset(m_TradingDaySelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_TradingDay;", -1, &m_TradingDayTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_TradingDayTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateTradingDay failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Exchange Values(?, ?);", -1, &m_ExchangeInsertStatement, nullptr);
	}
	SetStatementForExchangeRecord(m_ExchangeInsertStatement, record);
	
	auto rc = sqlite3_step(m_ExchangeInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertExchange Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertExchange(std::list<Exchange*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Exchange Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Exchange Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Exchange where ExchangeID = ?;", -1, &m_ExchangeDeleteStatement, nullptr);
	}
	SetStatementForExchangePrimaryKey(m_ExchangeDeleteStatement, record->ExchangeID);

	auto rc = sqlite3_step(m_ExchangeDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Exchange set ExchangeName = ? where ExchangeID = ?;", -1, &m_ExchangeUpdateStatement, nullptr);
	}
	SetStatementForExchangeRecordUpdate(m_ExchangeUpdateStatement, record);
	
	auto rc = sqlite3_step(m_ExchangeUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void SqliteDB::SelectExchange(std::vector<Exchange*>& records)
{
	auto start = steady_clock::now();
	if (m_ExchangeSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Exchange;", -1, &m_ExchangeSelectStatement, nullptr);
	}

	while (sqlite3_step(m_ExchangeSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_ExchangeSelectStatement, records);
	}
	sqlite3_reset(m_ExchangeSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Exchange;", -1, &m_ExchangeTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_ExchangeTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateExchange failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_ProductInsertStatement, nullptr);
	}
	SetStatementForProductRecord(m_ProductInsertStatement, record);
	
	auto rc = sqlite3_step(m_ProductInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertProduct Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertProduct(std::list<Product*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Product Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Product Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Product where ExchangeID = ? and ProductID = ?;", -1, &m_ProductDeleteStatement, nullptr);
	}
	SetStatementForProductPrimaryKey(m_ProductDeleteStatement, record->ExchangeID, record->ProductID);

	auto rc = sqlite3_step(m_ProductDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Product set ProductName = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and ProductID = ?;", -1, &m_ProductUpdateStatement, nullptr);
	}
	SetStatementForProductRecordUpdate(m_ProductUpdateStatement, record);
	
	auto rc = sqlite3_step(m_ProductUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void SqliteDB::SelectProduct(std::vector<Product*>& records)
{
	auto start = steady_clock::now();
	if (m_ProductSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Product;", -1, &m_ProductSelectStatement, nullptr);
	}

	while (sqlite3_step(m_ProductSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_ProductSelectStatement, records);
	}
	sqlite3_reset(m_ProductSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateProduct()
{
	auto start = steady_clock::now();
	if (m_ProductTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Product;", -1, &m_ProductTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_ProductTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateProduct failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Instrument Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_InstrumentInsertStatement, nullptr);
	}
	SetStatementForInstrumentRecord(m_InstrumentInsertStatement, record);
	
	auto rc = sqlite3_step(m_InstrumentInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertInstrument(std::list<Instrument*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Instrument Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Instrument Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Instrument where ExchangeID = ? and InstrumentID = ?;", -1, &m_InstrumentDeleteStatement, nullptr);
	}
	SetStatementForInstrumentPrimaryKey(m_InstrumentDeleteStatement, record->ExchangeID, record->InstrumentID);

	auto rc = sqlite3_step(m_InstrumentDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Instrument set InstrumentName = ?, ProductID = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ?, DeliveryYear = ?, DeliveryMonth = ? where ExchangeID = ? and InstrumentID = ?;", -1, &m_InstrumentUpdateStatement, nullptr);
	}
	SetStatementForInstrumentRecordUpdate(m_InstrumentUpdateStatement, record);
	
	auto rc = sqlite3_step(m_InstrumentUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void SqliteDB::SelectInstrument(std::vector<Instrument*>& records)
{
	auto start = steady_clock::now();
	if (m_InstrumentSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Instrument;", -1, &m_InstrumentSelectStatement, nullptr);
	}

	while (sqlite3_step(m_InstrumentSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_InstrumentSelectStatement, records);
	}
	sqlite3_reset(m_InstrumentSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Instrument;", -1, &m_InstrumentTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_InstrumentTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateInstrument failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Account Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_AccountInsertStatement, nullptr);
	}
	SetStatementForAccountRecord(m_AccountInsertStatement, record);
	
	auto rc = sqlite3_step(m_AccountInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertAccount Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertAccount(std::list<Account*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Account Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Account Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Account where AccountID = ?;", -1, &m_AccountDeleteStatement, nullptr);
	}
	SetStatementForAccountPrimaryKey(m_AccountDeleteStatement, record->AccountID);

	auto rc = sqlite3_step(m_AccountDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Account set TradingDay = ?, AccountName = ?, Password = ?, PreBalance = ?, Balance = ?, CloseProfitByDate = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, PremiumIn = ?, PremiumOut = ?, MarketValue = ? where AccountID = ?;", -1, &m_AccountUpdateStatement, nullptr);
	}
	SetStatementForAccountRecordUpdate(m_AccountUpdateStatement, record);
	
	auto rc = sqlite3_step(m_AccountUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void SqliteDB::SelectAccount(std::vector<Account*>& records)
{
	auto start = steady_clock::now();
	if (m_AccountSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Account;", -1, &m_AccountSelectStatement, nullptr);
	}

	while (sqlite3_step(m_AccountSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_AccountSelectStatement, records);
	}
	sqlite3_reset(m_AccountSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateAccount()
{
	auto start = steady_clock::now();
	if (m_AccountTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Account;", -1, &m_AccountTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_AccountTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateAccount failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertPosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_PositionInsertStatement, nullptr);
	}
	SetStatementForPositionRecord(m_PositionInsertStatement, record);
	
	auto rc = sqlite3_step(m_PositionInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertPosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPosition Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertPosition(std::list<Position*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Position Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Position Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", -1, &m_PositionDeleteStatement, nullptr);
	}
	SetStatementForPositionPrimaryKey(m_PositionDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection);

	auto rc = sqlite3_step(m_PositionDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void SqliteDB::DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByAccountIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position where TradingDay = ? and AccountID = ?;", -1, &m_PositionDeleteByAccountIndexStatement, nullptr);
	}
	SetStatementForPositionIndexAccount(m_PositionDeleteByAccountIndexStatement, record);
	
	auto rc = sqlite3_step(m_PositionDeleteByAccountIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDeleteByAccountIndexStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void SqliteDB::UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Position set ProductClass = ?, TotalPosition = ?, TodayPosition = ?, FrozenPosition = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, PremiumIn = ?, PremiumOut = ?, MarketValue = ?, VolumeMultiple = ?, SettlementPrice = ?, PreSettlementPrice = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", -1, &m_PositionUpdateStatement, nullptr);
	}
	SetStatementForPositionRecordUpdate(m_PositionUpdateStatement, record);
	
	auto rc = sqlite3_step(m_PositionUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void SqliteDB::SelectPosition(std::vector<Position*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Position;", -1, &m_PositionSelectStatement, nullptr);
	}

	while (sqlite3_step(m_PositionSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_PositionSelectStatement, records);
	}
	sqlite3_reset(m_PositionSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void SqliteDB::TruncatePosition()
{
	auto start = steady_clock::now();
	if (m_PositionTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position;", -1, &m_PositionTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_PositionTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncatePosition failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_OrderInsertStatement, nullptr);
	}
	SetStatementForOrderRecord(m_OrderInsertStatement, record);
	
	auto rc = sqlite3_step(m_OrderInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertOrder Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertOrder(std::list<Order*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Order Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Order Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Order where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", -1, &m_OrderDeleteStatement, nullptr);
	}
	SetStatementForOrderPrimaryKey(m_OrderDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->OrderID);

	auto rc = sqlite3_step(m_OrderDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Order set ClientOrderID = ?, Direction = ?, OffsetFlag = ?, OrderPriceType = ?, Price = ?, Volume = ?, VolumeRemain = ?, VolumeTraded = ?, VolumeMultiple = ?, OrderStatus = ?, OrderDate = ?, OrderTime = ?, CancelDate = ?, CancelTime = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", -1, &m_OrderUpdateStatement, nullptr);
	}
	SetStatementForOrderRecordUpdate(m_OrderUpdateStatement, record);
	
	auto rc = sqlite3_step(m_OrderUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void SqliteDB::SelectOrder(std::vector<Order*>& records)
{
	auto start = steady_clock::now();
	if (m_OrderSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Order;", -1, &m_OrderSelectStatement, nullptr);
	}

	while (sqlite3_step(m_OrderSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_OrderSelectStatement, records);
	}
	sqlite3_reset(m_OrderSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateOrder()
{
	auto start = steady_clock::now();
	if (m_OrderTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Order;", -1, &m_OrderTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_OrderTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateOrder failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_TradeInsertStatement, nullptr);
	}
	SetStatementForTradeRecord(m_TradeInsertStatement, record);
	
	auto rc = sqlite3_step(m_TradeInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTrade Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertTrade(std::list<Trade*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Trade Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Trade Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Trade where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", -1, &m_TradeDeleteStatement, nullptr);
	}
	SetStatementForTradePrimaryKey(m_TradeDeleteStatement, record->TradingDay, record->ExchangeID, record->TradeID, record->Direction);

	auto rc = sqlite3_step(m_TradeDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Trade set AccountID = ?, InstrumentID = ?, OrderID = ?, OffsetFlag = ?, Price = ?, Volume = ?, TradeAmount = ?, PremiumIn = ?, PremiumOut = ?, TradeDate = ?, TradeTime = ? where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", -1, &m_TradeUpdateStatement, nullptr);
	}
	SetStatementForTradeRecordUpdate(m_TradeUpdateStatement, record);
	
	auto rc = sqlite3_step(m_TradeUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void SqliteDB::SelectTrade(std::vector<Trade*>& records)
{
	auto start = steady_clock::now();
	if (m_TradeSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_Trade;", -1, &m_TradeSelectStatement, nullptr);
	}

	while (sqlite3_step(m_TradeSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_TradeSelectStatement, records);
	}
	sqlite3_reset(m_TradeSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateTrade()
{
	auto start = steady_clock::now();
	if (m_TradeTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Trade;", -1, &m_TradeTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_TradeTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateTrade failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void SqliteDB::InsertMdTick(MdTick* record)
{
	auto start = steady_clock::now();
	if (m_MdTickInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_MdTick Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_MdTickInsertStatement, nullptr);
	}
	SetStatementForMdTickRecord(m_MdTickInsertStatement, record);
	
	auto rc = sqlite3_step(m_MdTickInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_MdTickInsertStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertMdTick Spend:%lldms", duration);
	}
}
void SqliteDB::BatchInsertMdTick(std::list<MdTick*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_MdTick Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertMdTick Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_MdTick Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertMdTick Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertMdTick RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void SqliteDB::DeleteMdTick(MdTick* record)
{
	auto start = steady_clock::now();
	if (m_MdTickDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_MdTick where TradingDay = ? and ExchangeID = ? and InstrumentID = ?;", -1, &m_MdTickDeleteStatement, nullptr);
	}
	SetStatementForMdTickPrimaryKey(m_MdTickDeleteStatement, record->TradingDay, record->ExchangeID, record->InstrumentID);

	auto rc = sqlite3_step(m_MdTickDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_MdTickDeleteStatement);

	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteMdTick Spend:%lldms", duration);
	}
}
void SqliteDB::UpdateMdTick(MdTick* record)
{
	auto start = steady_clock::now();
	if (m_MdTickUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_MdTick set LastPrice = ?, PreSettlementPrice = ?, PreClosePrice = ?, PreOpenInterest = ?, OpenPrice = ?, HighestPrice = ?, LowestPrice = ?, Volume = ?, Turnover = ?, OpenInterest = ?, UpperLimitPrice = ?, LowerLimitPrice = ?, UpdateTime = ?, UpdateMillisec = ?, AskPrice1 = ?, AskPrice2 = ?, AskPrice3 = ?, AskPrice4 = ?, AskPrice5 = ?, AskVolume1 = ?, AskVolume2 = ?, AskVolume3 = ?, AskVolume4 = ?, AskVolume5 = ?, BidPrice1 = ?, BidPrice2 = ?, BidPrice3 = ?, BidPrice4 = ?, BidPrice5 = ?, BidVolume1 = ?, BidVolume2 = ?, BidVolume3 = ?, BidVolume4 = ?, BidVolume5 = ?, AveragePrice = ? where TradingDay = ? and ExchangeID = ? and InstrumentID = ?;", -1, &m_MdTickUpdateStatement, nullptr);
	}
	SetStatementForMdTickRecordUpdate(m_MdTickUpdateStatement, record);
	
	auto rc = sqlite3_step(m_MdTickUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateMdTick failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_MdTickUpdateStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateMdTick Spend:%lldms", duration);
	}
}
void SqliteDB::SelectMdTick(std::vector<MdTick*>& records)
{
	auto start = steady_clock::now();
	if (m_MdTickSelectStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "select * from t_MdTick;", -1, &m_MdTickSelectStatement, nullptr);
	}

	while (sqlite3_step(m_MdTickSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_MdTickSelectStatement, records);
	}
	sqlite3_reset(m_MdTickSelectStatement);
	
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectMdTick Spend:%lldms", duration);
	}
}
void SqliteDB::TruncateMdTick()
{
	auto start = steady_clock::now();
	if (m_MdTickTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_MdTick;", -1, &m_MdTickTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_MdTickTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateMdTick failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_MdTickTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateMdTick Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}


void SqliteDB::SetStatementForTradingDayRecord(sqlite3_stmt* statement, TradingDay* record)
{
	sqlite3_bind_int(statement, 1, record->PK);
	sqlite3_bind_text(statement, 2, record->CurrTradingDay, sizeof(record->CurrTradingDay), nullptr);
	sqlite3_bind_text(statement, 3, record->PreTradingDay, sizeof(record->PreTradingDay), nullptr);
}
void SqliteDB::SetStatementForTradingDayRecordUpdate(sqlite3_stmt* statement, TradingDay* record)
{
	sqlite3_bind_text(statement, 1, record->CurrTradingDay, sizeof(record->CurrTradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->PreTradingDay, sizeof(record->PreTradingDay), nullptr);
	sqlite3_bind_int(statement, 3, record->PK);
}
void SqliteDB::SetStatementForTradingDayPrimaryKey(sqlite3_stmt* statement, const IntType& PK)
{
	sqlite3_bind_int(statement, 1, PK);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<TradingDay*>& records)
{
	TradingDay* record = TradingDay::Allocate();
	record->PK = sqlite3_column_int(statement, 0);
	Strcpy(record->CurrTradingDay, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->PreTradingDay, (const char*)sqlite3_column_text(statement, 2));
	records.push_back(record);
}
void SqliteDB::SetStatementForExchangeRecord(sqlite3_stmt* statement, Exchange* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->ExchangeName, sizeof(record->ExchangeName), nullptr);
}
void SqliteDB::SetStatementForExchangeRecordUpdate(sqlite3_stmt* statement, Exchange* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeName, sizeof(record->ExchangeName), nullptr);
	sqlite3_bind_text(statement, 2, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
}
void SqliteDB::SetStatementForExchangePrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Exchange*>& records)
{
	Exchange* record = Exchange::Allocate();
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->ExchangeName, (const char*)sqlite3_column_text(statement, 1));
	records.push_back(record);
}
void SqliteDB::SetStatementForProductRecord(sqlite3_stmt* statement, Product* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_text(statement, 3, record->ProductName, sizeof(record->ProductName), nullptr);
	sqlite3_bind_int(statement, 4, int(record->ProductClass));
	sqlite3_bind_int(statement, 5, record->VolumeMultiple);
	sqlite3_bind_double(statement, 6, record->PriceTick);
	sqlite3_bind_int(statement, 7, record->MaxMarketOrderVolume);
	sqlite3_bind_int(statement, 8, record->MinMarketOrderVolume);
	sqlite3_bind_int(statement, 9, record->MaxLimitOrderVolume);
	sqlite3_bind_int(statement, 10, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 11, record->SessionName, sizeof(record->SessionName), nullptr);
}
void SqliteDB::SetStatementForProductRecordUpdate(sqlite3_stmt* statement, Product* record)
{
	sqlite3_bind_text(statement, 1, record->ProductName, sizeof(record->ProductName), nullptr);
	sqlite3_bind_int(statement, 2, int(record->ProductClass));
	sqlite3_bind_int(statement, 3, record->VolumeMultiple);
	sqlite3_bind_double(statement, 4, record->PriceTick);
	sqlite3_bind_int(statement, 5, record->MaxMarketOrderVolume);
	sqlite3_bind_int(statement, 6, record->MinMarketOrderVolume);
	sqlite3_bind_int(statement, 7, record->MaxLimitOrderVolume);
	sqlite3_bind_int(statement, 8, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 9, record->SessionName, sizeof(record->SessionName), nullptr);
	sqlite3_bind_text(statement, 10, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 11, record->ProductID, sizeof(record->ProductID), nullptr);
}
void SqliteDB::SetStatementForProductPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, ProductID, sizeof(ProductID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Product*>& records)
{
	Product* record = Product::Allocate();
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->ProductID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->ProductName, (const char*)sqlite3_column_text(statement, 2));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 3));
	record->VolumeMultiple = sqlite3_column_int(statement, 4);
	record->PriceTick = sqlite3_column_double(statement, 5);
	record->MaxMarketOrderVolume = sqlite3_column_int(statement, 6);
	record->MinMarketOrderVolume = sqlite3_column_int(statement, 7);
	record->MaxLimitOrderVolume = sqlite3_column_int(statement, 8);
	record->MinLimitOrderVolume = sqlite3_column_int(statement, 9);
	Strcpy(record->SessionName, (const char*)sqlite3_column_text(statement, 10));
	records.push_back(record);
}
void SqliteDB::SetStatementForInstrumentRecord(sqlite3_stmt* statement, Instrument* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_text(statement, 3, record->InstrumentName, sizeof(record->InstrumentName), nullptr);
	sqlite3_bind_text(statement, 4, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_int(statement, 5, int(record->ProductClass));
	sqlite3_bind_int(statement, 6, record->VolumeMultiple);
	sqlite3_bind_double(statement, 7, record->PriceTick);
	sqlite3_bind_int(statement, 8, record->MaxMarketOrderVolume);
	sqlite3_bind_int(statement, 9, record->MinMarketOrderVolume);
	sqlite3_bind_int(statement, 10, record->MaxLimitOrderVolume);
	sqlite3_bind_int(statement, 11, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 12, record->SessionName, sizeof(record->SessionName), nullptr);
	sqlite3_bind_int(statement, 13, record->DeliveryYear);
	sqlite3_bind_int(statement, 14, record->DeliveryMonth);
}
void SqliteDB::SetStatementForInstrumentRecordUpdate(sqlite3_stmt* statement, Instrument* record)
{
	sqlite3_bind_text(statement, 1, record->InstrumentName, sizeof(record->InstrumentName), nullptr);
	sqlite3_bind_text(statement, 2, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->ProductClass));
	sqlite3_bind_int(statement, 4, record->VolumeMultiple);
	sqlite3_bind_double(statement, 5, record->PriceTick);
	sqlite3_bind_int(statement, 6, record->MaxMarketOrderVolume);
	sqlite3_bind_int(statement, 7, record->MinMarketOrderVolume);
	sqlite3_bind_int(statement, 8, record->MaxLimitOrderVolume);
	sqlite3_bind_int(statement, 9, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 10, record->SessionName, sizeof(record->SessionName), nullptr);
	sqlite3_bind_int(statement, 11, record->DeliveryYear);
	sqlite3_bind_int(statement, 12, record->DeliveryMonth);
	sqlite3_bind_text(statement, 13, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 14, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
}
void SqliteDB::SetStatementForInstrumentPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, InstrumentID, sizeof(InstrumentID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Instrument*>& records)
{
	Instrument* record = Instrument::Allocate();
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->InstrumentName, (const char*)sqlite3_column_text(statement, 2));
	Strcpy(record->ProductID, (const char*)sqlite3_column_text(statement, 3));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 4));
	record->VolumeMultiple = sqlite3_column_int(statement, 5);
	record->PriceTick = sqlite3_column_double(statement, 6);
	record->MaxMarketOrderVolume = sqlite3_column_int(statement, 7);
	record->MinMarketOrderVolume = sqlite3_column_int(statement, 8);
	record->MaxLimitOrderVolume = sqlite3_column_int(statement, 9);
	record->MinLimitOrderVolume = sqlite3_column_int(statement, 10);
	Strcpy(record->SessionName, (const char*)sqlite3_column_text(statement, 11));
	record->DeliveryYear = sqlite3_column_int(statement, 12);
	record->DeliveryMonth = sqlite3_column_int(statement, 13);
	records.push_back(record);
}
void SqliteDB::SetStatementForAccountRecord(sqlite3_stmt* statement, Account* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 3, record->AccountName, sizeof(record->AccountName), nullptr);
	sqlite3_bind_text(statement, 4, record->Password, sizeof(record->Password), nullptr);
	sqlite3_bind_double(statement, 5, record->PreBalance);
	sqlite3_bind_double(statement, 6, record->Balance);
	sqlite3_bind_double(statement, 7, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 8, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 9, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 10, record->PremiumIn);
	sqlite3_bind_double(statement, 11, record->PremiumOut);
	sqlite3_bind_double(statement, 12, record->MarketValue);
}
void SqliteDB::SetStatementForAccountRecordUpdate(sqlite3_stmt* statement, Account* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountName, sizeof(record->AccountName), nullptr);
	sqlite3_bind_text(statement, 3, record->Password, sizeof(record->Password), nullptr);
	sqlite3_bind_double(statement, 4, record->PreBalance);
	sqlite3_bind_double(statement, 5, record->Balance);
	sqlite3_bind_double(statement, 6, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 7, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 8, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 9, record->PremiumIn);
	sqlite3_bind_double(statement, 10, record->PremiumOut);
	sqlite3_bind_double(statement, 11, record->MarketValue);
	sqlite3_bind_text(statement, 12, record->AccountID, sizeof(record->AccountID), nullptr);
}
void SqliteDB::SetStatementForAccountPrimaryKey(sqlite3_stmt* statement, const AccountIDType& AccountID)
{
	sqlite3_bind_text(statement, 1, AccountID, sizeof(AccountID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Account*>& records)
{
	Account* record = Account::Allocate();
	Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->AccountName, (const char*)sqlite3_column_text(statement, 2));
	Strcpy(record->Password, (const char*)sqlite3_column_text(statement, 3));
	record->PreBalance = sqlite3_column_double(statement, 4);
	record->Balance = sqlite3_column_double(statement, 5);
	record->CloseProfitByDate = sqlite3_column_double(statement, 6);
	record->PositionProfitByDate = sqlite3_column_double(statement, 7);
	record->PositionProfitByTrade = sqlite3_column_double(statement, 8);
	record->PremiumIn = sqlite3_column_double(statement, 9);
	record->PremiumOut = sqlite3_column_double(statement, 10);
	record->MarketValue = sqlite3_column_double(statement, 11);
	records.push_back(record);
}
void SqliteDB::SetStatementForPositionRecord(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 3, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, int(record->ProductClass));
	sqlite3_bind_int(statement, 6, int(record->PosiDirection));
	sqlite3_bind_int(statement, 7, record->TotalPosition);
	sqlite3_bind_int(statement, 8, record->TodayPosition);
	sqlite3_bind_int(statement, 9, record->FrozenPosition);
	sqlite3_bind_double(statement, 10, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 11, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 12, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 13, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 14, record->PremiumIn);
	sqlite3_bind_double(statement, 15, record->PremiumOut);
	sqlite3_bind_double(statement, 16, record->MarketValue);
	sqlite3_bind_int(statement, 17, record->VolumeMultiple);
	sqlite3_bind_double(statement, 18, record->SettlementPrice);
	sqlite3_bind_double(statement, 19, record->PreSettlementPrice);
}
void SqliteDB::SetStatementForPositionRecordUpdate(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_int(statement, 1, int(record->ProductClass));
	sqlite3_bind_int(statement, 2, record->TotalPosition);
	sqlite3_bind_int(statement, 3, record->TodayPosition);
	sqlite3_bind_int(statement, 4, record->FrozenPosition);
	sqlite3_bind_double(statement, 5, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 6, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 7, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 8, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 9, record->PremiumIn);
	sqlite3_bind_double(statement, 10, record->PremiumOut);
	sqlite3_bind_double(statement, 11, record->MarketValue);
	sqlite3_bind_int(statement, 12, record->VolumeMultiple);
	sqlite3_bind_double(statement, 13, record->SettlementPrice);
	sqlite3_bind_double(statement, 14, record->PreSettlementPrice);
	sqlite3_bind_text(statement, 15, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 16, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 17, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 18, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 19, int(record->PosiDirection));
}
void SqliteDB::SetStatementForPositionPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
	sqlite3_bind_text(statement, 3, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, InstrumentID, sizeof(InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, int(PosiDirection));
}
void SqliteDB::SetStatementForPositionIndexAccount(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Position*>& records)
{
	Position* record = Position::Allocate();
	Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 2));
	Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 3));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 4));
	record->PosiDirection = PosiDirectionType(sqlite3_column_int(statement, 5));
	record->TotalPosition = sqlite3_column_int(statement, 6);
	record->TodayPosition = sqlite3_column_int(statement, 7);
	record->FrozenPosition = sqlite3_column_int(statement, 8);
	record->CloseProfitByDate = sqlite3_column_double(statement, 9);
	record->CloseProfitByTrade = sqlite3_column_double(statement, 10);
	record->PositionProfitByDate = sqlite3_column_double(statement, 11);
	record->PositionProfitByTrade = sqlite3_column_double(statement, 12);
	record->PremiumIn = sqlite3_column_double(statement, 13);
	record->PremiumOut = sqlite3_column_double(statement, 14);
	record->MarketValue = sqlite3_column_double(statement, 15);
	record->VolumeMultiple = sqlite3_column_int(statement, 16);
	record->SettlementPrice = sqlite3_column_double(statement, 17);
	record->PreSettlementPrice = sqlite3_column_double(statement, 18);
	records.push_back(record);
}
void SqliteDB::SetStatementForOrderRecord(sqlite3_stmt* statement, Order* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 3, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, record->OrderID);
	sqlite3_bind_int(statement, 6, record->ClientOrderID);
	sqlite3_bind_int(statement, 7, int(record->Direction));
	sqlite3_bind_int(statement, 8, int(record->OffsetFlag));
	sqlite3_bind_int(statement, 9, int(record->OrderPriceType));
	sqlite3_bind_double(statement, 10, record->Price);
	sqlite3_bind_int(statement, 11, record->Volume);
	sqlite3_bind_int(statement, 12, record->VolumeRemain);
	sqlite3_bind_int(statement, 13, record->VolumeTraded);
	sqlite3_bind_int(statement, 14, record->VolumeMultiple);
	sqlite3_bind_int(statement, 15, int(record->OrderStatus));
	sqlite3_bind_text(statement, 16, record->OrderDate, sizeof(record->OrderDate), nullptr);
	sqlite3_bind_text(statement, 17, record->OrderTime, sizeof(record->OrderTime), nullptr);
	sqlite3_bind_text(statement, 18, record->CancelDate, sizeof(record->CancelDate), nullptr);
	sqlite3_bind_text(statement, 19, record->CancelTime, sizeof(record->CancelTime), nullptr);
}
void SqliteDB::SetStatementForOrderRecordUpdate(sqlite3_stmt* statement, Order* record)
{
	sqlite3_bind_int(statement, 1, record->ClientOrderID);
	sqlite3_bind_int(statement, 2, int(record->Direction));
	sqlite3_bind_int(statement, 3, int(record->OffsetFlag));
	sqlite3_bind_int(statement, 4, int(record->OrderPriceType));
	sqlite3_bind_double(statement, 5, record->Price);
	sqlite3_bind_int(statement, 6, record->Volume);
	sqlite3_bind_int(statement, 7, record->VolumeRemain);
	sqlite3_bind_int(statement, 8, record->VolumeTraded);
	sqlite3_bind_int(statement, 9, record->VolumeMultiple);
	sqlite3_bind_int(statement, 10, int(record->OrderStatus));
	sqlite3_bind_text(statement, 11, record->OrderDate, sizeof(record->OrderDate), nullptr);
	sqlite3_bind_text(statement, 12, record->OrderTime, sizeof(record->OrderTime), nullptr);
	sqlite3_bind_text(statement, 13, record->CancelDate, sizeof(record->CancelDate), nullptr);
	sqlite3_bind_text(statement, 14, record->CancelTime, sizeof(record->CancelTime), nullptr);
	sqlite3_bind_text(statement, 15, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 16, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 17, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 18, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 19, record->OrderID);
}
void SqliteDB::SetStatementForOrderPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
	sqlite3_bind_text(statement, 3, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, InstrumentID, sizeof(InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, OrderID);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Order*>& records)
{
	Order* record = Order::Allocate();
	Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 2));
	Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 3));
	record->OrderID = sqlite3_column_int(statement, 4);
	record->ClientOrderID = sqlite3_column_int(statement, 5);
	record->Direction = DirectionType(sqlite3_column_int(statement, 6));
	record->OffsetFlag = OffsetFlagType(sqlite3_column_int(statement, 7));
	record->OrderPriceType = OrderPriceTypeType(sqlite3_column_int(statement, 8));
	record->Price = sqlite3_column_double(statement, 9);
	record->Volume = sqlite3_column_int(statement, 10);
	record->VolumeRemain = sqlite3_column_int(statement, 11);
	record->VolumeTraded = sqlite3_column_int(statement, 12);
	record->VolumeMultiple = sqlite3_column_int(statement, 13);
	record->OrderStatus = OrderStatusType(sqlite3_column_int(statement, 14));
	Strcpy(record->OrderDate, (const char*)sqlite3_column_text(statement, 15));
	Strcpy(record->OrderTime, (const char*)sqlite3_column_text(statement, 16));
	Strcpy(record->CancelDate, (const char*)sqlite3_column_text(statement, 17));
	Strcpy(record->CancelTime, (const char*)sqlite3_column_text(statement, 18));
	records.push_back(record);
}
void SqliteDB::SetStatementForTradeRecord(sqlite3_stmt* statement, Trade* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 3, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, record->OrderID);
	sqlite3_bind_text(statement, 6, record->TradeID, sizeof(record->TradeID), nullptr);
	sqlite3_bind_int(statement, 7, int(record->Direction));
	sqlite3_bind_int(statement, 8, int(record->OffsetFlag));
	sqlite3_bind_double(statement, 9, record->Price);
	sqlite3_bind_int(statement, 10, record->Volume);
	sqlite3_bind_double(statement, 11, record->TradeAmount);
	sqlite3_bind_double(statement, 12, record->PremiumIn);
	sqlite3_bind_double(statement, 13, record->PremiumOut);
	sqlite3_bind_text(statement, 14, record->TradeDate, sizeof(record->TradeDate), nullptr);
	sqlite3_bind_text(statement, 15, record->TradeTime, sizeof(record->TradeTime), nullptr);
}
void SqliteDB::SetStatementForTradeRecordUpdate(sqlite3_stmt* statement, Trade* record)
{
	sqlite3_bind_text(statement, 1, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 2, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 3, record->OrderID);
	sqlite3_bind_int(statement, 4, int(record->OffsetFlag));
	sqlite3_bind_double(statement, 5, record->Price);
	sqlite3_bind_int(statement, 6, record->Volume);
	sqlite3_bind_double(statement, 7, record->TradeAmount);
	sqlite3_bind_double(statement, 8, record->PremiumIn);
	sqlite3_bind_double(statement, 9, record->PremiumOut);
	sqlite3_bind_text(statement, 10, record->TradeDate, sizeof(record->TradeDate), nullptr);
	sqlite3_bind_text(statement, 11, record->TradeTime, sizeof(record->TradeTime), nullptr);
	sqlite3_bind_text(statement, 12, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 13, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 14, record->TradeID, sizeof(record->TradeID), nullptr);
	sqlite3_bind_int(statement, 15, int(record->Direction));
}
void SqliteDB::SetStatementForTradePrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 3, TradeID, sizeof(TradeID), nullptr);
	sqlite3_bind_int(statement, 4, int(Direction));
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<Trade*>& records)
{
	Trade* record = Trade::Allocate();
	Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 2));
	Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 3));
	record->OrderID = sqlite3_column_int(statement, 4);
	Strcpy(record->TradeID, (const char*)sqlite3_column_text(statement, 5));
	record->Direction = DirectionType(sqlite3_column_int(statement, 6));
	record->OffsetFlag = OffsetFlagType(sqlite3_column_int(statement, 7));
	record->Price = sqlite3_column_double(statement, 8);
	record->Volume = sqlite3_column_int(statement, 9);
	record->TradeAmount = sqlite3_column_double(statement, 10);
	record->PremiumIn = sqlite3_column_double(statement, 11);
	record->PremiumOut = sqlite3_column_double(statement, 12);
	Strcpy(record->TradeDate, (const char*)sqlite3_column_text(statement, 13));
	Strcpy(record->TradeTime, (const char*)sqlite3_column_text(statement, 14));
	records.push_back(record);
}
void SqliteDB::SetStatementForMdTickRecord(sqlite3_stmt* statement, MdTick* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 3, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_double(statement, 4, record->LastPrice);
	sqlite3_bind_double(statement, 5, record->PreSettlementPrice);
	sqlite3_bind_double(statement, 6, record->PreClosePrice);
	sqlite3_bind_int(statement, 7, record->PreOpenInterest);
	sqlite3_bind_double(statement, 8, record->OpenPrice);
	sqlite3_bind_double(statement, 9, record->HighestPrice);
	sqlite3_bind_double(statement, 10, record->LowestPrice);
	sqlite3_bind_int(statement, 11, record->Volume);
	sqlite3_bind_double(statement, 12, record->Turnover);
	sqlite3_bind_int(statement, 13, record->OpenInterest);
	sqlite3_bind_double(statement, 14, record->UpperLimitPrice);
	sqlite3_bind_double(statement, 15, record->LowerLimitPrice);
	sqlite3_bind_text(statement, 16, record->UpdateTime, sizeof(record->UpdateTime), nullptr);
	sqlite3_bind_int(statement, 17, record->UpdateMillisec);
	sqlite3_bind_double(statement, 18, record->AskPrice1);
	sqlite3_bind_double(statement, 19, record->AskPrice2);
	sqlite3_bind_double(statement, 20, record->AskPrice3);
	sqlite3_bind_double(statement, 21, record->AskPrice4);
	sqlite3_bind_double(statement, 22, record->AskPrice5);
	sqlite3_bind_int(statement, 23, record->AskVolume1);
	sqlite3_bind_int(statement, 24, record->AskVolume2);
	sqlite3_bind_int(statement, 25, record->AskVolume3);
	sqlite3_bind_int(statement, 26, record->AskVolume4);
	sqlite3_bind_int(statement, 27, record->AskVolume5);
	sqlite3_bind_double(statement, 28, record->BidPrice1);
	sqlite3_bind_double(statement, 29, record->BidPrice2);
	sqlite3_bind_double(statement, 30, record->BidPrice3);
	sqlite3_bind_double(statement, 31, record->BidPrice4);
	sqlite3_bind_double(statement, 32, record->BidPrice5);
	sqlite3_bind_int(statement, 33, record->BidVolume1);
	sqlite3_bind_int(statement, 34, record->BidVolume2);
	sqlite3_bind_int(statement, 35, record->BidVolume3);
	sqlite3_bind_int(statement, 36, record->BidVolume4);
	sqlite3_bind_int(statement, 37, record->BidVolume5);
	sqlite3_bind_double(statement, 38, record->AveragePrice);
}
void SqliteDB::SetStatementForMdTickRecordUpdate(sqlite3_stmt* statement, MdTick* record)
{
	sqlite3_bind_double(statement, 1, record->LastPrice);
	sqlite3_bind_double(statement, 2, record->PreSettlementPrice);
	sqlite3_bind_double(statement, 3, record->PreClosePrice);
	sqlite3_bind_int(statement, 4, record->PreOpenInterest);
	sqlite3_bind_double(statement, 5, record->OpenPrice);
	sqlite3_bind_double(statement, 6, record->HighestPrice);
	sqlite3_bind_double(statement, 7, record->LowestPrice);
	sqlite3_bind_int(statement, 8, record->Volume);
	sqlite3_bind_double(statement, 9, record->Turnover);
	sqlite3_bind_int(statement, 10, record->OpenInterest);
	sqlite3_bind_double(statement, 11, record->UpperLimitPrice);
	sqlite3_bind_double(statement, 12, record->LowerLimitPrice);
	sqlite3_bind_text(statement, 13, record->UpdateTime, sizeof(record->UpdateTime), nullptr);
	sqlite3_bind_int(statement, 14, record->UpdateMillisec);
	sqlite3_bind_double(statement, 15, record->AskPrice1);
	sqlite3_bind_double(statement, 16, record->AskPrice2);
	sqlite3_bind_double(statement, 17, record->AskPrice3);
	sqlite3_bind_double(statement, 18, record->AskPrice4);
	sqlite3_bind_double(statement, 19, record->AskPrice5);
	sqlite3_bind_int(statement, 20, record->AskVolume1);
	sqlite3_bind_int(statement, 21, record->AskVolume2);
	sqlite3_bind_int(statement, 22, record->AskVolume3);
	sqlite3_bind_int(statement, 23, record->AskVolume4);
	sqlite3_bind_int(statement, 24, record->AskVolume5);
	sqlite3_bind_double(statement, 25, record->BidPrice1);
	sqlite3_bind_double(statement, 26, record->BidPrice2);
	sqlite3_bind_double(statement, 27, record->BidPrice3);
	sqlite3_bind_double(statement, 28, record->BidPrice4);
	sqlite3_bind_double(statement, 29, record->BidPrice5);
	sqlite3_bind_int(statement, 30, record->BidVolume1);
	sqlite3_bind_int(statement, 31, record->BidVolume2);
	sqlite3_bind_int(statement, 32, record->BidVolume3);
	sqlite3_bind_int(statement, 33, record->BidVolume4);
	sqlite3_bind_int(statement, 34, record->BidVolume5);
	sqlite3_bind_double(statement, 35, record->AveragePrice);
	sqlite3_bind_text(statement, 36, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 37, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 38, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
}
void SqliteDB::SetStatementForMdTickPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 3, InstrumentID, sizeof(InstrumentID), nullptr);
}
void SqliteDB::ParseRecord(sqlite3_stmt* statement, std::vector<MdTick*>& records)
{
	MdTick* record = MdTick::Allocate();
	Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 1));
	Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 2));
	record->LastPrice = sqlite3_column_double(statement, 3);
	record->PreSettlementPrice = sqlite3_column_double(statement, 4);
	record->PreClosePrice = sqlite3_column_double(statement, 5);
	record->PreOpenInterest = sqlite3_column_int(statement, 6);
	record->OpenPrice = sqlite3_column_double(statement, 7);
	record->HighestPrice = sqlite3_column_double(statement, 8);
	record->LowestPrice = sqlite3_column_double(statement, 9);
	record->Volume = sqlite3_column_int(statement, 10);
	record->Turnover = sqlite3_column_double(statement, 11);
	record->OpenInterest = sqlite3_column_int(statement, 12);
	record->UpperLimitPrice = sqlite3_column_double(statement, 13);
	record->LowerLimitPrice = sqlite3_column_double(statement, 14);
	Strcpy(record->UpdateTime, (const char*)sqlite3_column_text(statement, 15));
	record->UpdateMillisec = sqlite3_column_int(statement, 16);
	record->AskPrice1 = sqlite3_column_double(statement, 17);
	record->AskPrice2 = sqlite3_column_double(statement, 18);
	record->AskPrice3 = sqlite3_column_double(statement, 19);
	record->AskPrice4 = sqlite3_column_double(statement, 20);
	record->AskPrice5 = sqlite3_column_double(statement, 21);
	record->AskVolume1 = sqlite3_column_int(statement, 22);
	record->AskVolume2 = sqlite3_column_int(statement, 23);
	record->AskVolume3 = sqlite3_column_int(statement, 24);
	record->AskVolume4 = sqlite3_column_int(statement, 25);
	record->AskVolume5 = sqlite3_column_int(statement, 26);
	record->BidPrice1 = sqlite3_column_double(statement, 27);
	record->BidPrice2 = sqlite3_column_double(statement, 28);
	record->BidPrice3 = sqlite3_column_double(statement, 29);
	record->BidPrice4 = sqlite3_column_double(statement, 30);
	record->BidPrice5 = sqlite3_column_double(statement, 31);
	record->BidVolume1 = sqlite3_column_int(statement, 32);
	record->BidVolume2 = sqlite3_column_int(statement, 33);
	record->BidVolume3 = sqlite3_column_int(statement, 34);
	record->BidVolume4 = sqlite3_column_int(statement, 35);
	record->BidVolume5 = sqlite3_column_int(statement, 36);
	record->AveragePrice = sqlite3_column_double(statement, 37);
	records.push_back(record);
}
