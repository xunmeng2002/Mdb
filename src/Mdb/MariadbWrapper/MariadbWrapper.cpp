#include <Mdb/MariadbWrapper/MariadbWrapper.h>
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <PersonalLib/Core/Core.h>
#include <mariadb/conncpp.hpp>
#include <string.h>
#include <cstring>

using namespace mdb;
using namespace std;
using namespace std::chrono;

namespace
{
    struct SqlConnectionDeleter
	{
        void operator()(sql::Connection* conn) const
		{
            if (conn) { conn->close(); delete conn; }
        }
    };
    struct SqlStatementDeleter
	{
        void operator()(sql::Statement* stmt) const
		{
            if (stmt) { stmt->close(); delete stmt; }
        }
    };
    struct SqlPreparedStatementDeleter
	{
        void operator()(sql::PreparedStatement* pstmt) const
		{
            if (pstmt) { pstmt->close(); delete pstmt; }
        }
    };
}

struct MariadbWrapper::Impl
 {
	 Impl()
	{
        m_Driver = sql::mariadb::get_driver_instance();
    }
    ~Impl() = default;
	void DisConnect()
	{
		m_DBConnection.reset();
		m_Statement.reset();
		m_TradingDayCreateStatement.reset();
		m_TradingDayDropStatement.reset();
		m_TradingDayInsertStatement.reset();
		m_TradingDayDeleteStatement.reset();
		m_TradingDayUpdateStatement.reset();
		m_TradingDaySelectStatement.reset();
		m_TradingDayTruncateStatement.reset();
		m_ExchangeCreateStatement.reset();
		m_ExchangeDropStatement.reset();
		m_ExchangeInsertStatement.reset();
		m_ExchangeDeleteStatement.reset();
		m_ExchangeUpdateStatement.reset();
		m_ExchangeSelectStatement.reset();
		m_ExchangeTruncateStatement.reset();
		m_ProductCreateStatement.reset();
		m_ProductDropStatement.reset();
		m_ProductInsertStatement.reset();
		m_ProductDeleteStatement.reset();
		m_ProductUpdateStatement.reset();
		m_ProductSelectStatement.reset();
		m_ProductTruncateStatement.reset();
		m_InstrumentCreateStatement.reset();
		m_InstrumentDropStatement.reset();
		m_InstrumentInsertStatement.reset();
		m_InstrumentDeleteStatement.reset();
		m_InstrumentUpdateStatement.reset();
		m_InstrumentSelectStatement.reset();
		m_InstrumentTruncateStatement.reset();
		m_PrimaryAccountCreateStatement.reset();
		m_PrimaryAccountDropStatement.reset();
		m_PrimaryAccountInsertStatement.reset();
		m_PrimaryAccountDeleteStatement.reset();
		m_PrimaryAccountDeleteByOfferIDIndexStatement.reset();
		m_PrimaryAccountUpdateStatement.reset();
		m_PrimaryAccountSelectStatement.reset();
		m_PrimaryAccountTruncateStatement.reset();
		m_AccountCreateStatement.reset();
		m_AccountDropStatement.reset();
		m_AccountInsertStatement.reset();
		m_AccountDeleteStatement.reset();
		m_AccountUpdateStatement.reset();
		m_AccountSelectStatement.reset();
		m_AccountTruncateStatement.reset();
		m_CapitalCreateStatement.reset();
		m_CapitalDropStatement.reset();
		m_CapitalInsertStatement.reset();
		m_CapitalDeleteStatement.reset();
		m_CapitalDeleteByTradingDayIndexStatement.reset();
		m_CapitalUpdateStatement.reset();
		m_CapitalSelectStatement.reset();
		m_CapitalTruncateStatement.reset();
		m_PositionCreateStatement.reset();
		m_PositionDropStatement.reset();
		m_PositionInsertStatement.reset();
		m_PositionDeleteStatement.reset();
		m_PositionDeleteByAccountIndexStatement.reset();
		m_PositionDeleteByTradingDayIndexStatement.reset();
		m_PositionUpdateStatement.reset();
		m_PositionSelectStatement.reset();
		m_PositionTruncateStatement.reset();
		m_PositionDetailCreateStatement.reset();
		m_PositionDetailDropStatement.reset();
		m_PositionDetailInsertStatement.reset();
		m_PositionDetailDeleteStatement.reset();
		m_PositionDetailDeleteByTradeMatchIndexStatement.reset();
		m_PositionDetailDeleteByTradingDayIndexStatement.reset();
		m_PositionDetailUpdateStatement.reset();
		m_PositionDetailSelectStatement.reset();
		m_PositionDetailTruncateStatement.reset();
		m_OrderCreateStatement.reset();
		m_OrderDropStatement.reset();
		m_OrderInsertStatement.reset();
		m_OrderDeleteStatement.reset();
		m_OrderUpdateStatement.reset();
		m_OrderSelectStatement.reset();
		m_OrderTruncateStatement.reset();
		m_TradeCreateStatement.reset();
		m_TradeDropStatement.reset();
		m_TradeInsertStatement.reset();
		m_TradeDeleteStatement.reset();
		m_TradeUpdateStatement.reset();
		m_TradeSelectStatement.reset();
		m_TradeTruncateStatement.reset();
	}
	
    sql::Driver* m_Driver = nullptr;
    std::unique_ptr<sql::Connection, SqlConnectionDeleter> m_DBConnection;
    std::unique_ptr<sql::Statement, SqlStatementDeleter> m_Statement;

	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDaySelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradingDayTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ExchangeTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_ProductTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_InstrumentTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountDeleteByOfferIDIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PrimaryAccountTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_AccountTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalDeleteByTradingDayIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_CapitalTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDeleteByAccountIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDeleteByTradingDayIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailDeleteByTradeMatchIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailDeleteByTradingDayIndexStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_PositionDetailTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_OrderTruncateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeCreateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeDropStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeInsertStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeDeleteStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeUpdateStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeSelectStatement;
	std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter> m_TradeTruncateStatement;
};

static void ParseRecord(sql::ResultSet* result, std::list<TradingDay*>& records)
{
	TradingDay* record = TradingDay::Allocate();
	record->PK = result->getInt(1);
	Utility::Strcpy(record->CurrTradingDay, result->getString(2).c_str());
	Utility::Strcpy(record->PreTradingDay, result->getString(3).c_str());
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<Exchange*>& records)
{
	Exchange* record = Exchange::Allocate();
	Utility::Strcpy(record->ExchangeID, result->getString(1).c_str());
	Utility::Strcpy(record->ExchangeName, result->getString(2).c_str());
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<Product*>& records)
{
	Product* record = Product::Allocate();
	Utility::Strcpy(record->ExchangeID, result->getString(1).c_str());
	Utility::Strcpy(record->ProductID, result->getString(2).c_str());
	Utility::Strcpy(record->ProductName, result->getString(3).c_str());
	record->ProductClass = ProductClassType(result->getInt(4));
	record->VolumeMultiple = result->getInt(5);
	record->PriceTick = result->getDouble(6);
	record->MaxMarketOrderVolume = result->getInt64(7);
	record->MinMarketOrderVolume = result->getInt64(8);
	record->MaxLimitOrderVolume = result->getInt64(9);
	record->MinLimitOrderVolume = result->getInt64(10);
	Utility::Strcpy(record->SessionName, result->getString(11).c_str());
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<Instrument*>& records)
{
	Instrument* record = Instrument::Allocate();
	Utility::Strcpy(record->ExchangeID, result->getString(1).c_str());
	Utility::Strcpy(record->InstrumentID, result->getString(2).c_str());
	Utility::Strcpy(record->ExchangeInstID, result->getString(3).c_str());
	Utility::Strcpy(record->InstrumentName, result->getString(4).c_str());
	Utility::Strcpy(record->ProductID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->InstrumentClass = InstrumentClassType(result->getInt(7));
	record->Rank = result->getInt(8);
	record->VolumeMultiple = result->getInt(9);
	record->PriceTick = result->getDouble(10);
	record->MaxMarketOrderVolume = result->getInt64(11);
	record->MinMarketOrderVolume = result->getInt64(12);
	record->MaxLimitOrderVolume = result->getInt64(13);
	record->MinLimitOrderVolume = result->getInt64(14);
	Utility::Strcpy(record->SessionName, result->getString(15).c_str());
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<PrimaryAccount*>& records)
{
	PrimaryAccount* record = PrimaryAccount::Allocate();
	Utility::Strcpy(record->PrimaryAccountID, result->getString(1).c_str());
	Utility::Strcpy(record->PrimaryAccountName, result->getString(2).c_str());
	record->AccountClass = AccountClassType(result->getInt(3));
	Utility::Strcpy(record->BrokerPassword, result->getString(4).c_str());
	record->OfferID = result->getInt(5);
	record->IsAllowLogin = result->getBoolean(6);
	record->IsSimulateAccount = result->getBoolean(7);
	record->LoginStatus = LoginStatusType(result->getInt(8));
	record->InitStatus = InitStatusType(result->getInt(9));
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<Account*>& records)
{
	Account* record = Account::Allocate();
	Utility::Strcpy(record->AccountID, result->getString(1).c_str());
	Utility::Strcpy(record->AccountName, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	record->AccountStatus = AccountStatusType(result->getInt(4));
	Utility::Strcpy(record->Password, result->getString(5).c_str());
	record->TradeGroupID = result->getInt(6);
	record->RiskGroupID = result->getInt(7);
	record->CommissionGroupID = result->getInt(8);
	records.push_back(record);
}
static void ParseRecord(sql::ResultSet* result, std::list<Capital*>& records)
{
	Capital* record = Capital::Allocate();
	Utility::Strcpy(record->TradingDay, result->getString(1).c_str());
	Utility::Strcpy(record->AccountID, result->getString(2).c_str());
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
static void ParseRecord(sql::ResultSet* result, std::list<Position*>& records)
{
	Position* record = Position::Allocate();
	Utility::Strcpy(record->TradingDay, result->getString(1).c_str());
	Utility::Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Utility::Strcpy(record->ExchangeID, result->getString(4).c_str());
	Utility::Strcpy(record->InstrumentID, result->getString(5).c_str());
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
static void ParseRecord(sql::ResultSet* result, std::list<PositionDetail*>& records)
{
	PositionDetail* record = PositionDetail::Allocate();
	Utility::Strcpy(record->TradingDay, result->getString(1).c_str());
	Utility::Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Utility::Strcpy(record->ExchangeID, result->getString(4).c_str());
	Utility::Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->PosiDirection = PosiDirectionType(result->getInt(7));
	Utility::Strcpy(record->OpenDate, result->getString(8).c_str());
	Utility::Strcpy(record->TradeID, result->getString(9).c_str());
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
static void ParseRecord(sql::ResultSet* result, std::list<Order*>& records)
{
	Order* record = Order::Allocate();
	Utility::Strcpy(record->TradingDay, result->getString(1).c_str());
	Utility::Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Utility::Strcpy(record->ExchangeID, result->getString(4).c_str());
	Utility::Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->OrderID = result->getInt(7);
	Utility::Strcpy(record->OrderSysID, result->getString(8).c_str());
	record->Direction = DirectionType(result->getInt(9));
	record->OffsetFlag = OffsetFlagType(result->getInt(10));
	record->OrderPriceType = OrderPriceTypeType(result->getInt(11));
	record->Price = result->getDouble(12);
	record->Volume = result->getInt64(13);
	record->VolumeTotal = result->getInt64(14);
	record->VolumeTraded = result->getInt64(15);
	record->VolumeMultiple = result->getInt(16);
	record->OrderStatus = OrderStatusType(result->getInt(17));
	Utility::Strcpy(record->OrderDate, result->getString(18).c_str());
	Utility::Strcpy(record->OrderTime, result->getString(19).c_str());
	Utility::Strcpy(record->CancelDate, result->getString(20).c_str());
	Utility::Strcpy(record->CancelTime, result->getString(21).c_str());
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
static void ParseRecord(sql::ResultSet* result, std::list<Trade*>& records)
{
	Trade* record = Trade::Allocate();
	Utility::Strcpy(record->TradingDay, result->getString(1).c_str());
	Utility::Strcpy(record->AccountID, result->getString(2).c_str());
	record->AccountType = AccountTypeType(result->getInt(3));
	Utility::Strcpy(record->ExchangeID, result->getString(4).c_str());
	Utility::Strcpy(record->InstrumentID, result->getString(5).c_str());
	record->ProductClass = ProductClassType(result->getInt(6));
	record->OrderID = result->getInt(7);
	Utility::Strcpy(record->OrderSysID, result->getString(8).c_str());
	Utility::Strcpy(record->TradeID, result->getString(9).c_str());
	record->Direction = DirectionType(result->getInt(10));
	record->OffsetFlag = OffsetFlagType(result->getInt(11));
	record->Price = result->getDouble(12);
	record->Volume = result->getInt64(13);
	record->VolumeMultiple = result->getInt(14);
	record->TradeAmount = result->getDouble(15);
	record->Commission = result->getDouble(16);
	Utility::Strcpy(record->TradeDate, result->getString(17).c_str());
	Utility::Strcpy(record->TradeTime, result->getString(18).c_str());
	records.push_back(record);
}

MariadbWrapper::MariadbWrapper(const std::string& host, const std::string& user, const std::string& passwd)
	:m_Host(host), m_User(user), m_Passwd(passwd), m_Impl(std::make_unique<Impl>())
{
	m_SqlBuff = new char[BuffSize];
}
MariadbWrapper::~MariadbWrapper()
{
	delete[] m_SqlBuff;
	DisConnect();
}
bool MariadbWrapper::Connect()
{
	try
	{
		m_Impl->m_DBConnection.reset(m_Impl->m_Driver->connect(m_Host, m_User, m_Passwd));
        if (!m_Impl->m_DBConnection)
			return false;
        m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	catch (...)
	{
		WriteLog(LogLevel::Warning, "Connect Mariadb Failed.");
		return false;
	}
	WriteLog(LogLevel::Warning, "Connect Mariadb Success.");
	return true;
}
void MariadbWrapper::DisConnect()
{
	m_Impl->DisConnect();
}
void MariadbWrapper::InitDB()
{
	m_Impl->m_Statement->executeUpdate("Truncate Table t_TradingDay;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_TradingDay select * from Init.t_TradingDay;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Exchange;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Exchange select * from Init.t_Exchange;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Product;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Product select * from Init.t_Product;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Instrument;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Instrument select * from Init.t_Instrument;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_PrimaryAccount;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_PrimaryAccount select * from Init.t_PrimaryAccount;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Account;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Account select * from Init.t_Account;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Capital;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Capital select * from Init.t_Capital;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Position;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Position select * from Init.t_Position;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_PositionDetail;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_PositionDetail select * from Init.t_PositionDetail;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Order;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Order select * from Init.t_Order;");
	m_Impl->m_Statement->executeUpdate("Truncate Table t_Trade;");
	m_Impl->m_Statement->executeUpdate("Insert Into t_Trade select * from Init.t_Trade;");
}
void MariadbWrapper::CreateTables()
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
void MariadbWrapper::DropTables()
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
void MariadbWrapper::TruncateTables()
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
void MariadbWrapper::TruncateSessionTables()
{
	auto start = steady_clock::now();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::Exec(const char* sql)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	m_Impl->m_Statement->executeUpdate(sql);
}

void MariadbWrapper::CreateTradingDay()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_TradingDay` (`PK` int COMMENT '主键', `CurrTradingDay` char(16) COMMENT '当前交易日', `PreTradingDay` char(16) COMMENT '昨交易日', PRIMARY KEY(PK)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_TradingDayCreateStatement == nullptr)
	{
		m_Impl->m_TradingDayCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_TradingDayCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropTradingDay()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_TradingDay`;";
	if (m_Impl->m_TradingDayDropStatement == nullptr)
	{
		m_Impl->m_TradingDayDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_TradingDayDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradingDayInsertStatement == nullptr)
	{
		m_Impl->m_TradingDayInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_TradingDay Values(?, ?, ?);"));
	}
	SetStatementForTradingDayRecordInsert(record);
	
	m_Impl->m_TradingDayInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertTradingDay(std::list<TradingDay*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradingDayDeleteStatement == nullptr)
	{
		m_Impl->m_TradingDayDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_TradingDay where PK = ?;"));
	}
	SetStatementForTradingDayPrimaryKey(record->PK);
	m_Impl->m_TradingDayDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradingDayUpdateStatement == nullptr)
	{
		m_Impl->m_TradingDayUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_TradingDay set CurrTradingDay = ?, PreTradingDay = ? where PK = ?;"));
	}
	SetStatementForTradingDayRecordUpdate(record);
	m_Impl->m_TradingDayUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectTradingDay(std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradingDaySelectStatement == nullptr)
	{
		m_Impl->m_TradingDaySelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_TradingDay;"));
	}
	auto result = m_Impl->m_TradingDaySelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectTradingDayWithSql(const char* sql, std::list<TradingDay*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateTradingDay()
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradingDayTruncateStatement == nullptr)
	{
		m_Impl->m_TradingDayTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_TradingDay;"));
	}
	m_Impl->m_TradingDayTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateExchange()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Exchange` (`ExchangeID` char(8) COMMENT '交易所代码', `ExchangeName` char(64) COMMENT '交易所名称', PRIMARY KEY(ExchangeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_ExchangeCreateStatement == nullptr)
	{
		m_Impl->m_ExchangeCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_ExchangeCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateExchange Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropExchange()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Exchange`;";
	if (m_Impl->m_ExchangeDropStatement == nullptr)
	{
		m_Impl->m_ExchangeDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_ExchangeDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropExchange Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ExchangeInsertStatement == nullptr)
	{
		m_Impl->m_ExchangeInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Exchange Values(?, ?);"));
	}
	SetStatementForExchangeRecordInsert(record);
	
	m_Impl->m_ExchangeInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertExchange Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertExchange(std::list<Exchange*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ExchangeDeleteStatement == nullptr)
	{
		m_Impl->m_ExchangeDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Exchange where ExchangeID = ?;"));
	}
	SetStatementForExchangePrimaryKey(record->ExchangeID);
	m_Impl->m_ExchangeDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ExchangeUpdateStatement == nullptr)
	{
		m_Impl->m_ExchangeUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Exchange set ExchangeName = ? where ExchangeID = ?;"));
	}
	SetStatementForExchangeRecordUpdate(record);
	m_Impl->m_ExchangeUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectExchange(std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ExchangeSelectStatement == nullptr)
	{
		m_Impl->m_ExchangeSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Exchange;"));
	}
	auto result = m_Impl->m_ExchangeSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectExchangeWithSql(const char* sql, std::list<Exchange*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateExchange()
{
	auto start = steady_clock::now();
	if (m_Impl->m_ExchangeTruncateStatement == nullptr)
	{
		m_Impl->m_ExchangeTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Exchange;"));
	}
	m_Impl->m_ExchangeTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateProduct()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Product` (`ExchangeID` char(8) COMMENT '交易所代码', `ProductID` char(32) COMMENT '品种代码', `ProductName` char(32) COMMENT '品种名称', `ProductClass` int COMMENT '品种类型', `VolumeMultiple` int COMMENT '合约乘数', `PriceTick` decimal(24,8) COMMENT '最小变动价位', `MaxMarketOrderVolume` bigint COMMENT '市价最大下单量', `MinMarketOrderVolume` bigint COMMENT '市价最小下单量', `MaxLimitOrderVolume` bigint COMMENT '限价最大下单量', `MinLimitOrderVolume` bigint COMMENT '限价最小下单量', `SessionName` char(32) COMMENT '交易节名称', PRIMARY KEY(ExchangeID, ProductID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_ProductCreateStatement == nullptr)
	{
		m_Impl->m_ProductCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_ProductCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateProduct Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropProduct()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Product`;";
	if (m_Impl->m_ProductDropStatement == nullptr)
	{
		m_Impl->m_ProductDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_ProductDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropProduct Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ProductInsertStatement == nullptr)
	{
		m_Impl->m_ProductInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForProductRecordInsert(record);
	
	m_Impl->m_ProductInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertProduct Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertProduct(std::list<Product*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ProductDeleteStatement == nullptr)
	{
		m_Impl->m_ProductDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Product where ExchangeID = ? and ProductID = ?;"));
	}
	SetStatementForProductPrimaryKey(record->ExchangeID, record->ProductID);
	m_Impl->m_ProductDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ProductUpdateStatement == nullptr)
	{
		m_Impl->m_ProductUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Product set ProductName = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and ProductID = ?;"));
	}
	SetStatementForProductRecordUpdate(record);
	m_Impl->m_ProductUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectProduct(std::list<Product*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_ProductSelectStatement == nullptr)
	{
		m_Impl->m_ProductSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Product;"));
	}
	auto result = m_Impl->m_ProductSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectProductWithSql(const char* sql, std::list<Product*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateProduct()
{
	auto start = steady_clock::now();
	if (m_Impl->m_ProductTruncateStatement == nullptr)
	{
		m_Impl->m_ProductTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Product;"));
	}
	m_Impl->m_ProductTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateInstrument()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Instrument` (`ExchangeID` char(8) COMMENT '交易所代码', `InstrumentID` char(32) COMMENT '合约代码', `ExchangeInstID` char(32) COMMENT '交易所合约代码', `InstrumentName` char(64) COMMENT '合约名称', `ProductID` char(32) COMMENT '品种代码', `ProductClass` int COMMENT '品种类型', `InstrumentClass` int COMMENT '合约类别', `Rank` int COMMENT '级别', `VolumeMultiple` int COMMENT '合约乘数', `PriceTick` decimal(24,8) COMMENT '最小变动价位', `MaxMarketOrderVolume` bigint COMMENT '市价最大下单量', `MinMarketOrderVolume` bigint COMMENT '市价最小下单量', `MaxLimitOrderVolume` bigint COMMENT '限价最大下单量', `MinLimitOrderVolume` bigint COMMENT '限价最小下单量', `SessionName` char(32) COMMENT '交易节名称', PRIMARY KEY(ExchangeID, InstrumentID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_InstrumentCreateStatement == nullptr)
	{
		m_Impl->m_InstrumentCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_InstrumentCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateInstrument Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropInstrument()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Instrument`;";
	if (m_Impl->m_InstrumentDropStatement == nullptr)
	{
		m_Impl->m_InstrumentDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_InstrumentDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropInstrument Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_InstrumentInsertStatement == nullptr)
	{
		m_Impl->m_InstrumentInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Instrument Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForInstrumentRecordInsert(record);
	
	m_Impl->m_InstrumentInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertInstrument(std::list<Instrument*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_InstrumentDeleteStatement == nullptr)
	{
		m_Impl->m_InstrumentDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Instrument where ExchangeID = ? and InstrumentID = ?;"));
	}
	SetStatementForInstrumentPrimaryKey(record->ExchangeID, record->InstrumentID);
	m_Impl->m_InstrumentDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_InstrumentUpdateStatement == nullptr)
	{
		m_Impl->m_InstrumentUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Instrument set ExchangeInstID = ?, InstrumentName = ?, ProductID = ?, ProductClass = ?, InstrumentClass = ?, Rank = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and InstrumentID = ?;"));
	}
	SetStatementForInstrumentRecordUpdate(record);
	m_Impl->m_InstrumentUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectInstrument(std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_InstrumentSelectStatement == nullptr)
	{
		m_Impl->m_InstrumentSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Instrument;"));
	}
	auto result = m_Impl->m_InstrumentSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectInstrumentWithSql(const char* sql, std::list<Instrument*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateInstrument()
{
	auto start = steady_clock::now();
	if (m_Impl->m_InstrumentTruncateStatement == nullptr)
	{
		m_Impl->m_InstrumentTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Instrument;"));
	}
	m_Impl->m_InstrumentTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreatePrimaryAccount()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_PrimaryAccount` (`PrimaryAccountID` char(32) COMMENT '主账户代码', `PrimaryAccountName` char(64) COMMENT '主账户名称', `AccountClass` int COMMENT '账户类别', `BrokerPassword` char(64) COMMENT '经纪公司密码', `OfferID` int COMMENT '报盘代码', `IsAllowLogin` bool COMMENT '是否允许登陆', `IsSimulateAccount` bool COMMENT '是否模拟账号', `LoginStatus` int COMMENT '登录状态', `InitStatus` int COMMENT '初始化状态', INDEX OfferID(OfferID), PRIMARY KEY(PrimaryAccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_PrimaryAccountCreateStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_PrimaryAccountCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropPrimaryAccount()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_PrimaryAccount`;";
	if (m_Impl->m_PrimaryAccountDropStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_PrimaryAccountDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertPrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountInsertStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_PrimaryAccount Values(?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForPrimaryAccountRecordInsert(record);
	
	m_Impl->m_PrimaryAccountInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPrimaryAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertPrimaryAccount(std::list<PrimaryAccount*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeletePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountDeleteStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_PrimaryAccount where PrimaryAccountID = ?;"));
	}
	SetStatementForPrimaryAccountPrimaryKey(record->PrimaryAccountID);
	m_Impl->m_PrimaryAccountDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeletePrimaryAccountByOfferIDIndex(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountDeleteByOfferIDIndexStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountDeleteByOfferIDIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_PrimaryAccount where OfferID = ?;"));
	}
	SetStatementForPrimaryAccountIndexOfferID(record);
	m_Impl->m_PrimaryAccountDeleteByOfferIDIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccountByOfferIDIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdatePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountUpdateStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_PrimaryAccount set PrimaryAccountName = ?, AccountClass = ?, BrokerPassword = ?, OfferID = ?, IsAllowLogin = ?, IsSimulateAccount = ?, LoginStatus = ?, InitStatus = ? where PrimaryAccountID = ?;"));
	}
	SetStatementForPrimaryAccountRecordUpdate(record);
	m_Impl->m_PrimaryAccountUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePrimaryAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPrimaryAccount(std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountSelectStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_PrimaryAccount;"));
	}
	auto result = m_Impl->m_PrimaryAccountSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPrimaryAccountWithSql(const char* sql, std::list<PrimaryAccount*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncatePrimaryAccount()
{
	auto start = steady_clock::now();
	if (m_Impl->m_PrimaryAccountTruncateStatement == nullptr)
	{
		m_Impl->m_PrimaryAccountTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_PrimaryAccount;"));
	}
	m_Impl->m_PrimaryAccountTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePrimaryAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateAccount()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Account` (`AccountID` char(32) COMMENT '账户代码', `AccountName` char(64) COMMENT '账户名称', `AccountType` int COMMENT '账户类型', `AccountStatus` int COMMENT '账户状态', `Password` char(64) COMMENT '密码', `TradeGroupID` int COMMENT '交易组代码', `RiskGroupID` int COMMENT '交易组代码', `CommissionGroupID` int COMMENT '交易组代码', PRIMARY KEY(AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_AccountCreateStatement == nullptr)
	{
		m_Impl->m_AccountCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_AccountCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateAccount Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropAccount()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Account`;";
	if (m_Impl->m_AccountDropStatement == nullptr)
	{
		m_Impl->m_AccountDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_AccountDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropAccount Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_AccountInsertStatement == nullptr)
	{
		m_Impl->m_AccountInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Account Values(?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForAccountRecordInsert(record);
	
	m_Impl->m_AccountInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertAccount(std::list<Account*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_AccountDeleteStatement == nullptr)
	{
		m_Impl->m_AccountDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Account where AccountID = ?;"));
	}
	SetStatementForAccountPrimaryKey(record->AccountID);
	m_Impl->m_AccountDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_AccountUpdateStatement == nullptr)
	{
		m_Impl->m_AccountUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Account set AccountName = ?, AccountType = ?, AccountStatus = ?, Password = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ? where AccountID = ?;"));
	}
	SetStatementForAccountRecordUpdate(record);
	m_Impl->m_AccountUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectAccount(std::list<Account*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_AccountSelectStatement == nullptr)
	{
		m_Impl->m_AccountSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Account;"));
	}
	auto result = m_Impl->m_AccountSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectAccountWithSql(const char* sql, std::list<Account*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateAccount()
{
	auto start = steady_clock::now();
	if (m_Impl->m_AccountTruncateStatement == nullptr)
	{
		m_Impl->m_AccountTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Account;"));
	}
	m_Impl->m_AccountTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateCapital()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Capital` (`TradingDay` char(16) COMMENT '交易日', `AccountID` char(32) COMMENT '账户代码', `AccountType` int COMMENT '账户类型', `Balance` decimal(24,8) COMMENT '权益', `PreBalance` decimal(24,8) COMMENT '上日权益', `Available` decimal(24,8) COMMENT '可用资金', `MarketValue` decimal(24,8) COMMENT '市值', `CashIn` decimal(24,8) COMMENT '现金收入', `CashOut` decimal(24,8) COMMENT '现金支出', `Margin` decimal(24,8) COMMENT '保证金', `Commission` decimal(24,8) COMMENT '手续费', `FrozenCash` decimal(24,8) COMMENT '冻结资金', `FrozenMargin` decimal(24,8) COMMENT '冻结保证金', `FrozenCommission` decimal(24,8) COMMENT '冻结手续费', `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏', `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏', `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏', `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏', `Deposit` decimal(24,8) COMMENT '入金', `Withdraw` decimal(24,8) COMMENT '出金', INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_CapitalCreateStatement == nullptr)
	{
		m_Impl->m_CapitalCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_CapitalCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateCapital Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropCapital()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Capital`;";
	if (m_Impl->m_CapitalDropStatement == nullptr)
	{
		m_Impl->m_CapitalDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_CapitalDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropCapital Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalInsertStatement == nullptr)
	{
		m_Impl->m_CapitalInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Capital Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForCapitalRecordInsert(record);
	
	m_Impl->m_CapitalInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertCapital Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertCapital(std::list<Capital*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertCapital Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertCapital RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalDeleteStatement == nullptr)
	{
		m_Impl->m_CapitalDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Capital where TradingDay = ? and AccountID = ?;"));
	}
	SetStatementForCapitalPrimaryKey(record->TradingDay, record->AccountID);
	m_Impl->m_CapitalDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapital Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeleteCapitalByTradingDayIndex(Capital* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalDeleteByTradingDayIndexStatement == nullptr)
	{
		m_Impl->m_CapitalDeleteByTradingDayIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Capital where TradingDay = ?;"));
	}
	SetStatementForCapitalIndexTradingDay(record);
	m_Impl->m_CapitalDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapitalByTradingDayIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalUpdateStatement == nullptr)
	{
		m_Impl->m_CapitalUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Capital set AccountType = ?, Balance = ?, PreBalance = ?, Available = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, Deposit = ?, Withdraw = ? where TradingDay = ? and AccountID = ?;"));
	}
	SetStatementForCapitalRecordUpdate(record);
	m_Impl->m_CapitalUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateCapital Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectCapital(std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalSelectStatement == nullptr)
	{
		m_Impl->m_CapitalSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Capital;"));
	}
	auto result = m_Impl->m_CapitalSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapital Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectCapitalWithSql(const char* sql, std::list<Capital*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateCapital()
{
	auto start = steady_clock::now();
	if (m_Impl->m_CapitalTruncateStatement == nullptr)
	{
		m_Impl->m_CapitalTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Capital;"));
	}
	m_Impl->m_CapitalTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateCapital Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreatePosition()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Position` (`TradingDay` char(16) COMMENT '交易日', `AccountID` char(32) COMMENT '账户代码', `AccountType` int COMMENT '账户类型', `ExchangeID` char(8) COMMENT '交易所代码', `InstrumentID` char(32) COMMENT '合约代码', `ProductClass` int COMMENT '品种类型', `PosiDirection` int COMMENT '持仓方向', `TotalPosition` bigint COMMENT '持仓数量', `PositionFrozen` bigint COMMENT '冻结持仓', `TodayPosition` bigint COMMENT '今日持仓', `MarketValue` decimal(24,8) COMMENT '市值', `CashIn` decimal(24,8) COMMENT '现金收入', `CashOut` decimal(24,8) COMMENT '现金支出', `Margin` decimal(24,8) COMMENT '保证金', `Commission` decimal(24,8) COMMENT '手续费', `FrozenCash` decimal(24,8) COMMENT '冻结资金', `FrozenMargin` decimal(24,8) COMMENT '冻结保证金', `FrozenCommission` decimal(24,8) COMMENT '冻结手续费', `VolumeMultiple` int COMMENT '合约乘数', `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏', `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏', `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏', `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏', `SettlementPrice` decimal(24,8) COMMENT '结算价', `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价', INDEX Account(TradingDay, AccountID), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_PositionCreateStatement == nullptr)
	{
		m_Impl->m_PositionCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_PositionCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePosition Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropPosition()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Position`;";
	if (m_Impl->m_PositionDropStatement == nullptr)
	{
		m_Impl->m_PositionDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_PositionDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPosition Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertPosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionInsertStatement == nullptr)
	{
		m_Impl->m_PositionInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForPositionRecordInsert(record);
	
	m_Impl->m_PositionInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPosition Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertPosition(std::list<Position*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDeleteStatement == nullptr)
	{
		m_Impl->m_PositionDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;"));
	}
	SetStatementForPositionPrimaryKey(record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection);
	m_Impl->m_PositionDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDeleteByAccountIndexStatement == nullptr)
	{
		m_Impl->m_PositionDeleteByAccountIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ? and AccountID = ?;"));
	}
	SetStatementForPositionIndexAccount(record);
	m_Impl->m_PositionDeleteByAccountIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeletePositionByTradingDayIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDeleteByTradingDayIndexStatement == nullptr)
	{
		m_Impl->m_PositionDeleteByTradingDayIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Position where TradingDay = ?;"));
	}
	SetStatementForPositionIndexTradingDay(record);
	m_Impl->m_PositionDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByTradingDayIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionUpdateStatement == nullptr)
	{
		m_Impl->m_PositionUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Position set AccountType = ?, ProductClass = ?, TotalPosition = ?, PositionFrozen = ?, TodayPosition = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;"));
	}
	SetStatementForPositionRecordUpdate(record);
	m_Impl->m_PositionUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPosition(std::list<Position*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionSelectStatement == nullptr)
	{
		m_Impl->m_PositionSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Position;"));
	}
	auto result = m_Impl->m_PositionSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPositionWithSql(const char* sql, std::list<Position*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncatePosition()
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionTruncateStatement == nullptr)
	{
		m_Impl->m_PositionTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Position;"));
	}
	m_Impl->m_PositionTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreatePositionDetail()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_PositionDetail` (`TradingDay` char(16) COMMENT '交易日', `AccountID` char(32) COMMENT '账户代码', `AccountType` int COMMENT '账户类型', `ExchangeID` char(8) COMMENT '交易所代码', `InstrumentID` char(32) COMMENT '合约代码', `ProductClass` int COMMENT '品种类型', `PosiDirection` int COMMENT '持仓方向', `OpenDate` char(16) COMMENT '开仓日期', `TradeID` char(64) COMMENT '成交编号', `Volume` bigint COMMENT '委托数量', `OpenPrice` decimal(24,8) COMMENT '开盘价', `MarketValue` decimal(24,8) COMMENT '市值', `CashIn` decimal(24,8) COMMENT '现金收入', `CashOut` decimal(24,8) COMMENT '现金支出', `Margin` decimal(24,8) COMMENT '保证金', `Commission` decimal(24,8) COMMENT '手续费', `VolumeMultiple` int COMMENT '合约乘数', `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏', `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏', `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏', `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏', `SettlementPrice` decimal(24,8) COMMENT '结算价', `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价', `CloseVolume` bigint COMMENT '平仓数量', `CloseAmount` decimal(24,8) COMMENT '平仓金额', INDEX TradeMatch(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_PositionDetailCreateStatement == nullptr)
	{
		m_Impl->m_PositionDetailCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_PositionDetailCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropPositionDetail()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_PositionDetail`;";
	if (m_Impl->m_PositionDetailDropStatement == nullptr)
	{
		m_Impl->m_PositionDetailDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_PositionDetailDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertPositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailInsertStatement == nullptr)
	{
		m_Impl->m_PositionDetailInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_PositionDetail Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForPositionDetailRecordInsert(record);
	
	m_Impl->m_PositionDetailInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPositionDetail Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertPositionDetail(std::list<PositionDetail*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPositionDetail Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPositionDetail RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeletePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailDeleteStatement == nullptr)
	{
		m_Impl->m_PositionDetailDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;"));
	}
	SetStatementForPositionDetailPrimaryKey(record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection, record->OpenDate, record->TradeID);
	m_Impl->m_PositionDetailDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetail Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeletePositionDetailByTradeMatchIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement == nullptr)
	{
		m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;"));
	}
	SetStatementForPositionDetailIndexTradeMatch(record);
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradeMatchIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::DeletePositionDetailByTradingDayIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailDeleteByTradingDayIndexStatement == nullptr)
	{
		m_Impl->m_PositionDetailDeleteByTradingDayIndexStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_PositionDetail where TradingDay = ?;"));
	}
	SetStatementForPositionDetailIndexTradingDay(record);
	m_Impl->m_PositionDetailDeleteByTradingDayIndexStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradingDayIndex Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdatePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailUpdateStatement == nullptr)
	{
		m_Impl->m_PositionDetailUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_PositionDetail set AccountType = ?, ProductClass = ?, Volume = ?, OpenPrice = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ?, CloseVolume = ?, CloseAmount = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;"));
	}
	SetStatementForPositionDetailRecordUpdate(record);
	m_Impl->m_PositionDetailUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePositionDetail Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPositionDetail(std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailSelectStatement == nullptr)
	{
		m_Impl->m_PositionDetailSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_PositionDetail;"));
	}
	auto result = m_Impl->m_PositionDetailSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetail Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectPositionDetailWithSql(const char* sql, std::list<PositionDetail*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncatePositionDetail()
{
	auto start = steady_clock::now();
	if (m_Impl->m_PositionDetailTruncateStatement == nullptr)
	{
		m_Impl->m_PositionDetailTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_PositionDetail;"));
	}
	m_Impl->m_PositionDetailTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncatePositionDetail Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateOrder()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Order` (`TradingDay` char(16) COMMENT '交易日', `AccountID` char(32) COMMENT '账户代码', `AccountType` int COMMENT '账户类型', `ExchangeID` char(8) COMMENT '交易所代码', `InstrumentID` char(32) COMMENT '合约代码', `ProductClass` int COMMENT '品种类型', `OrderID` int COMMENT '委托编号', `OrderSysID` char(64) COMMENT '系统委托编号', `Direction` int COMMENT '买卖方向', `OffsetFlag` int COMMENT '开平标志', `OrderPriceType` int COMMENT '委托价格类型', `Price` decimal(24,8) COMMENT '委托价格', `Volume` bigint COMMENT '委托数量', `VolumeTotal` bigint COMMENT '剩余数量', `VolumeTraded` bigint COMMENT '成交数量', `VolumeMultiple` int COMMENT '合约乘数', `OrderStatus` int COMMENT '委托状态', `OrderDate` char(16) COMMENT '委托日期', `OrderTime` char(16) COMMENT '委托时间', `CancelDate` char(16) COMMENT '撤单日期', `CancelTime` char(16) COMMENT '撤单时间', `SessionID` bigint COMMENT '会话编号', `ClientOrderID` int COMMENT '客户端委托编号', `RequestID` int COMMENT '客户端请求编号', `OfferID` int COMMENT '报盘代码', `TradeGroupID` int COMMENT '交易组代码', `RiskGroupID` int COMMENT '交易组代码', `CommissionGroupID` int COMMENT '交易组代码', `FrozenCash` decimal(24,8) COMMENT '冻结资金', `FrozenMargin` decimal(24,8) COMMENT '冻结保证金', `FrozenCommission` decimal(24,8) COMMENT '冻结手续费', `RebuildMark` bool COMMENT '重建标志', `IsForceClose` bool COMMENT '是否强平单', UNIQUE ClientOrderID(TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_OrderCreateStatement == nullptr)
	{
		m_Impl->m_OrderCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_OrderCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateOrder Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropOrder()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Order`;";
	if (m_Impl->m_OrderDropStatement == nullptr)
	{
		m_Impl->m_OrderDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_OrderDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropOrder Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_OrderInsertStatement == nullptr)
	{
		m_Impl->m_OrderInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForOrderRecordInsert(record);
	
	m_Impl->m_OrderInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertOrder Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertOrder(std::list<Order*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_OrderDeleteStatement == nullptr)
	{
		m_Impl->m_OrderDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Order where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;"));
	}
	SetStatementForOrderPrimaryKey(record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->OrderID);
	m_Impl->m_OrderDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_OrderUpdateStatement == nullptr)
	{
		m_Impl->m_OrderUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Order set AccountType = ?, ProductClass = ?, OrderSysID = ?, Direction = ?, OffsetFlag = ?, OrderPriceType = ?, Price = ?, Volume = ?, VolumeTotal = ?, VolumeTraded = ?, VolumeMultiple = ?, OrderStatus = ?, OrderDate = ?, OrderTime = ?, CancelDate = ?, CancelTime = ?, SessionID = ?, ClientOrderID = ?, RequestID = ?, OfferID = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, RebuildMark = ?, IsForceClose = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;"));
	}
	SetStatementForOrderRecordUpdate(record);
	m_Impl->m_OrderUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectOrder(std::list<Order*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_OrderSelectStatement == nullptr)
	{
		m_Impl->m_OrderSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Order;"));
	}
	auto result = m_Impl->m_OrderSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectOrderWithSql(const char* sql, std::list<Order*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateOrder()
{
	auto start = steady_clock::now();
	if (m_Impl->m_OrderTruncateStatement == nullptr)
	{
		m_Impl->m_OrderTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Order;"));
	}
	m_Impl->m_OrderTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void MariadbWrapper::CreateTrade()
{
	auto start = steady_clock::now();
	const char* sql = "CREATE TABLE IF NOT EXISTS `t_Trade` (`TradingDay` char(16) COMMENT '交易日', `AccountID` char(32) COMMENT '账户代码', `AccountType` int COMMENT '账户类型', `ExchangeID` char(8) COMMENT '交易所代码', `InstrumentID` char(32) COMMENT '合约代码', `ProductClass` int COMMENT '品种类型', `OrderID` int COMMENT '委托编号', `OrderSysID` char(64) COMMENT '系统委托编号', `TradeID` char(64) COMMENT '成交编号', `Direction` int COMMENT '买卖方向', `OffsetFlag` int COMMENT '开平标志', `Price` decimal(24,8) COMMENT '委托价格', `Volume` bigint COMMENT '委托数量', `VolumeMultiple` int COMMENT '合约乘数', `TradeAmount` decimal(24,8) COMMENT '成交金额', `Commission` decimal(24,8) COMMENT '手续费', `TradeDate` char(16) COMMENT '成交日期', `TradeTime` char(16) COMMENT '成交时间', PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
	if (m_Impl->m_TradeCreateStatement == nullptr)
	{
		m_Impl->m_TradeCreateStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	
	m_Impl->m_TradeCreateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTrade Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::DropTrade()
{
	auto start = steady_clock::now();
	const char* sql = "DROP TABLE IF EXISTS `t_Trade`;";
	if (m_Impl->m_TradeDropStatement == nullptr)
	{
		m_Impl->m_TradeDropStatement.reset(m_Impl->m_DBConnection->prepareStatement(sql));
	}
	m_Impl->m_TradeDropStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTrade Spend:%lldms, sql:%s", duration, sql);
}
void MariadbWrapper::InsertTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradeInsertStatement == nullptr)
	{
		m_Impl->m_TradeInsertStatement.reset(m_Impl->m_DBConnection->prepareStatement("insert into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"));
	}
	SetStatementForTradeRecordInsert(record);
	
	m_Impl->m_TradeInsertStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTrade Spend:%lldms", duration);
	}
}
void MariadbWrapper::BatchInsertTrade(std::list<Trade*>* records)
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
				m_Impl->m_Statement->executeUpdate(m_SqlBuff);
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
		m_Impl->m_Statement->executeUpdate(m_SqlBuff);
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", e.what(), m_SqlBuff);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void MariadbWrapper::DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradeDeleteStatement == nullptr)
	{
		m_Impl->m_TradeDeleteStatement.reset(m_Impl->m_DBConnection->prepareStatement("delete from t_Trade where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;"));
	}
	SetStatementForTradePrimaryKey(record->TradingDay, record->ExchangeID, record->TradeID, record->Direction);
	m_Impl->m_TradeDeleteStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void MariadbWrapper::UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradeUpdateStatement == nullptr)
	{
		m_Impl->m_TradeUpdateStatement.reset(m_Impl->m_DBConnection->prepareStatement("update t_Trade set AccountID = ?, AccountType = ?, InstrumentID = ?, ProductClass = ?, OrderID = ?, OrderSysID = ?, OffsetFlag = ?, Price = ?, Volume = ?, VolumeMultiple = ?, TradeAmount = ?, Commission = ?, TradeDate = ?, TradeTime = ? where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;"));
	}
	SetStatementForTradeRecordUpdate(record);
	m_Impl->m_TradeUpdateStatement->executeUpdate();
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectTrade(std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradeSelectStatement == nullptr)
	{
		m_Impl->m_TradeSelectStatement.reset(m_Impl->m_DBConnection->prepareStatement("select * from t_Trade;"));
	}
	auto result = m_Impl->m_TradeSelectStatement->executeQuery();
	while (result->next())
	{
		ParseRecord(result, records);
	}
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void MariadbWrapper::SelectTradeWithSql(const char* sql, std::list<Trade*>& records)
{
	if (m_Impl->m_Statement == nullptr)
	{
		m_Impl->m_Statement.reset(m_Impl->m_DBConnection->createStatement());
	}
	auto result = m_Impl->m_Statement->executeQuery(sql);
	while (result->next())
	{
		ParseRecord(result, records);
	}
}
void MariadbWrapper::TruncateTrade()
{
	auto start = steady_clock::now();
	if (m_Impl->m_TradeTruncateStatement == nullptr)
	{
		m_Impl->m_TradeTruncateStatement.reset(m_Impl->m_DBConnection->prepareStatement("truncate table t_Trade;"));
	}
	m_Impl->m_TradeTruncateStatement->executeQuery();
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}


void MariadbWrapper::SetStatementForTradingDayRecordInsert(TradingDay* record)
{
	m_Impl->m_TradingDayInsertStatement->setInt(1, record->PK);
	m_Impl->m_TradingDayInsertStatement->setString(2, record->CurrTradingDay);
	m_Impl->m_TradingDayInsertStatement->setString(3, record->PreTradingDay);
}
void MariadbWrapper::SetStatementForTradingDayRecordUpdate(TradingDay* record)
{
	m_Impl->m_TradingDayUpdateStatement->setString(1, record->CurrTradingDay);
	m_Impl->m_TradingDayUpdateStatement->setString(2, record->PreTradingDay);
	m_Impl->m_TradingDayUpdateStatement->setInt(3, record->PK);
}
void MariadbWrapper::SetStatementForTradingDayPrimaryKey(const IntType& PK)
{
	m_Impl->m_TradingDayDeleteStatement->setInt(1, PK);
}
void MariadbWrapper::SetStatementForExchangeRecordInsert(Exchange* record)
{
	m_Impl->m_ExchangeInsertStatement->setString(1, record->ExchangeID);
	m_Impl->m_ExchangeInsertStatement->setString(2, record->ExchangeName);
}
void MariadbWrapper::SetStatementForExchangeRecordUpdate(Exchange* record)
{
	m_Impl->m_ExchangeUpdateStatement->setString(1, record->ExchangeName);
	m_Impl->m_ExchangeUpdateStatement->setString(2, record->ExchangeID);
}
void MariadbWrapper::SetStatementForExchangePrimaryKey(const ExchangeIDType& ExchangeID)
{
	m_Impl->m_ExchangeDeleteStatement->setString(1, ExchangeID);
}
void MariadbWrapper::SetStatementForProductRecordInsert(Product* record)
{
	m_Impl->m_ProductInsertStatement->setString(1, record->ExchangeID);
	m_Impl->m_ProductInsertStatement->setString(2, record->ProductID);
	m_Impl->m_ProductInsertStatement->setString(3, record->ProductName);
	m_Impl->m_ProductInsertStatement->setInt(4, int(record->ProductClass));
	m_Impl->m_ProductInsertStatement->setInt(5, record->VolumeMultiple);
	m_Impl->m_ProductInsertStatement->setDouble(6, record->PriceTick);
	m_Impl->m_ProductInsertStatement->setInt64(7, record->MaxMarketOrderVolume);
	m_Impl->m_ProductInsertStatement->setInt64(8, record->MinMarketOrderVolume);
	m_Impl->m_ProductInsertStatement->setInt64(9, record->MaxLimitOrderVolume);
	m_Impl->m_ProductInsertStatement->setInt64(10, record->MinLimitOrderVolume);
	m_Impl->m_ProductInsertStatement->setString(11, record->SessionName);
}
void MariadbWrapper::SetStatementForProductRecordUpdate(Product* record)
{
	m_Impl->m_ProductUpdateStatement->setString(1, record->ProductName);
	m_Impl->m_ProductUpdateStatement->setInt(2, int(record->ProductClass));
	m_Impl->m_ProductUpdateStatement->setInt(3, record->VolumeMultiple);
	m_Impl->m_ProductUpdateStatement->setDouble(4, record->PriceTick);
	m_Impl->m_ProductUpdateStatement->setInt64(5, record->MaxMarketOrderVolume);
	m_Impl->m_ProductUpdateStatement->setInt64(6, record->MinMarketOrderVolume);
	m_Impl->m_ProductUpdateStatement->setInt64(7, record->MaxLimitOrderVolume);
	m_Impl->m_ProductUpdateStatement->setInt64(8, record->MinLimitOrderVolume);
	m_Impl->m_ProductUpdateStatement->setString(9, record->SessionName);
	m_Impl->m_ProductUpdateStatement->setString(10, record->ExchangeID);
	m_Impl->m_ProductUpdateStatement->setString(11, record->ProductID);
}
void MariadbWrapper::SetStatementForProductPrimaryKey(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	m_Impl->m_ProductDeleteStatement->setString(1, ExchangeID);
	m_Impl->m_ProductDeleteStatement->setString(2, ProductID);
}
void MariadbWrapper::SetStatementForInstrumentRecordInsert(Instrument* record)
{
	m_Impl->m_InstrumentInsertStatement->setString(1, record->ExchangeID);
	m_Impl->m_InstrumentInsertStatement->setString(2, record->InstrumentID);
	m_Impl->m_InstrumentInsertStatement->setString(3, record->ExchangeInstID);
	m_Impl->m_InstrumentInsertStatement->setString(4, record->InstrumentName);
	m_Impl->m_InstrumentInsertStatement->setString(5, record->ProductID);
	m_Impl->m_InstrumentInsertStatement->setInt(6, int(record->ProductClass));
	m_Impl->m_InstrumentInsertStatement->setInt(7, int(record->InstrumentClass));
	m_Impl->m_InstrumentInsertStatement->setInt(8, record->Rank);
	m_Impl->m_InstrumentInsertStatement->setInt(9, record->VolumeMultiple);
	m_Impl->m_InstrumentInsertStatement->setDouble(10, record->PriceTick);
	m_Impl->m_InstrumentInsertStatement->setInt64(11, record->MaxMarketOrderVolume);
	m_Impl->m_InstrumentInsertStatement->setInt64(12, record->MinMarketOrderVolume);
	m_Impl->m_InstrumentInsertStatement->setInt64(13, record->MaxLimitOrderVolume);
	m_Impl->m_InstrumentInsertStatement->setInt64(14, record->MinLimitOrderVolume);
	m_Impl->m_InstrumentInsertStatement->setString(15, record->SessionName);
}
void MariadbWrapper::SetStatementForInstrumentRecordUpdate(Instrument* record)
{
	m_Impl->m_InstrumentUpdateStatement->setString(1, record->ExchangeInstID);
	m_Impl->m_InstrumentUpdateStatement->setString(2, record->InstrumentName);
	m_Impl->m_InstrumentUpdateStatement->setString(3, record->ProductID);
	m_Impl->m_InstrumentUpdateStatement->setInt(4, int(record->ProductClass));
	m_Impl->m_InstrumentUpdateStatement->setInt(5, int(record->InstrumentClass));
	m_Impl->m_InstrumentUpdateStatement->setInt(6, record->Rank);
	m_Impl->m_InstrumentUpdateStatement->setInt(7, record->VolumeMultiple);
	m_Impl->m_InstrumentUpdateStatement->setDouble(8, record->PriceTick);
	m_Impl->m_InstrumentUpdateStatement->setInt64(9, record->MaxMarketOrderVolume);
	m_Impl->m_InstrumentUpdateStatement->setInt64(10, record->MinMarketOrderVolume);
	m_Impl->m_InstrumentUpdateStatement->setInt64(11, record->MaxLimitOrderVolume);
	m_Impl->m_InstrumentUpdateStatement->setInt64(12, record->MinLimitOrderVolume);
	m_Impl->m_InstrumentUpdateStatement->setString(13, record->SessionName);
	m_Impl->m_InstrumentUpdateStatement->setString(14, record->ExchangeID);
	m_Impl->m_InstrumentUpdateStatement->setString(15, record->InstrumentID);
}
void MariadbWrapper::SetStatementForInstrumentPrimaryKey(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
{
	m_Impl->m_InstrumentDeleteStatement->setString(1, ExchangeID);
	m_Impl->m_InstrumentDeleteStatement->setString(2, InstrumentID);
}
void MariadbWrapper::SetStatementForPrimaryAccountRecordInsert(PrimaryAccount* record)
{
	m_Impl->m_PrimaryAccountInsertStatement->setString(1, record->PrimaryAccountID);
	m_Impl->m_PrimaryAccountInsertStatement->setString(2, record->PrimaryAccountName);
	m_Impl->m_PrimaryAccountInsertStatement->setInt(3, int(record->AccountClass));
	m_Impl->m_PrimaryAccountInsertStatement->setString(4, record->BrokerPassword);
	m_Impl->m_PrimaryAccountInsertStatement->setInt(5, record->OfferID);
	m_Impl->m_PrimaryAccountInsertStatement->setBoolean(6, record->IsAllowLogin);
	m_Impl->m_PrimaryAccountInsertStatement->setBoolean(7, record->IsSimulateAccount);
	m_Impl->m_PrimaryAccountInsertStatement->setInt(8, int(record->LoginStatus));
	m_Impl->m_PrimaryAccountInsertStatement->setInt(9, int(record->InitStatus));
}
void MariadbWrapper::SetStatementForPrimaryAccountRecordUpdate(PrimaryAccount* record)
{
	m_Impl->m_PrimaryAccountUpdateStatement->setString(1, record->PrimaryAccountName);
	m_Impl->m_PrimaryAccountUpdateStatement->setInt(2, int(record->AccountClass));
	m_Impl->m_PrimaryAccountUpdateStatement->setString(3, record->BrokerPassword);
	m_Impl->m_PrimaryAccountUpdateStatement->setInt(4, record->OfferID);
	m_Impl->m_PrimaryAccountUpdateStatement->setBoolean(5, record->IsAllowLogin);
	m_Impl->m_PrimaryAccountUpdateStatement->setBoolean(6, record->IsSimulateAccount);
	m_Impl->m_PrimaryAccountUpdateStatement->setInt(7, int(record->LoginStatus));
	m_Impl->m_PrimaryAccountUpdateStatement->setInt(8, int(record->InitStatus));
	m_Impl->m_PrimaryAccountUpdateStatement->setString(9, record->PrimaryAccountID);
}
void MariadbWrapper::SetStatementForPrimaryAccountPrimaryKey(const AccountIDType& PrimaryAccountID)
{
	m_Impl->m_PrimaryAccountDeleteStatement->setString(1, PrimaryAccountID);
}
void MariadbWrapper::SetStatementForPrimaryAccountIndexOfferID(PrimaryAccount* record)
{
	m_Impl->m_PrimaryAccountDeleteByOfferIDIndexStatement->setInt(1, record->OfferID);
}
void MariadbWrapper::SetStatementForAccountRecordInsert(Account* record)
{
	m_Impl->m_AccountInsertStatement->setString(1, record->AccountID);
	m_Impl->m_AccountInsertStatement->setString(2, record->AccountName);
	m_Impl->m_AccountInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_AccountInsertStatement->setInt(4, int(record->AccountStatus));
	m_Impl->m_AccountInsertStatement->setString(5, record->Password);
	m_Impl->m_AccountInsertStatement->setInt(6, record->TradeGroupID);
	m_Impl->m_AccountInsertStatement->setInt(7, record->RiskGroupID);
	m_Impl->m_AccountInsertStatement->setInt(8, record->CommissionGroupID);
}
void MariadbWrapper::SetStatementForAccountRecordUpdate(Account* record)
{
	m_Impl->m_AccountUpdateStatement->setString(1, record->AccountName);
	m_Impl->m_AccountUpdateStatement->setInt(2, int(record->AccountType));
	m_Impl->m_AccountUpdateStatement->setInt(3, int(record->AccountStatus));
	m_Impl->m_AccountUpdateStatement->setString(4, record->Password);
	m_Impl->m_AccountUpdateStatement->setInt(5, record->TradeGroupID);
	m_Impl->m_AccountUpdateStatement->setInt(6, record->RiskGroupID);
	m_Impl->m_AccountUpdateStatement->setInt(7, record->CommissionGroupID);
	m_Impl->m_AccountUpdateStatement->setString(8, record->AccountID);
}
void MariadbWrapper::SetStatementForAccountPrimaryKey(const AccountIDType& AccountID)
{
	m_Impl->m_AccountDeleteStatement->setString(1, AccountID);
}
void MariadbWrapper::SetStatementForCapitalRecordInsert(Capital* record)
{
	m_Impl->m_CapitalInsertStatement->setString(1, record->TradingDay);
	m_Impl->m_CapitalInsertStatement->setString(2, record->AccountID);
	m_Impl->m_CapitalInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_CapitalInsertStatement->setDouble(4, record->Balance);
	m_Impl->m_CapitalInsertStatement->setDouble(5, record->PreBalance);
	m_Impl->m_CapitalInsertStatement->setDouble(6, record->Available);
	m_Impl->m_CapitalInsertStatement->setDouble(7, record->MarketValue);
	m_Impl->m_CapitalInsertStatement->setDouble(8, record->CashIn);
	m_Impl->m_CapitalInsertStatement->setDouble(9, record->CashOut);
	m_Impl->m_CapitalInsertStatement->setDouble(10, record->Margin);
	m_Impl->m_CapitalInsertStatement->setDouble(11, record->Commission);
	m_Impl->m_CapitalInsertStatement->setDouble(12, record->FrozenCash);
	m_Impl->m_CapitalInsertStatement->setDouble(13, record->FrozenMargin);
	m_Impl->m_CapitalInsertStatement->setDouble(14, record->FrozenCommission);
	m_Impl->m_CapitalInsertStatement->setDouble(15, record->CloseProfitByDate);
	m_Impl->m_CapitalInsertStatement->setDouble(16, record->CloseProfitByTrade);
	m_Impl->m_CapitalInsertStatement->setDouble(17, record->PositionProfitByDate);
	m_Impl->m_CapitalInsertStatement->setDouble(18, record->PositionProfitByTrade);
	m_Impl->m_CapitalInsertStatement->setDouble(19, record->Deposit);
	m_Impl->m_CapitalInsertStatement->setDouble(20, record->Withdraw);
}
void MariadbWrapper::SetStatementForCapitalRecordUpdate(Capital* record)
{
	m_Impl->m_CapitalUpdateStatement->setInt(1, int(record->AccountType));
	m_Impl->m_CapitalUpdateStatement->setDouble(2, record->Balance);
	m_Impl->m_CapitalUpdateStatement->setDouble(3, record->PreBalance);
	m_Impl->m_CapitalUpdateStatement->setDouble(4, record->Available);
	m_Impl->m_CapitalUpdateStatement->setDouble(5, record->MarketValue);
	m_Impl->m_CapitalUpdateStatement->setDouble(6, record->CashIn);
	m_Impl->m_CapitalUpdateStatement->setDouble(7, record->CashOut);
	m_Impl->m_CapitalUpdateStatement->setDouble(8, record->Margin);
	m_Impl->m_CapitalUpdateStatement->setDouble(9, record->Commission);
	m_Impl->m_CapitalUpdateStatement->setDouble(10, record->FrozenCash);
	m_Impl->m_CapitalUpdateStatement->setDouble(11, record->FrozenMargin);
	m_Impl->m_CapitalUpdateStatement->setDouble(12, record->FrozenCommission);
	m_Impl->m_CapitalUpdateStatement->setDouble(13, record->CloseProfitByDate);
	m_Impl->m_CapitalUpdateStatement->setDouble(14, record->CloseProfitByTrade);
	m_Impl->m_CapitalUpdateStatement->setDouble(15, record->PositionProfitByDate);
	m_Impl->m_CapitalUpdateStatement->setDouble(16, record->PositionProfitByTrade);
	m_Impl->m_CapitalUpdateStatement->setDouble(17, record->Deposit);
	m_Impl->m_CapitalUpdateStatement->setDouble(18, record->Withdraw);
	m_Impl->m_CapitalUpdateStatement->setString(19, record->TradingDay);
	m_Impl->m_CapitalUpdateStatement->setString(20, record->AccountID);
}
void MariadbWrapper::SetStatementForCapitalPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID)
{
	m_Impl->m_CapitalDeleteStatement->setString(1, TradingDay);
	m_Impl->m_CapitalDeleteStatement->setString(2, AccountID);
}
void MariadbWrapper::SetStatementForCapitalIndexTradingDay(Capital* record)
{
	m_Impl->m_CapitalDeleteByTradingDayIndexStatement->setString(1, record->TradingDay);
}
void MariadbWrapper::SetStatementForPositionRecordInsert(Position* record)
{
	m_Impl->m_PositionInsertStatement->setString(1, record->TradingDay);
	m_Impl->m_PositionInsertStatement->setString(2, record->AccountID);
	m_Impl->m_PositionInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_PositionInsertStatement->setString(4, record->ExchangeID);
	m_Impl->m_PositionInsertStatement->setString(5, record->InstrumentID);
	m_Impl->m_PositionInsertStatement->setInt(6, int(record->ProductClass));
	m_Impl->m_PositionInsertStatement->setInt(7, int(record->PosiDirection));
	m_Impl->m_PositionInsertStatement->setInt64(8, record->TotalPosition);
	m_Impl->m_PositionInsertStatement->setInt64(9, record->PositionFrozen);
	m_Impl->m_PositionInsertStatement->setInt64(10, record->TodayPosition);
	m_Impl->m_PositionInsertStatement->setDouble(11, record->MarketValue);
	m_Impl->m_PositionInsertStatement->setDouble(12, record->CashIn);
	m_Impl->m_PositionInsertStatement->setDouble(13, record->CashOut);
	m_Impl->m_PositionInsertStatement->setDouble(14, record->Margin);
	m_Impl->m_PositionInsertStatement->setDouble(15, record->Commission);
	m_Impl->m_PositionInsertStatement->setDouble(16, record->FrozenCash);
	m_Impl->m_PositionInsertStatement->setDouble(17, record->FrozenMargin);
	m_Impl->m_PositionInsertStatement->setDouble(18, record->FrozenCommission);
	m_Impl->m_PositionInsertStatement->setInt(19, record->VolumeMultiple);
	m_Impl->m_PositionInsertStatement->setDouble(20, record->CloseProfitByDate);
	m_Impl->m_PositionInsertStatement->setDouble(21, record->CloseProfitByTrade);
	m_Impl->m_PositionInsertStatement->setDouble(22, record->PositionProfitByDate);
	m_Impl->m_PositionInsertStatement->setDouble(23, record->PositionProfitByTrade);
	m_Impl->m_PositionInsertStatement->setDouble(24, record->SettlementPrice);
	m_Impl->m_PositionInsertStatement->setDouble(25, record->PreSettlementPrice);
}
void MariadbWrapper::SetStatementForPositionRecordUpdate(Position* record)
{
	m_Impl->m_PositionUpdateStatement->setInt(1, int(record->AccountType));
	m_Impl->m_PositionUpdateStatement->setInt(2, int(record->ProductClass));
	m_Impl->m_PositionUpdateStatement->setInt64(3, record->TotalPosition);
	m_Impl->m_PositionUpdateStatement->setInt64(4, record->PositionFrozen);
	m_Impl->m_PositionUpdateStatement->setInt64(5, record->TodayPosition);
	m_Impl->m_PositionUpdateStatement->setDouble(6, record->MarketValue);
	m_Impl->m_PositionUpdateStatement->setDouble(7, record->CashIn);
	m_Impl->m_PositionUpdateStatement->setDouble(8, record->CashOut);
	m_Impl->m_PositionUpdateStatement->setDouble(9, record->Margin);
	m_Impl->m_PositionUpdateStatement->setDouble(10, record->Commission);
	m_Impl->m_PositionUpdateStatement->setDouble(11, record->FrozenCash);
	m_Impl->m_PositionUpdateStatement->setDouble(12, record->FrozenMargin);
	m_Impl->m_PositionUpdateStatement->setDouble(13, record->FrozenCommission);
	m_Impl->m_PositionUpdateStatement->setInt(14, record->VolumeMultiple);
	m_Impl->m_PositionUpdateStatement->setDouble(15, record->CloseProfitByDate);
	m_Impl->m_PositionUpdateStatement->setDouble(16, record->CloseProfitByTrade);
	m_Impl->m_PositionUpdateStatement->setDouble(17, record->PositionProfitByDate);
	m_Impl->m_PositionUpdateStatement->setDouble(18, record->PositionProfitByTrade);
	m_Impl->m_PositionUpdateStatement->setDouble(19, record->SettlementPrice);
	m_Impl->m_PositionUpdateStatement->setDouble(20, record->PreSettlementPrice);
	m_Impl->m_PositionUpdateStatement->setString(21, record->TradingDay);
	m_Impl->m_PositionUpdateStatement->setString(22, record->AccountID);
	m_Impl->m_PositionUpdateStatement->setString(23, record->ExchangeID);
	m_Impl->m_PositionUpdateStatement->setString(24, record->InstrumentID);
	m_Impl->m_PositionUpdateStatement->setInt(25, int(record->PosiDirection));
}
void MariadbWrapper::SetStatementForPositionPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
{
	m_Impl->m_PositionDeleteStatement->setString(1, TradingDay);
	m_Impl->m_PositionDeleteStatement->setString(2, AccountID);
	m_Impl->m_PositionDeleteStatement->setString(3, ExchangeID);
	m_Impl->m_PositionDeleteStatement->setString(4, InstrumentID);
	m_Impl->m_PositionDeleteStatement->setInt(5, int(PosiDirection));
}
void MariadbWrapper::SetStatementForPositionIndexAccount(Position* record)
{
	m_Impl->m_PositionDeleteByAccountIndexStatement->setString(1, record->TradingDay);
	m_Impl->m_PositionDeleteByAccountIndexStatement->setString(2, record->AccountID);
}
void MariadbWrapper::SetStatementForPositionIndexTradingDay(Position* record)
{
	m_Impl->m_PositionDeleteByTradingDayIndexStatement->setString(1, record->TradingDay);
}
void MariadbWrapper::SetStatementForPositionDetailRecordInsert(PositionDetail* record)
{
	m_Impl->m_PositionDetailInsertStatement->setString(1, record->TradingDay);
	m_Impl->m_PositionDetailInsertStatement->setString(2, record->AccountID);
	m_Impl->m_PositionDetailInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_PositionDetailInsertStatement->setString(4, record->ExchangeID);
	m_Impl->m_PositionDetailInsertStatement->setString(5, record->InstrumentID);
	m_Impl->m_PositionDetailInsertStatement->setInt(6, int(record->ProductClass));
	m_Impl->m_PositionDetailInsertStatement->setInt(7, int(record->PosiDirection));
	m_Impl->m_PositionDetailInsertStatement->setString(8, record->OpenDate);
	m_Impl->m_PositionDetailInsertStatement->setString(9, record->TradeID);
	m_Impl->m_PositionDetailInsertStatement->setInt64(10, record->Volume);
	m_Impl->m_PositionDetailInsertStatement->setDouble(11, record->OpenPrice);
	m_Impl->m_PositionDetailInsertStatement->setDouble(12, record->MarketValue);
	m_Impl->m_PositionDetailInsertStatement->setDouble(13, record->CashIn);
	m_Impl->m_PositionDetailInsertStatement->setDouble(14, record->CashOut);
	m_Impl->m_PositionDetailInsertStatement->setDouble(15, record->Margin);
	m_Impl->m_PositionDetailInsertStatement->setDouble(16, record->Commission);
	m_Impl->m_PositionDetailInsertStatement->setInt(17, record->VolumeMultiple);
	m_Impl->m_PositionDetailInsertStatement->setDouble(18, record->CloseProfitByDate);
	m_Impl->m_PositionDetailInsertStatement->setDouble(19, record->CloseProfitByTrade);
	m_Impl->m_PositionDetailInsertStatement->setDouble(20, record->PositionProfitByDate);
	m_Impl->m_PositionDetailInsertStatement->setDouble(21, record->PositionProfitByTrade);
	m_Impl->m_PositionDetailInsertStatement->setDouble(22, record->SettlementPrice);
	m_Impl->m_PositionDetailInsertStatement->setDouble(23, record->PreSettlementPrice);
	m_Impl->m_PositionDetailInsertStatement->setInt64(24, record->CloseVolume);
	m_Impl->m_PositionDetailInsertStatement->setDouble(25, record->CloseAmount);
}
void MariadbWrapper::SetStatementForPositionDetailRecordUpdate(PositionDetail* record)
{
	m_Impl->m_PositionDetailUpdateStatement->setInt(1, int(record->AccountType));
	m_Impl->m_PositionDetailUpdateStatement->setInt(2, int(record->ProductClass));
	m_Impl->m_PositionDetailUpdateStatement->setInt64(3, record->Volume);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(4, record->OpenPrice);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(5, record->MarketValue);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(6, record->CashIn);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(7, record->CashOut);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(8, record->Margin);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(9, record->Commission);
	m_Impl->m_PositionDetailUpdateStatement->setInt(10, record->VolumeMultiple);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(11, record->CloseProfitByDate);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(12, record->CloseProfitByTrade);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(13, record->PositionProfitByDate);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(14, record->PositionProfitByTrade);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(15, record->SettlementPrice);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(16, record->PreSettlementPrice);
	m_Impl->m_PositionDetailUpdateStatement->setInt64(17, record->CloseVolume);
	m_Impl->m_PositionDetailUpdateStatement->setDouble(18, record->CloseAmount);
	m_Impl->m_PositionDetailUpdateStatement->setString(19, record->TradingDay);
	m_Impl->m_PositionDetailUpdateStatement->setString(20, record->AccountID);
	m_Impl->m_PositionDetailUpdateStatement->setString(21, record->ExchangeID);
	m_Impl->m_PositionDetailUpdateStatement->setString(22, record->InstrumentID);
	m_Impl->m_PositionDetailUpdateStatement->setInt(23, int(record->PosiDirection));
	m_Impl->m_PositionDetailUpdateStatement->setString(24, record->OpenDate);
	m_Impl->m_PositionDetailUpdateStatement->setString(25, record->TradeID);
}
void MariadbWrapper::SetStatementForPositionDetailPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID)
{
	m_Impl->m_PositionDetailDeleteStatement->setString(1, TradingDay);
	m_Impl->m_PositionDetailDeleteStatement->setString(2, AccountID);
	m_Impl->m_PositionDetailDeleteStatement->setString(3, ExchangeID);
	m_Impl->m_PositionDetailDeleteStatement->setString(4, InstrumentID);
	m_Impl->m_PositionDetailDeleteStatement->setInt(5, int(PosiDirection));
	m_Impl->m_PositionDetailDeleteStatement->setString(6, OpenDate);
	m_Impl->m_PositionDetailDeleteStatement->setString(7, TradeID);
}
void MariadbWrapper::SetStatementForPositionDetailIndexTradeMatch(PositionDetail* record)
{
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->setString(1, record->TradingDay);
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->setString(2, record->AccountID);
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->setString(3, record->ExchangeID);
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->setString(4, record->InstrumentID);
	m_Impl->m_PositionDetailDeleteByTradeMatchIndexStatement->setInt(5, int(record->PosiDirection));
}
void MariadbWrapper::SetStatementForPositionDetailIndexTradingDay(PositionDetail* record)
{
	m_Impl->m_PositionDetailDeleteByTradingDayIndexStatement->setString(1, record->TradingDay);
}
void MariadbWrapper::SetStatementForOrderRecordInsert(Order* record)
{
	m_Impl->m_OrderInsertStatement->setString(1, record->TradingDay);
	m_Impl->m_OrderInsertStatement->setString(2, record->AccountID);
	m_Impl->m_OrderInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_OrderInsertStatement->setString(4, record->ExchangeID);
	m_Impl->m_OrderInsertStatement->setString(5, record->InstrumentID);
	m_Impl->m_OrderInsertStatement->setInt(6, int(record->ProductClass));
	m_Impl->m_OrderInsertStatement->setInt(7, record->OrderID);
	m_Impl->m_OrderInsertStatement->setString(8, record->OrderSysID);
	m_Impl->m_OrderInsertStatement->setInt(9, int(record->Direction));
	m_Impl->m_OrderInsertStatement->setInt(10, int(record->OffsetFlag));
	m_Impl->m_OrderInsertStatement->setInt(11, int(record->OrderPriceType));
	m_Impl->m_OrderInsertStatement->setDouble(12, record->Price);
	m_Impl->m_OrderInsertStatement->setInt64(13, record->Volume);
	m_Impl->m_OrderInsertStatement->setInt64(14, record->VolumeTotal);
	m_Impl->m_OrderInsertStatement->setInt64(15, record->VolumeTraded);
	m_Impl->m_OrderInsertStatement->setInt(16, record->VolumeMultiple);
	m_Impl->m_OrderInsertStatement->setInt(17, int(record->OrderStatus));
	m_Impl->m_OrderInsertStatement->setString(18, record->OrderDate);
	m_Impl->m_OrderInsertStatement->setString(19, record->OrderTime);
	m_Impl->m_OrderInsertStatement->setString(20, record->CancelDate);
	m_Impl->m_OrderInsertStatement->setString(21, record->CancelTime);
	m_Impl->m_OrderInsertStatement->setInt64(22, record->SessionID);
	m_Impl->m_OrderInsertStatement->setInt(23, record->ClientOrderID);
	m_Impl->m_OrderInsertStatement->setInt(24, record->RequestID);
	m_Impl->m_OrderInsertStatement->setInt(25, record->OfferID);
	m_Impl->m_OrderInsertStatement->setInt(26, record->TradeGroupID);
	m_Impl->m_OrderInsertStatement->setInt(27, record->RiskGroupID);
	m_Impl->m_OrderInsertStatement->setInt(28, record->CommissionGroupID);
	m_Impl->m_OrderInsertStatement->setDouble(29, record->FrozenCash);
	m_Impl->m_OrderInsertStatement->setDouble(30, record->FrozenMargin);
	m_Impl->m_OrderInsertStatement->setDouble(31, record->FrozenCommission);
	m_Impl->m_OrderInsertStatement->setBoolean(32, record->RebuildMark);
	m_Impl->m_OrderInsertStatement->setBoolean(33, record->IsForceClose);
}
void MariadbWrapper::SetStatementForOrderRecordUpdate(Order* record)
{
	m_Impl->m_OrderUpdateStatement->setInt(1, int(record->AccountType));
	m_Impl->m_OrderUpdateStatement->setInt(2, int(record->ProductClass));
	m_Impl->m_OrderUpdateStatement->setString(3, record->OrderSysID);
	m_Impl->m_OrderUpdateStatement->setInt(4, int(record->Direction));
	m_Impl->m_OrderUpdateStatement->setInt(5, int(record->OffsetFlag));
	m_Impl->m_OrderUpdateStatement->setInt(6, int(record->OrderPriceType));
	m_Impl->m_OrderUpdateStatement->setDouble(7, record->Price);
	m_Impl->m_OrderUpdateStatement->setInt64(8, record->Volume);
	m_Impl->m_OrderUpdateStatement->setInt64(9, record->VolumeTotal);
	m_Impl->m_OrderUpdateStatement->setInt64(10, record->VolumeTraded);
	m_Impl->m_OrderUpdateStatement->setInt(11, record->VolumeMultiple);
	m_Impl->m_OrderUpdateStatement->setInt(12, int(record->OrderStatus));
	m_Impl->m_OrderUpdateStatement->setString(13, record->OrderDate);
	m_Impl->m_OrderUpdateStatement->setString(14, record->OrderTime);
	m_Impl->m_OrderUpdateStatement->setString(15, record->CancelDate);
	m_Impl->m_OrderUpdateStatement->setString(16, record->CancelTime);
	m_Impl->m_OrderUpdateStatement->setInt64(17, record->SessionID);
	m_Impl->m_OrderUpdateStatement->setInt(18, record->ClientOrderID);
	m_Impl->m_OrderUpdateStatement->setInt(19, record->RequestID);
	m_Impl->m_OrderUpdateStatement->setInt(20, record->OfferID);
	m_Impl->m_OrderUpdateStatement->setInt(21, record->TradeGroupID);
	m_Impl->m_OrderUpdateStatement->setInt(22, record->RiskGroupID);
	m_Impl->m_OrderUpdateStatement->setInt(23, record->CommissionGroupID);
	m_Impl->m_OrderUpdateStatement->setDouble(24, record->FrozenCash);
	m_Impl->m_OrderUpdateStatement->setDouble(25, record->FrozenMargin);
	m_Impl->m_OrderUpdateStatement->setDouble(26, record->FrozenCommission);
	m_Impl->m_OrderUpdateStatement->setBoolean(27, record->RebuildMark);
	m_Impl->m_OrderUpdateStatement->setBoolean(28, record->IsForceClose);
	m_Impl->m_OrderUpdateStatement->setString(29, record->TradingDay);
	m_Impl->m_OrderUpdateStatement->setString(30, record->AccountID);
	m_Impl->m_OrderUpdateStatement->setString(31, record->ExchangeID);
	m_Impl->m_OrderUpdateStatement->setString(32, record->InstrumentID);
	m_Impl->m_OrderUpdateStatement->setInt(33, record->OrderID);
}
void MariadbWrapper::SetStatementForOrderPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
{
	m_Impl->m_OrderDeleteStatement->setString(1, TradingDay);
	m_Impl->m_OrderDeleteStatement->setString(2, AccountID);
	m_Impl->m_OrderDeleteStatement->setString(3, ExchangeID);
	m_Impl->m_OrderDeleteStatement->setString(4, InstrumentID);
	m_Impl->m_OrderDeleteStatement->setInt(5, OrderID);
}
void MariadbWrapper::SetStatementForTradeRecordInsert(Trade* record)
{
	m_Impl->m_TradeInsertStatement->setString(1, record->TradingDay);
	m_Impl->m_TradeInsertStatement->setString(2, record->AccountID);
	m_Impl->m_TradeInsertStatement->setInt(3, int(record->AccountType));
	m_Impl->m_TradeInsertStatement->setString(4, record->ExchangeID);
	m_Impl->m_TradeInsertStatement->setString(5, record->InstrumentID);
	m_Impl->m_TradeInsertStatement->setInt(6, int(record->ProductClass));
	m_Impl->m_TradeInsertStatement->setInt(7, record->OrderID);
	m_Impl->m_TradeInsertStatement->setString(8, record->OrderSysID);
	m_Impl->m_TradeInsertStatement->setString(9, record->TradeID);
	m_Impl->m_TradeInsertStatement->setInt(10, int(record->Direction));
	m_Impl->m_TradeInsertStatement->setInt(11, int(record->OffsetFlag));
	m_Impl->m_TradeInsertStatement->setDouble(12, record->Price);
	m_Impl->m_TradeInsertStatement->setInt64(13, record->Volume);
	m_Impl->m_TradeInsertStatement->setInt(14, record->VolumeMultiple);
	m_Impl->m_TradeInsertStatement->setDouble(15, record->TradeAmount);
	m_Impl->m_TradeInsertStatement->setDouble(16, record->Commission);
	m_Impl->m_TradeInsertStatement->setString(17, record->TradeDate);
	m_Impl->m_TradeInsertStatement->setString(18, record->TradeTime);
}
void MariadbWrapper::SetStatementForTradeRecordUpdate(Trade* record)
{
	m_Impl->m_TradeUpdateStatement->setString(1, record->AccountID);
	m_Impl->m_TradeUpdateStatement->setInt(2, int(record->AccountType));
	m_Impl->m_TradeUpdateStatement->setString(3, record->InstrumentID);
	m_Impl->m_TradeUpdateStatement->setInt(4, int(record->ProductClass));
	m_Impl->m_TradeUpdateStatement->setInt(5, record->OrderID);
	m_Impl->m_TradeUpdateStatement->setString(6, record->OrderSysID);
	m_Impl->m_TradeUpdateStatement->setInt(7, int(record->OffsetFlag));
	m_Impl->m_TradeUpdateStatement->setDouble(8, record->Price);
	m_Impl->m_TradeUpdateStatement->setInt64(9, record->Volume);
	m_Impl->m_TradeUpdateStatement->setInt(10, record->VolumeMultiple);
	m_Impl->m_TradeUpdateStatement->setDouble(11, record->TradeAmount);
	m_Impl->m_TradeUpdateStatement->setDouble(12, record->Commission);
	m_Impl->m_TradeUpdateStatement->setString(13, record->TradeDate);
	m_Impl->m_TradeUpdateStatement->setString(14, record->TradeTime);
	m_Impl->m_TradeUpdateStatement->setString(15, record->TradingDay);
	m_Impl->m_TradeUpdateStatement->setString(16, record->ExchangeID);
	m_Impl->m_TradeUpdateStatement->setString(17, record->TradeID);
	m_Impl->m_TradeUpdateStatement->setInt(18, int(record->Direction));
}
void MariadbWrapper::SetStatementForTradePrimaryKey(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
{
	m_Impl->m_TradeDeleteStatement->setString(1, TradingDay);
	m_Impl->m_TradeDeleteStatement->setString(2, ExchangeID);
	m_Impl->m_TradeDeleteStatement->setString(3, TradeID);
	m_Impl->m_TradeDeleteStatement->setInt(4, int(Direction));
}

