#pragma warning(disable: 4311)
#include "MysqlDB.h"
#include "Constant.h"
#include "Logger.h"
#include "Utility.h"
#include "TimeUtility.h"
#include <string.h>
#include <cstring>

using namespace mdb;
using namespace std;
using namespace std::chrono;

MysqlDB::MysqlDB(const std::string& host, const std::string& user, const std::string& passwd)
{
	m_Host = host;
	m_User = user;
	m_Passwd = passwd;
	m_SqlBuff = new char[BuffSize];

	m_Driver = sql::mysql::get_driver_instance();
	m_DBConnection = nullptr;
	m_Statement = nullptr;
	
	m_TradingDayCreateStatement = nullptr;
	m_TradingDayDropStatement = nullptr;
	m_TradingDayInsertStatement = nullptr;
	m_TradingDayDeleteStatement = nullptr;
	m_TradingDayUpdateStatement = nullptr;
	m_TradingDaySelectStatement = nullptr;
	m_TradingDayTruncateStatement = nullptr;

	m_ExchangeCreateStatement = nullptr;
	m_ExchangeDropStatement = nullptr;
	m_ExchangeInsertStatement = nullptr;
	m_ExchangeDeleteStatement = nullptr;
	m_ExchangeUpdateStatement = nullptr;
	m_ExchangeSelectStatement = nullptr;
	m_ExchangeTruncateStatement = nullptr;

	m_ProductCreateStatement = nullptr;
	m_ProductDropStatement = nullptr;
	m_ProductInsertStatement = nullptr;
	m_ProductDeleteStatement = nullptr;
	m_ProductUpdateStatement = nullptr;
	m_ProductSelectStatement = nullptr;
	m_ProductTruncateStatement = nullptr;

	m_InstrumentCreateStatement = nullptr;
	m_InstrumentDropStatement = nullptr;
	m_InstrumentInsertStatement = nullptr;
	m_InstrumentDeleteStatement = nullptr;
	m_InstrumentUpdateStatement = nullptr;
	m_InstrumentSelectStatement = nullptr;
	m_InstrumentTruncateStatement = nullptr;

	m_PrimaryAccountCreateStatement = nullptr;
	m_PrimaryAccountDropStatement = nullptr;
	m_PrimaryAccountInsertStatement = nullptr;
	m_PrimaryAccountDeleteStatement = nullptr;
	m_PrimaryAccountDeleteByOfferIDIndexStatement = nullptr;
	m_PrimaryAccountUpdateStatement = nullptr;
	m_PrimaryAccountSelectStatement = nullptr;
	m_PrimaryAccountTruncateStatement = nullptr;

	m_AccountCreateStatement = nullptr;
	m_AccountDropStatement = nullptr;
	m_AccountInsertStatement = nullptr;
	m_AccountDeleteStatement = nullptr;
	m_AccountUpdateStatement = nullptr;
	m_AccountSelectStatement = nullptr;
	m_AccountTruncateStatement = nullptr;

	m_CapitalCreateStatement = nullptr;
	m_CapitalDropStatement = nullptr;
	m_CapitalInsertStatement = nullptr;
	m_CapitalDeleteStatement = nullptr;
	m_CapitalDeleteByTradingDayIndexStatement = nullptr;
	m_CapitalUpdateStatement = nullptr;
	m_CapitalSelectStatement = nullptr;
	m_CapitalTruncateStatement = nullptr;

	m_PositionCreateStatement = nullptr;
	m_PositionDropStatement = nullptr;
	m_PositionInsertStatement = nullptr;
	m_PositionDeleteStatement = nullptr;
	m_PositionDeleteByAccountIndexStatement = nullptr;
	m_PositionDeleteByTradingDayIndexStatement = nullptr;
	m_PositionUpdateStatement = nullptr;
	m_PositionSelectStatement = nullptr;
	m_PositionTruncateStatement = nullptr;

	m_PositionDetailCreateStatement = nullptr;
	m_PositionDetailDropStatement = nullptr;
	m_PositionDetailInsertStatement = nullptr;
	m_PositionDetailDeleteStatement = nullptr;
	m_PositionDetailDeleteByTradeMatchIndexStatement = nullptr;
	m_PositionDetailDeleteByTradingDayIndexStatement = nullptr;
	m_PositionDetailUpdateStatement = nullptr;
	m_PositionDetailSelectStatement = nullptr;
	m_PositionDetailTruncateStatement = nullptr;

	m_OrderCreateStatement = nullptr;
	m_OrderDropStatement = nullptr;
	m_OrderInsertStatement = nullptr;
	m_OrderDeleteStatement = nullptr;
	m_OrderUpdateStatement = nullptr;
	m_OrderSelectStatement = nullptr;
	m_OrderTruncateStatement = nullptr;

	m_TradeCreateStatement = nullptr;
	m_TradeDropStatement = nullptr;
	m_TradeInsertStatement = nullptr;
	m_TradeDeleteStatement = nullptr;
	m_TradeUpdateStatement = nullptr;
	m_TradeSelectStatement = nullptr;
	m_TradeTruncateStatement = nullptr;

}
MysqlDB::~MysqlDB()
{
	delete[] m_SqlBuff;
	DisConnect();
	if (m_DBConnection != nullptr)
	{
		m_DBConnection->close();
	}
}
bool MysqlDB::Connect()
{
	try
	{
		if (m_Driver == nullptr)
		{
			m_Driver = sql::mysql::get_driver_instance();
		}
		if (m_DBConnection == nullptr)
		{
			m_DBConnection = m_Driver->connect(m_Host, m_User, m_Passwd);
			if (m_DBConnection == nullptr)
			{
				WriteLog(LogLevel::Info, "MysqlDB Connect Failed");
				return false;
			}
		}
		else
		{
			auto result = m_DBConnection->reconnect();
			WriteLog(LogLevel::Info, "MysqlDB Reconnect Result:[%d]", result);
			if (!result)
			{
				m_Driver = nullptr;
				m_DBConnection = nullptr;
				return false;
			}
		}
		m_Statement = m_DBConnection->createStatement();
	}
	catch (std::exception e)
	{
		WriteLog(LogLevel::Warning, "Connect MysqlDB Failed. Msg:%s", e.what());
		return false;
	}
	return true;
}
void MysqlDB::DisConnect()
{
	if (m_Statement != nullptr)
	{
		m_Statement->close();
		m_Statement = nullptr;
	}
	if (m_TradingDayCreateStatement != nullptr)
	{
		m_TradingDayCreateStatement->close();
		m_TradingDayCreateStatement = nullptr;
	}
	if (m_TradingDayDropStatement != nullptr)
	{
		m_TradingDayDropStatement->close();
		m_TradingDayDropStatement = nullptr;
	}
	if (m_TradingDayInsertStatement != nullptr)
	{
		m_TradingDayInsertStatement->close();
		m_TradingDayInsertStatement = nullptr;
	}
	if (m_TradingDayDeleteStatement != nullptr)
	{
		m_TradingDayDeleteStatement->close();
		m_TradingDayDeleteStatement = nullptr;
	}
	if (m_TradingDayUpdateStatement != nullptr)
	{
		m_TradingDayUpdateStatement->close();
		m_TradingDayUpdateStatement = nullptr;
	}
	if (m_TradingDaySelectStatement != nullptr)
	{
		m_TradingDaySelectStatement->close();
		m_TradingDaySelectStatement = nullptr;
	}
	if (m_TradingDayTruncateStatement != nullptr)
	{
		m_TradingDayTruncateStatement->close();
		m_TradingDayTruncateStatement = nullptr;
	}
	if (m_ExchangeCreateStatement != nullptr)
	{
		m_ExchangeCreateStatement->close();
		m_ExchangeCreateStatement = nullptr;
	}
	if (m_ExchangeDropStatement != nullptr)
	{
		m_ExchangeDropStatement->close();
		m_ExchangeDropStatement = nullptr;
	}
	if (m_ExchangeInsertStatement != nullptr)
	{
		m_ExchangeInsertStatement->close();
		m_ExchangeInsertStatement = nullptr;
	}
	if (m_ExchangeDeleteStatement != nullptr)
	{
		m_ExchangeDeleteStatement->close();
		m_ExchangeDeleteStatement = nullptr;
	}
	if (m_ExchangeUpdateStatement != nullptr)
	{
		m_ExchangeUpdateStatement->close();
		m_ExchangeUpdateStatement = nullptr;
	}
	if (m_ExchangeSelectStatement != nullptr)
	{
		m_ExchangeSelectStatement->close();
		m_ExchangeSelectStatement = nullptr;
	}
	if (m_ExchangeTruncateStatement != nullptr)
	{
		m_ExchangeTruncateStatement->close();
		m_ExchangeTruncateStatement = nullptr;
	}
	if (m_ProductCreateStatement != nullptr)
	{
		m_ProductCreateStatement->close();
		m_ProductCreateStatement = nullptr;
	}
	if (m_ProductDropStatement != nullptr)
	{
		m_ProductDropStatement->close();
		m_ProductDropStatement = nullptr;
	}
	if (m_ProductInsertStatement != nullptr)
	{
		m_ProductInsertStatement->close();
		m_ProductInsertStatement = nullptr;
	}
	if (m_ProductDeleteStatement != nullptr)
	{
		m_ProductDeleteStatement->close();
		m_ProductDeleteStatement = nullptr;
	}
	if (m_ProductUpdateStatement != nullptr)
	{
		m_ProductUpdateStatement->close();
		m_ProductUpdateStatement = nullptr;
	}
	if (m_ProductSelectStatement != nullptr)
	{
		m_ProductSelectStatement->close();
		m_ProductSelectStatement = nullptr;
	}
	if (m_ProductTruncateStatement != nullptr)
	{
		m_ProductTruncateStatement->close();
		m_ProductTruncateStatement = nullptr;
	}
	if (m_InstrumentCreateStatement != nullptr)
	{
		m_InstrumentCreateStatement->close();
		m_InstrumentCreateStatement = nullptr;
	}
	if (m_InstrumentDropStatement != nullptr)
	{
		m_InstrumentDropStatement->close();
		m_InstrumentDropStatement = nullptr;
	}
	if (m_InstrumentInsertStatement != nullptr)
	{
		m_InstrumentInsertStatement->close();
		m_InstrumentInsertStatement = nullptr;
	}
	if (m_InstrumentDeleteStatement != nullptr)
	{
		m_InstrumentDeleteStatement->close();
		m_InstrumentDeleteStatement = nullptr;
	}
	if (m_InstrumentUpdateStatement != nullptr)
	{
		m_InstrumentUpdateStatement->close();
		m_InstrumentUpdateStatement = nullptr;
	}
	if (m_InstrumentSelectStatement != nullptr)
	{
		m_InstrumentSelectStatement->close();
		m_InstrumentSelectStatement = nullptr;
	}
	if (m_InstrumentTruncateStatement != nullptr)
	{
		m_InstrumentTruncateStatement->close();
		m_InstrumentTruncateStatement = nullptr;
	}
	if (m_PrimaryAccountCreateStatement != nullptr)
	{
		m_PrimaryAccountCreateStatement->close();
		m_PrimaryAccountCreateStatement = nullptr;
	}
	if (m_PrimaryAccountDropStatement != nullptr)
	{
		m_PrimaryAccountDropStatement->close();
		m_PrimaryAccountDropStatement = nullptr;
	}
	if (m_PrimaryAccountInsertStatement != nullptr)
	{
		m_PrimaryAccountInsertStatement->close();
		m_PrimaryAccountInsertStatement = nullptr;
	}
	if (m_PrimaryAccountDeleteStatement != nullptr)
	{
		m_PrimaryAccountDeleteStatement->close();
		m_PrimaryAccountDeleteStatement = nullptr;
	}
	if (m_PrimaryAccountDeleteByOfferIDIndexStatement != nullptr)
	{
		m_PrimaryAccountDeleteByOfferIDIndexStatement->close();
		m_PrimaryAccountDeleteByOfferIDIndexStatement = nullptr;
	}
	if (m_PrimaryAccountUpdateStatement != nullptr)
	{
		m_PrimaryAccountUpdateStatement->close();
		m_PrimaryAccountUpdateStatement = nullptr;
	}
	if (m_PrimaryAccountSelectStatement != nullptr)
	{
		m_PrimaryAccountSelectStatement->close();
		m_PrimaryAccountSelectStatement = nullptr;
	}
	if (m_PrimaryAccountTruncateStatement != nullptr)
	{
		m_PrimaryAccountTruncateStatement->close();
		m_PrimaryAccountTruncateStatement = nullptr;
	}
	if (m_AccountCreateStatement != nullptr)
	{
		m_AccountCreateStatement->close();
		m_AccountCreateStatement = nullptr;
	}
	if (m_AccountDropStatement != nullptr)
	{
		m_AccountDropStatement->close();
		m_AccountDropStatement = nullptr;
	}
	if (m_AccountInsertStatement != nullptr)
	{
		m_AccountInsertStatement->close();
		m_AccountInsertStatement = nullptr;
	}
	if (m_AccountDeleteStatement != nullptr)
	{
		m_AccountDeleteStatement->close();
		m_AccountDeleteStatement = nullptr;
	}
	if (m_AccountUpdateStatement != nullptr)
	{
		m_AccountUpdateStatement->close();
		m_AccountUpdateStatement = nullptr;
	}
	if (m_AccountSelectStatement != nullptr)
	{
		m_AccountSelectStatement->close();
		m_AccountSelectStatement = nullptr;
	}
	if (m_AccountTruncateStatement != nullptr)
	{
		m_AccountTruncateStatement->close();
		m_AccountTruncateStatement = nullptr;
	}
	if (m_CapitalCreateStatement != nullptr)
	{
		m_CapitalCreateStatement->close();
		m_CapitalCreateStatement = nullptr;
	}
	if (m_CapitalDropStatement != nullptr)
	{
		m_CapitalDropStatement->close();
		m_CapitalDropStatement = nullptr;
	}
	if (m_CapitalInsertStatement != nullptr)
	{
		m_CapitalInsertStatement->close();
		m_CapitalInsertStatement = nullptr;
	}
	if (m_CapitalDeleteStatement != nullptr)
	{
		m_CapitalDeleteStatement->close();
		m_CapitalDeleteStatement = nullptr;
	}
	if (m_CapitalDeleteByTradingDayIndexStatement != nullptr)
	{
		m_CapitalDeleteByTradingDayIndexStatement->close();
		m_CapitalDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_CapitalUpdateStatement != nullptr)
	{
		m_CapitalUpdateStatement->close();
		m_CapitalUpdateStatement = nullptr;
	}
	if (m_CapitalSelectStatement != nullptr)
	{
		m_CapitalSelectStatement->close();
		m_CapitalSelectStatement = nullptr;
	}
	if (m_CapitalTruncateStatement != nullptr)
	{
		m_CapitalTruncateStatement->close();
		m_CapitalTruncateStatement = nullptr;
	}
	if (m_PositionCreateStatement != nullptr)
	{
		m_PositionCreateStatement->close();
		m_PositionCreateStatement = nullptr;
	}
	if (m_PositionDropStatement != nullptr)
	{
		m_PositionDropStatement->close();
		m_PositionDropStatement = nullptr;
	}
	if (m_PositionInsertStatement != nullptr)
	{
		m_PositionInsertStatement->close();
		m_PositionInsertStatement = nullptr;
	}
	if (m_PositionDeleteStatement != nullptr)
	{
		m_PositionDeleteStatement->close();
		m_PositionDeleteStatement = nullptr;
	}
	if (m_PositionDeleteByAccountIndexStatement != nullptr)
	{
		m_PositionDeleteByAccountIndexStatement->close();
		m_PositionDeleteByAccountIndexStatement = nullptr;
	}
	if (m_PositionDeleteByTradingDayIndexStatement != nullptr)
	{
		m_PositionDeleteByTradingDayIndexStatement->close();
		m_PositionDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_PositionUpdateStatement != nullptr)
	{
		m_PositionUpdateStatement->close();
		m_PositionUpdateStatement = nullptr;
	}
	if (m_PositionSelectStatement != nullptr)
	{
		m_PositionSelectStatement->close();
		m_PositionSelectStatement = nullptr;
	}
	if (m_PositionTruncateStatement != nullptr)
	{
		m_PositionTruncateStatement->close();
		m_PositionTruncateStatement = nullptr;
	}
	if (m_PositionDetailCreateStatement != nullptr)
	{
		m_PositionDetailCreateStatement->close();
		m_PositionDetailCreateStatement = nullptr;
	}
	if (m_PositionDetailDropStatement != nullptr)
	{
		m_PositionDetailDropStatement->close();
		m_PositionDetailDropStatement = nullptr;
	}
	if (m_PositionDetailInsertStatement != nullptr)
	{
		m_PositionDetailInsertStatement->close();
		m_PositionDetailInsertStatement = nullptr;
	}
	if (m_PositionDetailDeleteStatement != nullptr)
	{
		m_PositionDetailDeleteStatement->close();
		m_PositionDetailDeleteStatement = nullptr;
	}
	if (m_PositionDetailDeleteByTradeMatchIndexStatement != nullptr)
	{
		m_PositionDetailDeleteByTradeMatchIndexStatement->close();
		m_PositionDetailDeleteByTradeMatchIndexStatement = nullptr;
	}
	if (m_PositionDetailDeleteByTradingDayIndexStatement != nullptr)
	{
		m_PositionDetailDeleteByTradingDayIndexStatement->close();
		m_PositionDetailDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_PositionDetailUpdateStatement != nullptr)
	{
		m_PositionDetailUpdateStatement->close();
		m_PositionDetailUpdateStatement = nullptr;
	}
	if (m_PositionDetailSelectStatement != nullptr)
	{
		m_PositionDetailSelectStatement->close();
		m_PositionDetailSelectStatement = nullptr;
	}
	if (m_PositionDetailTruncateStatement != nullptr)
	{
		m_PositionDetailTruncateStatement->close();
		m_PositionDetailTruncateStatement = nullptr;
	}
	if (m_OrderCreateStatement != nullptr)
	{
		m_OrderCreateStatement->close();
		m_OrderCreateStatement = nullptr;
	}
	if (m_OrderDropStatement != nullptr)
	{
		m_OrderDropStatement->close();
		m_OrderDropStatement = nullptr;
	}
	if (m_OrderInsertStatement != nullptr)
	{
		m_OrderInsertStatement->close();
		m_OrderInsertStatement = nullptr;
	}
	if (m_OrderDeleteStatement != nullptr)
	{
		m_OrderDeleteStatement->close();
		m_OrderDeleteStatement = nullptr;
	}
	if (m_OrderUpdateStatement != nullptr)
	{
		m_OrderUpdateStatement->close();
		m_OrderUpdateStatement = nullptr;
	}
	if (m_OrderSelectStatement != nullptr)
	{
		m_OrderSelectStatement->close();
		m_OrderSelectStatement = nullptr;
	}
	if (m_OrderTruncateStatement != nullptr)
	{
		m_OrderTruncateStatement->close();
		m_OrderTruncateStatement = nullptr;
	}
	if (m_TradeCreateStatement != nullptr)
	{
		m_TradeCreateStatement->close();
		m_TradeCreateStatement = nullptr;
	}
	if (m_TradeDropStatement != nullptr)
	{
		m_TradeDropStatement->close();
		m_TradeDropStatement = nullptr;
	}
	if (m_TradeInsertStatement != nullptr)
	{
		m_TradeInsertStatement->close();
		m_TradeInsertStatement = nullptr;
	}
	if (m_TradeDeleteStatement != nullptr)
	{
		m_TradeDeleteStatement->close();
		m_TradeDeleteStatement = nullptr;
	}
	if (m_TradeUpdateStatement != nullptr)
	{
		m_TradeUpdateStatement->close();
		m_TradeUpdateStatement = nullptr;
	}
	if (m_TradeSelectStatement != nullptr)
	{
		m_TradeSelectStatement->close();
		m_TradeSelectStatement = nullptr;
	}
	if (m_TradeTruncateStatement != nullptr)
	{
		m_TradeTruncateStatement->close();
		m_TradeTruncateStatement = nullptr;
	}
}
void MysqlDB::InitDB()
{
	m_Statement->executeUpdate("Truncate Table t_TradingDay;");
	m_Statement->executeUpdate("Insert Into t_TradingDay select * from Init.t_TradingDay;");
	m_Statement->executeUpdate("Truncate Table t_Exchange;");
	m_Statement->executeUpdate("Insert Into t_Exchange select * from Init.t_Exchange;");
	m_Statement->executeUpdate("Truncate Table t_Product;");
	m_Statement->executeUpdate("Insert Into t_Product select * from Init.t_Product;");
	m_Statement->executeUpdate("Truncate Table t_Instrument;");
	m_Statement->executeUpdate("Insert Into t_Instrument select * from Init.t_Instrument;");
	m_Statement->executeUpdate("Truncate Table t_PrimaryAccount;");
	m_Statement->executeUpdate("Insert Into t_PrimaryAccount select * from Init.t_PrimaryAccount;");
	m_Statement->executeUpdate("Truncate Table t_Account;");
	m_Statement->executeUpdate("Insert Into t_Account select * from Init.t_Account;");
	m_Statement->executeUpdate("Truncate Table t_Capital;");
	m_Statement->executeUpdate("Insert Into t_Capital select * from Init.t_Capital;");
	m_Statement->executeUpdate("Truncate Table t_Position;");
	m_Statement->executeUpdate("Insert Into t_Position select * from Init.t_Position;");
	m_Statement->executeUpdate("Truncate Table t_PositionDetail;");
	m_Statement->executeUpdate("Insert Into t_PositionDetail select * from Init.t_PositionDetail;");
	m_Statement->executeUpdate("Truncate Table t_Order;");
	m_Statement->executeUpdate("Insert Into t_Order select * from Init.t_Order;");
	m_Statement->executeUpdate("Truncate Table t_Trade;");
	m_Statement->executeUpdate("Insert Into t_Trade select * from Init.t_Trade;");
}
void MysqlDB::CreateTables()
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
void MysqlDB::DropTables()
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
void MysqlDB::TruncateTables()
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
void MysqlDB::TruncateSessionTables()
{
	auto start = steady_clock::now();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}

void MysqlDB::CreateTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayCreateStatement == nullptr)
	{
		m_TradingDayCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_TradingDay(`PK` int, `CurrTradingDay` char(9), `PreTradingDay` char(9), PRIMARY KEY(PK)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_TradingDayCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTradingDay Spend:%lldms", duration);
}
void MysqlDB::DropTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayDropStatement == nullptr)
	{
		m_TradingDayDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_TradingDay;");
	}
	m_TradingDayDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTradingDay Spend:%lldms", duration);
}
void MysqlDB::InsertTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayInsertStatement == nullptr)
	{
		m_TradingDayInsertStatement = m_DBConnection->prepareStatement("insert into t_TradingDay Values(?, ?, ?);");
	}
	SetStatementForTradingDayRecord(m_TradingDayInsertStatement, record);
	
	m_TradingDayInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayDeleteStatement == nullptr)
	{
		m_TradingDayDeleteStatement = m_DBConnection->prepareStatement("delete from t_TradingDay where PK = ?;");
	}
	SetStatementForTradingDayPrimaryKey(m_TradingDayDeleteStatement, record->PK);
	m_TradingDayDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayUpdateStatement == nullptr)
	{
		m_TradingDayUpdateStatement = m_DBConnection->prepareStatement("update t_TradingDay set CurrTradingDay = ?, PreTradingDay = ? where PK = ?;");
	}
	SetStatementForTradingDayRecordUpdate(m_TradingDayUpdateStatement, record);
	m_TradingDayUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void MysqlDB::SelectTradingDay(std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	if (m_TradingDaySelectStatement == nullptr)
	{
		m_TradingDaySelectStatement = m_DBConnection->prepareStatement("select * from t_TradingDay;");
	}
	auto result = m_TradingDaySelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayTruncateStatement == nullptr)
	{
		m_TradingDayTruncateStatement = m_DBConnection->prepareStatement("truncate table t_TradingDay;");
	}
	m_TradingDayTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeCreateStatement == nullptr)
	{
		m_ExchangeCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Exchange(`ExchangeID` char(8), `ExchangeName` char(64), PRIMARY KEY(ExchangeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_ExchangeCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateExchange Spend:%lldms", duration);
}
void MysqlDB::DropExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeDropStatement == nullptr)
	{
		m_ExchangeDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Exchange;");
	}
	m_ExchangeDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropExchange Spend:%lldms", duration);
}
void MysqlDB::InsertExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeInsertStatement == nullptr)
	{
		m_ExchangeInsertStatement = m_DBConnection->prepareStatement("insert into t_Exchange Values(?, ?);");
	}
	SetStatementForExchangeRecord(m_ExchangeInsertStatement, record);
	
	m_ExchangeInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertExchange Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertExchange(std::list<Exchange*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Exchange Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Exchange Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeDeleteStatement == nullptr)
	{
		m_ExchangeDeleteStatement = m_DBConnection->prepareStatement("delete from t_Exchange where ExchangeID = ?;");
	}
	SetStatementForExchangePrimaryKey(m_ExchangeDeleteStatement, record->ExchangeID);
	m_ExchangeDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeUpdateStatement == nullptr)
	{
		m_ExchangeUpdateStatement = m_DBConnection->prepareStatement("update t_Exchange set ExchangeName = ? where ExchangeID = ?;");
	}
	SetStatementForExchangeRecordUpdate(m_ExchangeUpdateStatement, record);
	m_ExchangeUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void MysqlDB::SelectExchange(std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	if (m_ExchangeSelectStatement == nullptr)
	{
		m_ExchangeSelectStatement = m_DBConnection->prepareStatement("select * from t_Exchange;");
	}
	auto result = m_ExchangeSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeTruncateStatement == nullptr)
	{
		m_ExchangeTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Exchange;");
	}
	m_ExchangeTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateProduct()
{
	auto start = steady_clock::now();
	if (m_ProductCreateStatement == nullptr)
	{
		m_ProductCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Product(`ExchangeID` char(8), `ProductID` char(32), `ProductName` char(32), `ProductClass` int, `VolumeMultiple` int, `PriceTick` decimal(24,8), `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` char(32), PRIMARY KEY(ExchangeID, ProductID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_ProductCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateProduct Spend:%lldms", duration);
}
void MysqlDB::DropProduct()
{
	auto start = steady_clock::now();
	if (m_ProductDropStatement == nullptr)
	{
		m_ProductDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Product;");
	}
	m_ProductDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropProduct Spend:%lldms", duration);
}
void MysqlDB::InsertProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductInsertStatement == nullptr)
	{
		m_ProductInsertStatement = m_DBConnection->prepareStatement("insert into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForProductRecord(m_ProductInsertStatement, record);
	
	m_ProductInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertProduct Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertProduct(std::list<Product*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Product Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Product Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductDeleteStatement == nullptr)
	{
		m_ProductDeleteStatement = m_DBConnection->prepareStatement("delete from t_Product where ExchangeID = ? and ProductID = ?;");
	}
	SetStatementForProductPrimaryKey(m_ProductDeleteStatement, record->ExchangeID, record->ProductID);
	m_ProductDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductUpdateStatement == nullptr)
	{
		m_ProductUpdateStatement = m_DBConnection->prepareStatement("update t_Product set ProductName = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and ProductID = ?;");
	}
	SetStatementForProductRecordUpdate(m_ProductUpdateStatement, record);
	m_ProductUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void MysqlDB::SelectProduct(std::list<Product*>& records)
{
	auto start = steady_clock::now();
	if (m_ProductSelectStatement == nullptr)
	{
		m_ProductSelectStatement = m_DBConnection->prepareStatement("select * from t_Product;");
	}
	auto result = m_ProductSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateProduct()
{
	auto start = steady_clock::now();
	if (m_ProductTruncateStatement == nullptr)
	{
		m_ProductTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Product;");
	}
	m_ProductTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentCreateStatement == nullptr)
	{
		m_InstrumentCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Instrument(`ExchangeID` char(8), `InstrumentID` char(32), `ExchangeInstID` char(32), `InstrumentName` char(64), `ProductID` char(32), `ProductClass` int, `InstrumentClass` int, `Rank` int, `VolumeMultiple` int, `PriceTick` decimal(24,8), `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` char(32), PRIMARY KEY(ExchangeID, InstrumentID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_InstrumentCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateInstrument Spend:%lldms", duration);
}
void MysqlDB::DropInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentDropStatement == nullptr)
	{
		m_InstrumentDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Instrument;");
	}
	m_InstrumentDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropInstrument Spend:%lldms", duration);
}
void MysqlDB::InsertInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentInsertStatement == nullptr)
	{
		m_InstrumentInsertStatement = m_DBConnection->prepareStatement("insert into t_Instrument Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForInstrumentRecord(m_InstrumentInsertStatement, record);
	
	m_InstrumentInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertInstrument(std::list<Instrument*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Instrument Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Instrument Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentDeleteStatement == nullptr)
	{
		m_InstrumentDeleteStatement = m_DBConnection->prepareStatement("delete from t_Instrument where ExchangeID = ? and InstrumentID = ?;");
	}
	SetStatementForInstrumentPrimaryKey(m_InstrumentDeleteStatement, record->ExchangeID, record->InstrumentID);
	m_InstrumentDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentUpdateStatement == nullptr)
	{
		m_InstrumentUpdateStatement = m_DBConnection->prepareStatement("update t_Instrument set ExchangeInstID = ?, InstrumentName = ?, ProductID = ?, ProductClass = ?, InstrumentClass = ?, Rank = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and InstrumentID = ?;");
	}
	SetStatementForInstrumentRecordUpdate(m_InstrumentUpdateStatement, record);
	m_InstrumentUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void MysqlDB::SelectInstrument(std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	if (m_InstrumentSelectStatement == nullptr)
	{
		m_InstrumentSelectStatement = m_DBConnection->prepareStatement("select * from t_Instrument;");
	}
	auto result = m_InstrumentSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentTruncateStatement == nullptr)
	{
		m_InstrumentTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Instrument;");
	}
	m_InstrumentTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreatePrimaryAccount()
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountCreateStatement == nullptr)
	{
		m_PrimaryAccountCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_PrimaryAccount(`PrimaryAccountID` char(32), `PrimaryAccountName` char(64), `AccountClass` int, `BrokerPassword` char(64), `OfferID` int, `IsAllowLogin` bool, `IsSimulateAccount` bool, `LoginStatus` int, `InitStatus` int, INDEX PrimaryAccountOfferID(OfferID), PRIMARY KEY(PrimaryAccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_PrimaryAccountCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePrimaryAccount Spend:%lldms", duration);
}
void MysqlDB::DropPrimaryAccount()
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountDropStatement == nullptr)
	{
		m_PrimaryAccountDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_PrimaryAccount;");
	}
	m_PrimaryAccountDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPrimaryAccount Spend:%lldms", duration);
}
void MysqlDB::InsertPrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountInsertStatement == nullptr)
	{
		m_PrimaryAccountInsertStatement = m_DBConnection->prepareStatement("insert into t_PrimaryAccount Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForPrimaryAccountRecord(m_PrimaryAccountInsertStatement, record);
	
	m_PrimaryAccountInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPrimaryAccount Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertPrimaryAccount(std::list<PrimaryAccount*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_PrimaryAccount Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_PrimaryAccount Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeletePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountDeleteStatement == nullptr)
	{
		m_PrimaryAccountDeleteStatement = m_DBConnection->prepareStatement("delete from t_PrimaryAccount where PrimaryAccountID = ?;");
	}
	SetStatementForPrimaryAccountPrimaryKey(m_PrimaryAccountDeleteStatement, record->PrimaryAccountID);
	m_PrimaryAccountDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccount Spend:%lldms", duration);
	}
}
void MysqlDB::DeletePrimaryAccountByOfferIDIndex(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountDeleteByOfferIDIndexStatement == nullptr)
	{
		m_PrimaryAccountDeleteByOfferIDIndexStatement = m_DBConnection->prepareStatement("delete from t_PrimaryAccount where OfferID = ?;");
	}
	SetStatementForPrimaryAccountIndexOfferID(m_PrimaryAccountDeleteByOfferIDIndexStatement, record);
	m_PrimaryAccountDeleteByOfferIDIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccountByOfferIDIndex Spend:%lldms", duration);
	}
}
void MysqlDB::UpdatePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountUpdateStatement == nullptr)
	{
		m_PrimaryAccountUpdateStatement = m_DBConnection->prepareStatement("update t_PrimaryAccount set PrimaryAccountName = ?, AccountClass = ?, BrokerPassword = ?, OfferID = ?, IsAllowLogin = ?, IsSimulateAccount = ?, LoginStatus = ?, InitStatus = ? where PrimaryAccountID = ?;");
	}
	SetStatementForPrimaryAccountRecordUpdate(m_PrimaryAccountUpdateStatement, record);
	m_PrimaryAccountUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePrimaryAccount Spend:%lldms", duration);
	}
}
void MysqlDB::SelectPrimaryAccount(std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountSelectStatement == nullptr)
	{
		m_PrimaryAccountSelectStatement = m_DBConnection->prepareStatement("select * from t_PrimaryAccount;");
	}
	auto result = m_PrimaryAccountSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccount Spend:%lldms", duration);
	}
}
void MysqlDB::TruncatePrimaryAccount()
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountTruncateStatement == nullptr)
	{
		m_PrimaryAccountTruncateStatement = m_DBConnection->prepareStatement("truncate table t_PrimaryAccount;");
	}
	m_PrimaryAccountTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePrimaryAccount Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateAccount()
{
	auto start = steady_clock::now();
	if (m_AccountCreateStatement == nullptr)
	{
		m_AccountCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Account(`AccountID` char(32), `AccountName` char(64), `AccountType` int, `AccountStatus` int, `Password` char(64), `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, PRIMARY KEY(AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_AccountCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateAccount Spend:%lldms", duration);
}
void MysqlDB::DropAccount()
{
	auto start = steady_clock::now();
	if (m_AccountDropStatement == nullptr)
	{
		m_AccountDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Account;");
	}
	m_AccountDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropAccount Spend:%lldms", duration);
}
void MysqlDB::InsertAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountInsertStatement == nullptr)
	{
		m_AccountInsertStatement = m_DBConnection->prepareStatement("insert into t_Account Values(?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForAccountRecord(m_AccountInsertStatement, record);
	
	m_AccountInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertAccount Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertAccount(std::list<Account*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Account Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Account Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountDeleteStatement == nullptr)
	{
		m_AccountDeleteStatement = m_DBConnection->prepareStatement("delete from t_Account where AccountID = ?;");
	}
	SetStatementForAccountPrimaryKey(m_AccountDeleteStatement, record->AccountID);
	m_AccountDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountUpdateStatement == nullptr)
	{
		m_AccountUpdateStatement = m_DBConnection->prepareStatement("update t_Account set AccountName = ?, AccountType = ?, AccountStatus = ?, Password = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ? where AccountID = ?;");
	}
	SetStatementForAccountRecordUpdate(m_AccountUpdateStatement, record);
	m_AccountUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void MysqlDB::SelectAccount(std::list<Account*>& records)
{
	auto start = steady_clock::now();
	if (m_AccountSelectStatement == nullptr)
	{
		m_AccountSelectStatement = m_DBConnection->prepareStatement("select * from t_Account;");
	}
	auto result = m_AccountSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateAccount()
{
	auto start = steady_clock::now();
	if (m_AccountTruncateStatement == nullptr)
	{
		m_AccountTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Account;");
	}
	m_AccountTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateCapital()
{
	auto start = steady_clock::now();
	if (m_CapitalCreateStatement == nullptr)
	{
		m_CapitalCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Capital(`TradingDay` char(9), `AccountID` char(32), `AccountType` int, `Balance` decimal(24,8), `PreBalance` decimal(24,8), `Available` decimal(24,8), `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `Deposit` decimal(24,8), `Withdraw` decimal(24,8), INDEX CapitalTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_CapitalCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateCapital Spend:%lldms", duration);
}
void MysqlDB::DropCapital()
{
	auto start = steady_clock::now();
	if (m_CapitalDropStatement == nullptr)
	{
		m_CapitalDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Capital;");
	}
	m_CapitalDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropCapital Spend:%lldms", duration);
}
void MysqlDB::InsertCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalInsertStatement == nullptr)
	{
		m_CapitalInsertStatement = m_DBConnection->prepareStatement("insert into t_Capital Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForCapitalRecord(m_CapitalInsertStatement, record);
	
	m_CapitalInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertCapital Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertCapital(std::list<Capital*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Capital Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertCapital Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Capital Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertCapital Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertCapital RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalDeleteStatement == nullptr)
	{
		m_CapitalDeleteStatement = m_DBConnection->prepareStatement("delete from t_Capital where TradingDay = ? and AccountID = ?;");
	}
	SetStatementForCapitalPrimaryKey(m_CapitalDeleteStatement, record->TradingDay, record->AccountID);
	m_CapitalDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapital Spend:%lldms", duration);
	}
}
void MysqlDB::DeleteCapitalByTradingDayIndex(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalDeleteByTradingDayIndexStatement == nullptr)
	{
		m_CapitalDeleteByTradingDayIndexStatement = m_DBConnection->prepareStatement("delete from t_Capital where TradingDay = ?;");
	}
	SetStatementForCapitalIndexTradingDay(m_CapitalDeleteByTradingDayIndexStatement, record);
	m_CapitalDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapitalByTradingDayIndex Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalUpdateStatement == nullptr)
	{
		m_CapitalUpdateStatement = m_DBConnection->prepareStatement("update t_Capital set AccountType = ?, Balance = ?, PreBalance = ?, Available = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, Deposit = ?, Withdraw = ? where TradingDay = ? and AccountID = ?;");
	}
	SetStatementForCapitalRecordUpdate(m_CapitalUpdateStatement, record);
	m_CapitalUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateCapital Spend:%lldms", duration);
	}
}
void MysqlDB::SelectCapital(std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	if (m_CapitalSelectStatement == nullptr)
	{
		m_CapitalSelectStatement = m_DBConnection->prepareStatement("select * from t_Capital;");
	}
	auto result = m_CapitalSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapital Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateCapital()
{
	auto start = steady_clock::now();
	if (m_CapitalTruncateStatement == nullptr)
	{
		m_CapitalTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Capital;");
	}
	m_CapitalTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateCapital Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreatePosition()
{
	auto start = steady_clock::now();
	if (m_PositionCreateStatement == nullptr)
	{
		m_PositionCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Position(`TradingDay` char(9), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `PosiDirection` int, `TotalPosition` bigint, `PositionFrozen` bigint, `TodayPosition` bigint, `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `VolumeMultiple` int, `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `SettlementPrice` decimal(24,8), `PreSettlementPrice` decimal(24,8), INDEX PositionAccount(TradingDay, AccountID), INDEX PositionTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_PositionCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePosition Spend:%lldms", duration);
}
void MysqlDB::DropPosition()
{
	auto start = steady_clock::now();
	if (m_PositionDropStatement == nullptr)
	{
		m_PositionDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Position;");
	}
	m_PositionDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPosition Spend:%lldms", duration);
}
void MysqlDB::InsertPosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionInsertStatement == nullptr)
	{
		m_PositionInsertStatement = m_DBConnection->prepareStatement("insert into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForPositionRecord(m_PositionInsertStatement, record);
	
	m_PositionInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPosition Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertPosition(std::list<Position*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Position Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Position Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteStatement == nullptr)
	{
		m_PositionDeleteStatement = m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;");
	}
	SetStatementForPositionPrimaryKey(m_PositionDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection);
	m_PositionDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void MysqlDB::DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByAccountIndexStatement == nullptr)
	{
		m_PositionDeleteByAccountIndexStatement = m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ? and AccountID = ?;");
	}
	SetStatementForPositionIndexAccount(m_PositionDeleteByAccountIndexStatement, record);
	m_PositionDeleteByAccountIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void MysqlDB::DeletePositionByTradingDayIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByTradingDayIndexStatement == nullptr)
	{
		m_PositionDeleteByTradingDayIndexStatement = m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ?;");
	}
	SetStatementForPositionIndexTradingDay(m_PositionDeleteByTradingDayIndexStatement, record);
	m_PositionDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByTradingDayIndex Spend:%lldms", duration);
	}
}
void MysqlDB::UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionUpdateStatement == nullptr)
	{
		m_PositionUpdateStatement = m_DBConnection->prepareStatement("update t_Position set AccountType = ?, ProductClass = ?, TotalPosition = ?, PositionFrozen = ?, TodayPosition = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;");
	}
	SetStatementForPositionRecordUpdate(m_PositionUpdateStatement, record);
	m_PositionUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void MysqlDB::SelectPosition(std::list<Position*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionSelectStatement == nullptr)
	{
		m_PositionSelectStatement = m_DBConnection->prepareStatement("select * from t_Position;");
	}
	auto result = m_PositionSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void MysqlDB::TruncatePosition()
{
	auto start = steady_clock::now();
	if (m_PositionTruncateStatement == nullptr)
	{
		m_PositionTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Position;");
	}
	m_PositionTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreatePositionDetail()
{
	auto start = steady_clock::now();
	if (m_PositionDetailCreateStatement == nullptr)
	{
		m_PositionDetailCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_PositionDetail(`TradingDay` char(9), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `PosiDirection` int, `OpenDate` char(9), `TradeID` char(64), `Volume` bigint, `OpenPrice` decimal(24,8), `MarketValue` decimal(24,8), `CashIn` decimal(24,8), `CashOut` decimal(24,8), `Margin` decimal(24,8), `Commission` decimal(24,8), `VolumeMultiple` int, `CloseProfitByDate` decimal(24,8), `CloseProfitByTrade` decimal(24,8), `PositionProfitByDate` decimal(24,8), `PositionProfitByTrade` decimal(24,8), `SettlementPrice` decimal(24,8), `PreSettlementPrice` decimal(24,8), `CloseVolume` bigint, `CloseAmount` decimal(24,8), INDEX PositionDetailTradeMatch(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection), INDEX PositionDetailTradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_PositionDetailCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePositionDetail Spend:%lldms", duration);
}
void MysqlDB::DropPositionDetail()
{
	auto start = steady_clock::now();
	if (m_PositionDetailDropStatement == nullptr)
	{
		m_PositionDetailDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_PositionDetail;");
	}
	m_PositionDetailDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPositionDetail Spend:%lldms", duration);
}
void MysqlDB::InsertPositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailInsertStatement == nullptr)
	{
		m_PositionDetailInsertStatement = m_DBConnection->prepareStatement("insert into t_PositionDetail Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForPositionDetailRecord(m_PositionDetailInsertStatement, record);
	
	m_PositionDetailInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPositionDetail Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertPositionDetail(std::list<PositionDetail*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_PositionDetail Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPositionDetail Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_PositionDetail Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPositionDetail Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPositionDetail RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeletePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteStatement == nullptr)
	{
		m_PositionDetailDeleteStatement = m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;");
	}
	SetStatementForPositionDetailPrimaryKey(m_PositionDetailDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection, record->OpenDate, record->TradeID);
	m_PositionDetailDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetail Spend:%lldms", duration);
	}
}
void MysqlDB::DeletePositionDetailByTradeMatchIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteByTradeMatchIndexStatement == nullptr)
	{
		m_PositionDetailDeleteByTradeMatchIndexStatement = m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;");
	}
	SetStatementForPositionDetailIndexTradeMatch(m_PositionDetailDeleteByTradeMatchIndexStatement, record);
	m_PositionDetailDeleteByTradeMatchIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradeMatchIndex Spend:%lldms", duration);
	}
}
void MysqlDB::DeletePositionDetailByTradingDayIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteByTradingDayIndexStatement == nullptr)
	{
		m_PositionDetailDeleteByTradingDayIndexStatement = m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ?;");
	}
	SetStatementForPositionDetailIndexTradingDay(m_PositionDetailDeleteByTradingDayIndexStatement, record);
	m_PositionDetailDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradingDayIndex Spend:%lldms", duration);
	}
}
void MysqlDB::UpdatePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailUpdateStatement == nullptr)
	{
		m_PositionDetailUpdateStatement = m_DBConnection->prepareStatement("update t_PositionDetail set AccountType = ?, ProductClass = ?, Volume = ?, OpenPrice = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ?, CloseVolume = ?, CloseAmount = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;");
	}
	SetStatementForPositionDetailRecordUpdate(m_PositionDetailUpdateStatement, record);
	m_PositionDetailUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePositionDetail Spend:%lldms", duration);
	}
}
void MysqlDB::SelectPositionDetail(std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionDetailSelectStatement == nullptr)
	{
		m_PositionDetailSelectStatement = m_DBConnection->prepareStatement("select * from t_PositionDetail;");
	}
	auto result = m_PositionDetailSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetail Spend:%lldms", duration);
	}
}
void MysqlDB::TruncatePositionDetail()
{
	auto start = steady_clock::now();
	if (m_PositionDetailTruncateStatement == nullptr)
	{
		m_PositionDetailTruncateStatement = m_DBConnection->prepareStatement("truncate table t_PositionDetail;");
	}
	m_PositionDetailTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePositionDetail Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateOrder()
{
	auto start = steady_clock::now();
	if (m_OrderCreateStatement == nullptr)
	{
		m_OrderCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Order(`TradingDay` char(9), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `OrderID` int, `OrderSysID` char(64), `Direction` int, `OffsetFlag` int, `OrderPriceType` int, `Price` decimal(24,8), `Volume` bigint, `VolumeTotal` bigint, `VolumeTraded` bigint, `VolumeMultiple` int, `OrderStatus` int, `OrderDate` char(9), `OrderTime` char(9), `CancelDate` char(9), `CancelTime` char(9), `SessionID` bigint, `ClientOrderID` int, `RequestID` int, `OfferID` int, `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, `FrozenCash` decimal(24,8), `FrozenMargin` decimal(24,8), `FrozenCommission` decimal(24,8), `RebuildMark` bool, `IsForceClose` bool, UNIQUE ClientOrderID(TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_OrderCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateOrder Spend:%lldms", duration);
}
void MysqlDB::DropOrder()
{
	auto start = steady_clock::now();
	if (m_OrderDropStatement == nullptr)
	{
		m_OrderDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Order;");
	}
	m_OrderDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropOrder Spend:%lldms", duration);
}
void MysqlDB::InsertOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderInsertStatement == nullptr)
	{
		m_OrderInsertStatement = m_DBConnection->prepareStatement("insert into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForOrderRecord(m_OrderInsertStatement, record);
	
	m_OrderInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertOrder Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertOrder(std::list<Order*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Order Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Order Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderDeleteStatement == nullptr)
	{
		m_OrderDeleteStatement = m_DBConnection->prepareStatement("delete from t_Order where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;");
	}
	SetStatementForOrderPrimaryKey(m_OrderDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->OrderID);
	m_OrderDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderUpdateStatement == nullptr)
	{
		m_OrderUpdateStatement = m_DBConnection->prepareStatement("update t_Order set AccountType = ?, ProductClass = ?, OrderSysID = ?, Direction = ?, OffsetFlag = ?, OrderPriceType = ?, Price = ?, Volume = ?, VolumeTotal = ?, VolumeTraded = ?, VolumeMultiple = ?, OrderStatus = ?, OrderDate = ?, OrderTime = ?, CancelDate = ?, CancelTime = ?, SessionID = ?, ClientOrderID = ?, RequestID = ?, OfferID = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, RebuildMark = ?, IsForceClose = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;");
	}
	SetStatementForOrderRecordUpdate(m_OrderUpdateStatement, record);
	m_OrderUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void MysqlDB::SelectOrder(std::list<Order*>& records)
{
	auto start = steady_clock::now();
	if (m_OrderSelectStatement == nullptr)
	{
		m_OrderSelectStatement = m_DBConnection->prepareStatement("select * from t_Order;");
	}
	auto result = m_OrderSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateOrder()
{
	auto start = steady_clock::now();
	if (m_OrderTruncateStatement == nullptr)
	{
		m_OrderTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Order;");
	}
	m_OrderTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}
void MysqlDB::CreateTrade()
{
	auto start = steady_clock::now();
	if (m_TradeCreateStatement == nullptr)
	{
		m_TradeCreateStatement = m_DBConnection->prepareStatement("CREATE TABLE IF NOT EXISTS t_Trade(`TradingDay` char(9), `AccountID` char(32), `AccountType` int, `ExchangeID` char(8), `InstrumentID` char(32), `ProductClass` int, `OrderID` int, `OrderSysID` char(64), `TradeID` char(64), `Direction` int, `OffsetFlag` int, `Price` decimal(24,8), `Volume` bigint, `VolumeMultiple` int, `TradeAmount` decimal(24,8), `Commission` decimal(24,8), `TradeDate` char(9), `TradeTime` char(9), PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';");
	}
	m_TradeCreateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTrade Spend:%lldms", duration);
}
void MysqlDB::DropTrade()
{
	auto start = steady_clock::now();
	if (m_TradeDropStatement == nullptr)
	{
		m_TradeDropStatement = m_DBConnection->prepareStatement("DROP TABLE IF EXISTS t_Trade;");
	}
	m_TradeDropStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTrade Spend:%lldms", duration);
}
void MysqlDB::InsertTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeInsertStatement == nullptr)
	{
		m_TradeInsertStatement = m_DBConnection->prepareStatement("insert into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForTradeRecord(m_TradeInsertStatement, record);
	
	m_TradeInsertStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTrade Spend:%lldms", duration);
	}
}
void MysqlDB::BatchInsertTrade(std::list<Trade*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Trade Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > 60000)
		{
			m_SqlBuff[n - 1] = ';';
			try
			{
				m_Statement->executeUpdate(m_SqlBuff);
			}
			catch(exception e)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
			}
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Trade Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';
	try
	{
		m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MysqlDB::DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeDeleteStatement == nullptr)
	{
		m_TradeDeleteStatement = m_DBConnection->prepareStatement("delete from t_Trade where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;");
	}
	SetStatementForTradePrimaryKey(m_TradeDeleteStatement, record->TradingDay, record->ExchangeID, record->TradeID, record->Direction);
	m_TradeDeleteStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void MysqlDB::UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeUpdateStatement == nullptr)
	{
		m_TradeUpdateStatement = m_DBConnection->prepareStatement("update t_Trade set AccountID = ?, AccountType = ?, InstrumentID = ?, ProductClass = ?, OrderID = ?, OrderSysID = ?, OffsetFlag = ?, Price = ?, Volume = ?, VolumeMultiple = ?, TradeAmount = ?, Commission = ?, TradeDate = ?, TradeTime = ? where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;");
	}
	SetStatementForTradeRecordUpdate(m_TradeUpdateStatement, record);
	m_TradeUpdateStatement->executeUpdate();
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void MysqlDB::SelectTrade(std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	if (m_TradeSelectStatement == nullptr)
	{
		m_TradeSelectStatement = m_DBConnection->prepareStatement("select * from t_Trade;");
	}
	auto result = m_TradeSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void MysqlDB::TruncateTrade()
{
	auto start = steady_clock::now();
	if (m_TradeTruncateStatement == nullptr)
	{
		m_TradeTruncateStatement = m_DBConnection->prepareStatement("truncate table t_Trade;");
	}
	m_TradeTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", GetDuration<chrono::milliseconds>(start));
}


void MysqlDB::SetStatementForTradingDayRecord(sql::PreparedStatement* statement, TradingDay* record)
{
	statement->setInt(1, record->PK);
	statement->setString(2, record->CurrTradingDay);
	statement->setString(3, record->PreTradingDay);
}
void MysqlDB::SetStatementForTradingDayRecordUpdate(sql::PreparedStatement* statement, TradingDay* record)
{
	statement->setString(1, record->CurrTradingDay);
	statement->setString(2, record->PreTradingDay);
	statement->setInt(3, record->PK);
}
void MysqlDB::SetStatementForTradingDayPrimaryKey(sql::PreparedStatement* statement, const IntType& PK)
{
	statement->setInt(1, PK);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<TradingDay*>& records)
{
	TradingDay* record = TradingDay::Allocate();
	record->PK = result->getInt(1);
	Strcpy(record->CurrTradingDay, result->getString(2).c_str());
	Strcpy(record->PreTradingDay, result->getString(3).c_str());
	records.push_back(record);
}
void MysqlDB::SetStatementForExchangeRecord(sql::PreparedStatement* statement, Exchange* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ExchangeName);
}
void MysqlDB::SetStatementForExchangeRecordUpdate(sql::PreparedStatement* statement, Exchange* record)
{
	statement->setString(1, record->ExchangeName);
	statement->setString(2, record->ExchangeID);
}
void MysqlDB::SetStatementForExchangePrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID)
{
	statement->setString(1, ExchangeID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Exchange*>& records)
{
	Exchange* record = Exchange::Allocate();
	Strcpy(record->ExchangeID, result->getString(1).c_str());
	Strcpy(record->ExchangeName, result->getString(2).c_str());
	records.push_back(record);
}
void MysqlDB::SetStatementForProductRecord(sql::PreparedStatement* statement, Product* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ProductID);
	statement->setString(3, record->ProductName);
	statement->setInt(4, int(record->ProductClass));
	statement->setInt(5, record->VolumeMultiple);
	statement->setDouble(6, record->PriceTick);
	statement->setInt64(7, record->MaxMarketOrderVolume);
	statement->setInt64(8, record->MinMarketOrderVolume);
	statement->setInt64(9, record->MaxLimitOrderVolume);
	statement->setInt64(10, record->MinLimitOrderVolume);
	statement->setString(11, record->SessionName);
}
void MysqlDB::SetStatementForProductRecordUpdate(sql::PreparedStatement* statement, Product* record)
{
	statement->setString(1, record->ProductName);
	statement->setInt(2, int(record->ProductClass));
	statement->setInt(3, record->VolumeMultiple);
	statement->setDouble(4, record->PriceTick);
	statement->setInt64(5, record->MaxMarketOrderVolume);
	statement->setInt64(6, record->MinMarketOrderVolume);
	statement->setInt64(7, record->MaxLimitOrderVolume);
	statement->setInt64(8, record->MinLimitOrderVolume);
	statement->setString(9, record->SessionName);
	statement->setString(10, record->ExchangeID);
	statement->setString(11, record->ProductID);
}
void MysqlDB::SetStatementForProductPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	statement->setString(1, ExchangeID);
	statement->setString(2, ProductID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Product*>& records)
{
	Product* record = Product::Allocate();
	Strcpy(record->ExchangeID, result->getString(1).c_str());
	Strcpy(record->ProductID, result->getString(2).c_str());
	Strcpy(record->ProductName, result->getString(3).c_str());
	record->ProductClass = ProductClassType(result->getInt(4));
	record->VolumeMultiple = result->getInt(5);
	record->PriceTick = result->getDouble(6);
	record->MaxMarketOrderVolume = result->getInt64(7);
	record->MinMarketOrderVolume = result->getInt64(8);
	record->MaxLimitOrderVolume = result->getInt64(9);
	record->MinLimitOrderVolume = result->getInt64(10);
	Strcpy(record->SessionName, result->getString(11).c_str());
	records.push_back(record);
}
void MysqlDB::SetStatementForInstrumentRecord(sql::PreparedStatement* statement, Instrument* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->InstrumentID);
	statement->setString(3, record->ExchangeInstID);
	statement->setString(4, record->InstrumentName);
	statement->setString(5, record->ProductID);
	statement->setInt(6, int(record->ProductClass));
	statement->setInt(7, int(record->InstrumentClass));
	statement->setInt(8, record->Rank);
	statement->setInt(9, record->VolumeMultiple);
	statement->setDouble(10, record->PriceTick);
	statement->setInt64(11, record->MaxMarketOrderVolume);
	statement->setInt64(12, record->MinMarketOrderVolume);
	statement->setInt64(13, record->MaxLimitOrderVolume);
	statement->setInt64(14, record->MinLimitOrderVolume);
	statement->setString(15, record->SessionName);
}
void MysqlDB::SetStatementForInstrumentRecordUpdate(sql::PreparedStatement* statement, Instrument* record)
{
	statement->setString(1, record->ExchangeInstID);
	statement->setString(2, record->InstrumentName);
	statement->setString(3, record->ProductID);
	statement->setInt(4, int(record->ProductClass));
	statement->setInt(5, int(record->InstrumentClass));
	statement->setInt(6, record->Rank);
	statement->setInt(7, record->VolumeMultiple);
	statement->setDouble(8, record->PriceTick);
	statement->setInt64(9, record->MaxMarketOrderVolume);
	statement->setInt64(10, record->MinMarketOrderVolume);
	statement->setInt64(11, record->MaxLimitOrderVolume);
	statement->setInt64(12, record->MinLimitOrderVolume);
	statement->setString(13, record->SessionName);
	statement->setString(14, record->ExchangeID);
	statement->setString(15, record->InstrumentID);
}
void MysqlDB::SetStatementForInstrumentPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
{
	statement->setString(1, ExchangeID);
	statement->setString(2, InstrumentID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Instrument*>& records)
{
	Instrument* record = Instrument::Allocate();
	Strcpy(record->ExchangeID, result->getString(1).c_str());
	Strcpy(record->InstrumentID, result->getString(2).c_str());
	Strcpy(record->ExchangeInstID, result->getString(3).c_str());
	Strcpy(record->InstrumentName, result->getString(4).c_str());
	Strcpy(record->ProductID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->InstrumentClass = InstrumentClassType(result->getInt(7));
	record->Rank = result->getInt(8);
	record->VolumeMultiple = result->getInt(9);
	record->PriceTick = result->getDouble(10);
	record->MaxMarketOrderVolume = result->getInt64(11);
	record->MinMarketOrderVolume = result->getInt64(12);
	record->MaxLimitOrderVolume = result->getInt64(13);
	record->MinLimitOrderVolume = result->getInt64(14);
	Strcpy(record->SessionName, result->getString(15).c_str());
	records.push_back(record);
}
void MysqlDB::SetStatementForPrimaryAccountRecord(sql::PreparedStatement* statement, PrimaryAccount* record)
{
	statement->setString(1, record->PrimaryAccountID);
	statement->setString(2, record->PrimaryAccountName);
	statement->setInt(3, int(record->AccountClass));
	statement->setString(4, record->BrokerPassword);
	statement->setInt(5, record->OfferID);
	statement->setBoolean(6, record->IsAllowLogin);
	statement->setBoolean(7, record->IsSimulateAccount);
	statement->setInt(8, int(record->LoginStatus));
	statement->setInt(9, int(record->InitStatus));
}
void MysqlDB::SetStatementForPrimaryAccountRecordUpdate(sql::PreparedStatement* statement, PrimaryAccount* record)
{
	statement->setString(1, record->PrimaryAccountName);
	statement->setInt(2, int(record->AccountClass));
	statement->setString(3, record->BrokerPassword);
	statement->setInt(4, record->OfferID);
	statement->setBoolean(5, record->IsAllowLogin);
	statement->setBoolean(6, record->IsSimulateAccount);
	statement->setInt(7, int(record->LoginStatus));
	statement->setInt(8, int(record->InitStatus));
	statement->setString(9, record->PrimaryAccountID);
}
void MysqlDB::SetStatementForPrimaryAccountPrimaryKey(sql::PreparedStatement* statement, const AccountIDType& PrimaryAccountID)
{
	statement->setString(1, PrimaryAccountID);
}
void MysqlDB::SetStatementForPrimaryAccountIndexOfferID(sql::PreparedStatement* statement, PrimaryAccount* record)
{
	statement->setInt(1, record->OfferID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<PrimaryAccount*>& records)
{
	PrimaryAccount* record = PrimaryAccount::Allocate();
	Strcpy(record->PrimaryAccountID, result->getString(1).c_str());
	Strcpy(record->PrimaryAccountName, result->getString(2).c_str());
	record->AccountClass = AccountClassType(result->getInt(3));
	Strcpy(record->BrokerPassword, result->getString(4).c_str());
	record->OfferID = result->getInt(5);
	record->IsAllowLogin = result->getBoolean(6);
	record->IsSimulateAccount = result->getBoolean(7);
	record->LoginStatus = LoginStatusType(result->getInt(8));
	record->InitStatus = InitStatusType(result->getInt(9));
	records.push_back(record);
}
void MysqlDB::SetStatementForAccountRecord(sql::PreparedStatement* statement, Account* record)
{
	statement->setString(1, record->AccountID);
	statement->setString(2, record->AccountName);
	statement->setInt(3, int(record->AccountType));
	statement->setInt(4, int(record->AccountStatus));
	statement->setString(5, record->Password);
	statement->setInt(6, record->TradeGroupID);
	statement->setInt(7, record->RiskGroupID);
	statement->setInt(8, record->CommissionGroupID);
}
void MysqlDB::SetStatementForAccountRecordUpdate(sql::PreparedStatement* statement, Account* record)
{
	statement->setString(1, record->AccountName);
	statement->setInt(2, int(record->AccountType));
	statement->setInt(3, int(record->AccountStatus));
	statement->setString(4, record->Password);
	statement->setInt(5, record->TradeGroupID);
	statement->setInt(6, record->RiskGroupID);
	statement->setInt(7, record->CommissionGroupID);
	statement->setString(8, record->AccountID);
}
void MysqlDB::SetStatementForAccountPrimaryKey(sql::PreparedStatement* statement, const AccountIDType& AccountID)
{
	statement->setString(1, AccountID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Account*>& records)
{
	Account* record = Account::Allocate();
	Strcpy(record->AccountID, result->getString(1).c_str());
	Strcpy(record->AccountName, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	record->AccountStatus = AccountStatusType(result->getInt(4));
	Strcpy(record->Password, result->getString(5).c_str());
	record->TradeGroupID = result->getInt(6);
	record->RiskGroupID = result->getInt(7);
	record->CommissionGroupID = result->getInt(8);
	records.push_back(record);
}
void MysqlDB::SetStatementForCapitalRecord(sql::PreparedStatement* statement, Capital* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setInt(3, int(record->AccountType));
	statement->setDouble(4, record->Balance);
	statement->setDouble(5, record->PreBalance);
	statement->setDouble(6, record->Available);
	statement->setDouble(7, record->MarketValue);
	statement->setDouble(8, record->CashIn);
	statement->setDouble(9, record->CashOut);
	statement->setDouble(10, record->Margin);
	statement->setDouble(11, record->Commission);
	statement->setDouble(12, record->FrozenCash);
	statement->setDouble(13, record->FrozenMargin);
	statement->setDouble(14, record->FrozenCommission);
	statement->setDouble(15, record->CloseProfitByDate);
	statement->setDouble(16, record->CloseProfitByTrade);
	statement->setDouble(17, record->PositionProfitByDate);
	statement->setDouble(18, record->PositionProfitByTrade);
	statement->setDouble(19, record->Deposit);
	statement->setDouble(20, record->Withdraw);
}
void MysqlDB::SetStatementForCapitalRecordUpdate(sql::PreparedStatement* statement, Capital* record)
{
	statement->setInt(1, int(record->AccountType));
	statement->setDouble(2, record->Balance);
	statement->setDouble(3, record->PreBalance);
	statement->setDouble(4, record->Available);
	statement->setDouble(5, record->MarketValue);
	statement->setDouble(6, record->CashIn);
	statement->setDouble(7, record->CashOut);
	statement->setDouble(8, record->Margin);
	statement->setDouble(9, record->Commission);
	statement->setDouble(10, record->FrozenCash);
	statement->setDouble(11, record->FrozenMargin);
	statement->setDouble(12, record->FrozenCommission);
	statement->setDouble(13, record->CloseProfitByDate);
	statement->setDouble(14, record->CloseProfitByTrade);
	statement->setDouble(15, record->PositionProfitByDate);
	statement->setDouble(16, record->PositionProfitByTrade);
	statement->setDouble(17, record->Deposit);
	statement->setDouble(18, record->Withdraw);
	statement->setString(19, record->TradingDay);
	statement->setString(20, record->AccountID);
}
void MysqlDB::SetStatementForCapitalPrimaryKey(sql::PreparedStatement* statement, const DateType& TradingDay, const AccountIDType& AccountID)
{
	statement->setString(1, TradingDay);
	statement->setString(2, AccountID);
}
void MysqlDB::SetStatementForCapitalIndexTradingDay(sql::PreparedStatement* statement, Capital* record)
{
	statement->setString(1, record->TradingDay);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Capital*>& records)
{
	Capital* record = Capital::Allocate();
	Strcpy(record->TradingDay, result->getString(1).c_str());
	Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	record->Balance = result->getDouble(4);
	record->PreBalance = result->getDouble(5);
	record->Available = result->getDouble(6);
	record->MarketValue = result->getDouble(7);
	record->CashIn = result->getDouble(8);
	record->CashOut = result->getDouble(9);
	record->Margin = result->getDouble(10);
	record->Commission = result->getDouble(11);
	record->FrozenCash = result->getDouble(12);
	record->FrozenMargin = result->getDouble(13);
	record->FrozenCommission = result->getDouble(14);
	record->CloseProfitByDate = result->getDouble(15);
	record->CloseProfitByTrade = result->getDouble(16);
	record->PositionProfitByDate = result->getDouble(17);
	record->PositionProfitByTrade = result->getDouble(18);
	record->Deposit = result->getDouble(19);
	record->Withdraw = result->getDouble(20);
	records.push_back(record);
}
void MysqlDB::SetStatementForPositionRecord(sql::PreparedStatement* statement, Position* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setInt(3, int(record->AccountType));
	statement->setString(4, record->ExchangeID);
	statement->setString(5, record->InstrumentID);
	statement->setInt(6, int(record->ProductClass));
	statement->setInt(7, int(record->PosiDirection));
	statement->setInt64(8, record->TotalPosition);
	statement->setInt64(9, record->PositionFrozen);
	statement->setInt64(10, record->TodayPosition);
	statement->setDouble(11, record->MarketValue);
	statement->setDouble(12, record->CashIn);
	statement->setDouble(13, record->CashOut);
	statement->setDouble(14, record->Margin);
	statement->setDouble(15, record->Commission);
	statement->setDouble(16, record->FrozenCash);
	statement->setDouble(17, record->FrozenMargin);
	statement->setDouble(18, record->FrozenCommission);
	statement->setInt(19, record->VolumeMultiple);
	statement->setDouble(20, record->CloseProfitByDate);
	statement->setDouble(21, record->CloseProfitByTrade);
	statement->setDouble(22, record->PositionProfitByDate);
	statement->setDouble(23, record->PositionProfitByTrade);
	statement->setDouble(24, record->SettlementPrice);
	statement->setDouble(25, record->PreSettlementPrice);
}
void MysqlDB::SetStatementForPositionRecordUpdate(sql::PreparedStatement* statement, Position* record)
{
	statement->setInt(1, int(record->AccountType));
	statement->setInt(2, int(record->ProductClass));
	statement->setInt64(3, record->TotalPosition);
	statement->setInt64(4, record->PositionFrozen);
	statement->setInt64(5, record->TodayPosition);
	statement->setDouble(6, record->MarketValue);
	statement->setDouble(7, record->CashIn);
	statement->setDouble(8, record->CashOut);
	statement->setDouble(9, record->Margin);
	statement->setDouble(10, record->Commission);
	statement->setDouble(11, record->FrozenCash);
	statement->setDouble(12, record->FrozenMargin);
	statement->setDouble(13, record->FrozenCommission);
	statement->setInt(14, record->VolumeMultiple);
	statement->setDouble(15, record->CloseProfitByDate);
	statement->setDouble(16, record->CloseProfitByTrade);
	statement->setDouble(17, record->PositionProfitByDate);
	statement->setDouble(18, record->PositionProfitByTrade);
	statement->setDouble(19, record->SettlementPrice);
	statement->setDouble(20, record->PreSettlementPrice);
	statement->setString(21, record->TradingDay);
	statement->setString(22, record->AccountID);
	statement->setString(23, record->ExchangeID);
	statement->setString(24, record->InstrumentID);
	statement->setInt(25, int(record->PosiDirection));
}
void MysqlDB::SetStatementForPositionPrimaryKey(sql::PreparedStatement* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
{
	statement->setString(1, TradingDay);
	statement->setString(2, AccountID);
	statement->setString(3, ExchangeID);
	statement->setString(4, InstrumentID);
	statement->setInt(5, int(PosiDirection));
}
void MysqlDB::SetStatementForPositionIndexAccount(sql::PreparedStatement* statement, Position* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
}
void MysqlDB::SetStatementForPositionIndexTradingDay(sql::PreparedStatement* statement, Position* record)
{
	statement->setString(1, record->TradingDay);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Position*>& records)
{
	Position* record = Position::Allocate();
	Strcpy(record->TradingDay, result->getString(1).c_str());
	Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Strcpy(record->ExchangeID, result->getString(4).c_str());
	Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->PosiDirection = PosiDirectionType(result->getInt(7));
	record->TotalPosition = result->getInt64(8);
	record->PositionFrozen = result->getInt64(9);
	record->TodayPosition = result->getInt64(10);
	record->MarketValue = result->getDouble(11);
	record->CashIn = result->getDouble(12);
	record->CashOut = result->getDouble(13);
	record->Margin = result->getDouble(14);
	record->Commission = result->getDouble(15);
	record->FrozenCash = result->getDouble(16);
	record->FrozenMargin = result->getDouble(17);
	record->FrozenCommission = result->getDouble(18);
	record->VolumeMultiple = result->getInt(19);
	record->CloseProfitByDate = result->getDouble(20);
	record->CloseProfitByTrade = result->getDouble(21);
	record->PositionProfitByDate = result->getDouble(22);
	record->PositionProfitByTrade = result->getDouble(23);
	record->SettlementPrice = result->getDouble(24);
	record->PreSettlementPrice = result->getDouble(25);
	records.push_back(record);
}
void MysqlDB::SetStatementForPositionDetailRecord(sql::PreparedStatement* statement, PositionDetail* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setInt(3, int(record->AccountType));
	statement->setString(4, record->ExchangeID);
	statement->setString(5, record->InstrumentID);
	statement->setInt(6, int(record->ProductClass));
	statement->setInt(7, int(record->PosiDirection));
	statement->setString(8, record->OpenDate);
	statement->setString(9, record->TradeID);
	statement->setInt64(10, record->Volume);
	statement->setDouble(11, record->OpenPrice);
	statement->setDouble(12, record->MarketValue);
	statement->setDouble(13, record->CashIn);
	statement->setDouble(14, record->CashOut);
	statement->setDouble(15, record->Margin);
	statement->setDouble(16, record->Commission);
	statement->setInt(17, record->VolumeMultiple);
	statement->setDouble(18, record->CloseProfitByDate);
	statement->setDouble(19, record->CloseProfitByTrade);
	statement->setDouble(20, record->PositionProfitByDate);
	statement->setDouble(21, record->PositionProfitByTrade);
	statement->setDouble(22, record->SettlementPrice);
	statement->setDouble(23, record->PreSettlementPrice);
	statement->setInt64(24, record->CloseVolume);
	statement->setDouble(25, record->CloseAmount);
}
void MysqlDB::SetStatementForPositionDetailRecordUpdate(sql::PreparedStatement* statement, PositionDetail* record)
{
	statement->setInt(1, int(record->AccountType));
	statement->setInt(2, int(record->ProductClass));
	statement->setInt64(3, record->Volume);
	statement->setDouble(4, record->OpenPrice);
	statement->setDouble(5, record->MarketValue);
	statement->setDouble(6, record->CashIn);
	statement->setDouble(7, record->CashOut);
	statement->setDouble(8, record->Margin);
	statement->setDouble(9, record->Commission);
	statement->setInt(10, record->VolumeMultiple);
	statement->setDouble(11, record->CloseProfitByDate);
	statement->setDouble(12, record->CloseProfitByTrade);
	statement->setDouble(13, record->PositionProfitByDate);
	statement->setDouble(14, record->PositionProfitByTrade);
	statement->setDouble(15, record->SettlementPrice);
	statement->setDouble(16, record->PreSettlementPrice);
	statement->setInt64(17, record->CloseVolume);
	statement->setDouble(18, record->CloseAmount);
	statement->setString(19, record->TradingDay);
	statement->setString(20, record->AccountID);
	statement->setString(21, record->ExchangeID);
	statement->setString(22, record->InstrumentID);
	statement->setInt(23, int(record->PosiDirection));
	statement->setString(24, record->OpenDate);
	statement->setString(25, record->TradeID);
}
void MysqlDB::SetStatementForPositionDetailPrimaryKey(sql::PreparedStatement* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID)
{
	statement->setString(1, TradingDay);
	statement->setString(2, AccountID);
	statement->setString(3, ExchangeID);
	statement->setString(4, InstrumentID);
	statement->setInt(5, int(PosiDirection));
	statement->setString(6, OpenDate);
	statement->setString(7, TradeID);
}
void MysqlDB::SetStatementForPositionDetailIndexTradeMatch(sql::PreparedStatement* statement, PositionDetail* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setString(3, record->ExchangeID);
	statement->setString(4, record->InstrumentID);
	statement->setInt(5, int(record->PosiDirection));
}
void MysqlDB::SetStatementForPositionDetailIndexTradingDay(sql::PreparedStatement* statement, PositionDetail* record)
{
	statement->setString(1, record->TradingDay);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<PositionDetail*>& records)
{
	PositionDetail* record = PositionDetail::Allocate();
	Strcpy(record->TradingDay, result->getString(1).c_str());
	Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Strcpy(record->ExchangeID, result->getString(4).c_str());
	Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->PosiDirection = PosiDirectionType(result->getInt(7));
	Strcpy(record->OpenDate, result->getString(8).c_str());
	Strcpy(record->TradeID, result->getString(9).c_str());
	record->Volume = result->getInt64(10);
	record->OpenPrice = result->getDouble(11);
	record->MarketValue = result->getDouble(12);
	record->CashIn = result->getDouble(13);
	record->CashOut = result->getDouble(14);
	record->Margin = result->getDouble(15);
	record->Commission = result->getDouble(16);
	record->VolumeMultiple = result->getInt(17);
	record->CloseProfitByDate = result->getDouble(18);
	record->CloseProfitByTrade = result->getDouble(19);
	record->PositionProfitByDate = result->getDouble(20);
	record->PositionProfitByTrade = result->getDouble(21);
	record->SettlementPrice = result->getDouble(22);
	record->PreSettlementPrice = result->getDouble(23);
	record->CloseVolume = result->getInt64(24);
	record->CloseAmount = result->getDouble(25);
	records.push_back(record);
}
void MysqlDB::SetStatementForOrderRecord(sql::PreparedStatement* statement, Order* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setInt(3, int(record->AccountType));
	statement->setString(4, record->ExchangeID);
	statement->setString(5, record->InstrumentID);
	statement->setInt(6, int(record->ProductClass));
	statement->setInt(7, record->OrderID);
	statement->setString(8, record->OrderSysID);
	statement->setInt(9, int(record->Direction));
	statement->setInt(10, int(record->OffsetFlag));
	statement->setInt(11, int(record->OrderPriceType));
	statement->setDouble(12, record->Price);
	statement->setInt64(13, record->Volume);
	statement->setInt64(14, record->VolumeTotal);
	statement->setInt64(15, record->VolumeTraded);
	statement->setInt(16, record->VolumeMultiple);
	statement->setInt(17, int(record->OrderStatus));
	statement->setString(18, record->OrderDate);
	statement->setString(19, record->OrderTime);
	statement->setString(20, record->CancelDate);
	statement->setString(21, record->CancelTime);
	statement->setInt64(22, record->SessionID);
	statement->setInt(23, record->ClientOrderID);
	statement->setInt(24, record->RequestID);
	statement->setInt(25, record->OfferID);
	statement->setInt(26, record->TradeGroupID);
	statement->setInt(27, record->RiskGroupID);
	statement->setInt(28, record->CommissionGroupID);
	statement->setDouble(29, record->FrozenCash);
	statement->setDouble(30, record->FrozenMargin);
	statement->setDouble(31, record->FrozenCommission);
	statement->setBoolean(32, record->RebuildMark);
	statement->setBoolean(33, record->IsForceClose);
}
void MysqlDB::SetStatementForOrderRecordUpdate(sql::PreparedStatement* statement, Order* record)
{
	statement->setInt(1, int(record->AccountType));
	statement->setInt(2, int(record->ProductClass));
	statement->setString(3, record->OrderSysID);
	statement->setInt(4, int(record->Direction));
	statement->setInt(5, int(record->OffsetFlag));
	statement->setInt(6, int(record->OrderPriceType));
	statement->setDouble(7, record->Price);
	statement->setInt64(8, record->Volume);
	statement->setInt64(9, record->VolumeTotal);
	statement->setInt64(10, record->VolumeTraded);
	statement->setInt(11, record->VolumeMultiple);
	statement->setInt(12, int(record->OrderStatus));
	statement->setString(13, record->OrderDate);
	statement->setString(14, record->OrderTime);
	statement->setString(15, record->CancelDate);
	statement->setString(16, record->CancelTime);
	statement->setInt64(17, record->SessionID);
	statement->setInt(18, record->ClientOrderID);
	statement->setInt(19, record->RequestID);
	statement->setInt(20, record->OfferID);
	statement->setInt(21, record->TradeGroupID);
	statement->setInt(22, record->RiskGroupID);
	statement->setInt(23, record->CommissionGroupID);
	statement->setDouble(24, record->FrozenCash);
	statement->setDouble(25, record->FrozenMargin);
	statement->setDouble(26, record->FrozenCommission);
	statement->setBoolean(27, record->RebuildMark);
	statement->setBoolean(28, record->IsForceClose);
	statement->setString(29, record->TradingDay);
	statement->setString(30, record->AccountID);
	statement->setString(31, record->ExchangeID);
	statement->setString(32, record->InstrumentID);
	statement->setInt(33, record->OrderID);
}
void MysqlDB::SetStatementForOrderPrimaryKey(sql::PreparedStatement* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
{
	statement->setString(1, TradingDay);
	statement->setString(2, AccountID);
	statement->setString(3, ExchangeID);
	statement->setString(4, InstrumentID);
	statement->setInt(5, OrderID);
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Order*>& records)
{
	Order* record = Order::Allocate();
	Strcpy(record->TradingDay, result->getString(1).c_str());
	Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Strcpy(record->ExchangeID, result->getString(4).c_str());
	Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->OrderID = result->getInt(7);
	Strcpy(record->OrderSysID, result->getString(8).c_str());
	record->Direction = DirectionType(result->getInt(9));
	record->OffsetFlag = OffsetFlagType(result->getInt(10));
	record->OrderPriceType = OrderPriceTypeType(result->getInt(11));
	record->Price = result->getDouble(12);
	record->Volume = result->getInt64(13);
	record->VolumeTotal = result->getInt64(14);
	record->VolumeTraded = result->getInt64(15);
	record->VolumeMultiple = result->getInt(16);
	record->OrderStatus = OrderStatusType(result->getInt(17));
	Strcpy(record->OrderDate, result->getString(18).c_str());
	Strcpy(record->OrderTime, result->getString(19).c_str());
	Strcpy(record->CancelDate, result->getString(20).c_str());
	Strcpy(record->CancelTime, result->getString(21).c_str());
	record->SessionID = result->getInt64(22);
	record->ClientOrderID = result->getInt(23);
	record->RequestID = result->getInt(24);
	record->OfferID = result->getInt(25);
	record->TradeGroupID = result->getInt(26);
	record->RiskGroupID = result->getInt(27);
	record->CommissionGroupID = result->getInt(28);
	record->FrozenCash = result->getDouble(29);
	record->FrozenMargin = result->getDouble(30);
	record->FrozenCommission = result->getDouble(31);
	record->RebuildMark = result->getBoolean(32);
	record->IsForceClose = result->getBoolean(33);
	records.push_back(record);
}
void MysqlDB::SetStatementForTradeRecord(sql::PreparedStatement* statement, Trade* record)
{
	statement->setString(1, record->TradingDay);
	statement->setString(2, record->AccountID);
	statement->setInt(3, int(record->AccountType));
	statement->setString(4, record->ExchangeID);
	statement->setString(5, record->InstrumentID);
	statement->setInt(6, int(record->ProductClass));
	statement->setInt(7, record->OrderID);
	statement->setString(8, record->OrderSysID);
	statement->setString(9, record->TradeID);
	statement->setInt(10, int(record->Direction));
	statement->setInt(11, int(record->OffsetFlag));
	statement->setDouble(12, record->Price);
	statement->setInt64(13, record->Volume);
	statement->setInt(14, record->VolumeMultiple);
	statement->setDouble(15, record->TradeAmount);
	statement->setDouble(16, record->Commission);
	statement->setString(17, record->TradeDate);
	statement->setString(18, record->TradeTime);
}
void MysqlDB::SetStatementForTradeRecordUpdate(sql::PreparedStatement* statement, Trade* record)
{
	statement->setString(1, record->AccountID);
	statement->setInt(2, int(record->AccountType));
	statement->setString(3, record->InstrumentID);
	statement->setInt(4, int(record->ProductClass));
	statement->setInt(5, record->OrderID);
	statement->setString(6, record->OrderSysID);
	statement->setInt(7, int(record->OffsetFlag));
	statement->setDouble(8, record->Price);
	statement->setInt64(9, record->Volume);
	statement->setInt(10, record->VolumeMultiple);
	statement->setDouble(11, record->TradeAmount);
	statement->setDouble(12, record->Commission);
	statement->setString(13, record->TradeDate);
	statement->setString(14, record->TradeTime);
	statement->setString(15, record->TradingDay);
	statement->setString(16, record->ExchangeID);
	statement->setString(17, record->TradeID);
	statement->setInt(18, int(record->Direction));
}
void MysqlDB::SetStatementForTradePrimaryKey(sql::PreparedStatement* statement, const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
{
	statement->setString(1, TradingDay);
	statement->setString(2, ExchangeID);
	statement->setString(3, TradeID);
	statement->setInt(4, int(Direction));
}
void MysqlDB::ParseRecord(sql::ResultSet* result, std::list<Trade*>& records)
{
	Trade* record = Trade::Allocate();
	Strcpy(record->TradingDay, result->getString(1).c_str());
	Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Strcpy(record->ExchangeID, result->getString(4).c_str());
	Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->OrderID = result->getInt(7);
	Strcpy(record->OrderSysID, result->getString(8).c_str());
	Strcpy(record->TradeID, result->getString(9).c_str());
	record->Direction = DirectionType(result->getInt(10));
	record->OffsetFlag = OffsetFlagType(result->getInt(11));
	record->Price = result->getDouble(12);
	record->Volume = result->getInt64(13);
	record->VolumeMultiple = result->getInt(14);
	record->TradeAmount = result->getDouble(15);
	record->Commission = result->getDouble(16);
	Strcpy(record->TradeDate, result->getString(17).c_str());
	Strcpy(record->TradeTime, result->getString(18).c_str());
	records.push_back(record);
}

