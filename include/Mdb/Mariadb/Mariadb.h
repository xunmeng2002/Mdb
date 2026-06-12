#pragma once
#include "Mdb/Mdb/DB.h"
#include <mariadb/conncpp.hpp>
#include <string>
#include <memory>
#include <list>

struct SqlConnectionDeleter {
    void operator()(sql::Connection* conn) const {
        if (conn) {
            conn->close();
            delete conn;
        }
    }
};
struct SqlStatementDeleter {
    void operator()(sql::Statement* stmt) const {
        if (stmt) {
            stmt->close();
            delete stmt;
        }
    }
};
struct SqlPreparedStatementDeleter {
    void operator()(sql::PreparedStatement* pstmt) const {
        if (pstmt) {
            pstmt->close();
            delete pstmt;
        }
    }
};


class Mariadb : public DB
{
public:
	Mariadb(const std::string& host, const std::string& user, const std::string& passwd);
	~Mariadb();
	virtual bool Connect() override;
	virtual void DisConnect() override;
	virtual void InitDB() override;
	virtual void CreateTables() override;
	virtual void DropTables() override;
	virtual void TruncateTables() override;
	virtual void TruncateSessionTables() override;
	
	template<typename T>
	void CustomSelectSql(const char* sql, std::list<T*>& records)
	{
		if (m_Statement == nullptr)
		{
			m_Statement.reset(m_DBConnection->createStatement());
		}
		auto result = m_Statement->executeQuery(sql);
		while (result->next())
		{
			ParseRecord(result, records);
		}
	}
	inline void CustomExecuteSql(const char* sql)
	{
		if (m_Statement == nullptr)
		{
			m_Statement.reset(m_DBConnection->createStatement());
		}
		m_Statement->executeUpdate(sql);
	}
	
	virtual void CreateTradingDay() override;
	virtual void DropTradingDay() override;
	virtual void InsertTradingDay(mdb::TradingDay* record) override;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) override;
	virtual void DeleteTradingDay(mdb::TradingDay* record) override;
	virtual void UpdateTradingDay(mdb::TradingDay* record) override;
	virtual void SelectTradingDay(std::list<mdb::TradingDay*>& records) override;
	virtual void TruncateTradingDay() override;
	
	virtual void CreateExchange() override;
	virtual void DropExchange() override;
	virtual void InsertExchange(mdb::Exchange* record) override;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) override;
	virtual void DeleteExchange(mdb::Exchange* record) override;
	virtual void UpdateExchange(mdb::Exchange* record) override;
	virtual void SelectExchange(std::list<mdb::Exchange*>& records) override;
	virtual void TruncateExchange() override;
	
	virtual void CreateProduct() override;
	virtual void DropProduct() override;
	virtual void InsertProduct(mdb::Product* record) override;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) override;
	virtual void DeleteProduct(mdb::Product* record) override;
	virtual void UpdateProduct(mdb::Product* record) override;
	virtual void SelectProduct(std::list<mdb::Product*>& records) override;
	virtual void TruncateProduct() override;
	
	virtual void CreateInstrument() override;
	virtual void DropInstrument() override;
	virtual void InsertInstrument(mdb::Instrument* record) override;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) override;
	virtual void DeleteInstrument(mdb::Instrument* record) override;
	virtual void UpdateInstrument(mdb::Instrument* record) override;
	virtual void SelectInstrument(std::list<mdb::Instrument*>& records) override;
	virtual void TruncateInstrument() override;
	
	virtual void CreatePrimaryAccount() override;
	virtual void DropPrimaryAccount() override;
	virtual void InsertPrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void BatchInsertPrimaryAccount(std::list<mdb::PrimaryAccount*>* records) override;
	virtual void DeletePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void DeletePrimaryAccountByOfferIDIndex(mdb::PrimaryAccount* record) override;
	virtual void UpdatePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void SelectPrimaryAccount(std::list<mdb::PrimaryAccount*>& records) override;
	virtual void TruncatePrimaryAccount() override;
	
	virtual void CreateAccount() override;
	virtual void DropAccount() override;
	virtual void InsertAccount(mdb::Account* record) override;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) override;
	virtual void DeleteAccount(mdb::Account* record) override;
	virtual void UpdateAccount(mdb::Account* record) override;
	virtual void SelectAccount(std::list<mdb::Account*>& records) override;
	virtual void TruncateAccount() override;
	
	virtual void CreateCapital() override;
	virtual void DropCapital() override;
	virtual void InsertCapital(mdb::Capital* record) override;
	virtual void BatchInsertCapital(std::list<mdb::Capital*>* records) override;
	virtual void DeleteCapital(mdb::Capital* record) override;
	virtual void DeleteCapitalByTradingDayIndex(mdb::Capital* record) override;
	virtual void UpdateCapital(mdb::Capital* record) override;
	virtual void SelectCapital(std::list<mdb::Capital*>& records) override;
	virtual void TruncateCapital() override;
	
	virtual void CreatePosition() override;
	virtual void DropPosition() override;
	virtual void InsertPosition(mdb::Position* record) override;
	virtual void BatchInsertPosition(std::list<mdb::Position*>* records) override;
	virtual void DeletePosition(mdb::Position* record) override;
	virtual void DeletePositionByAccountIndex(mdb::Position* record) override;
	virtual void DeletePositionByTradingDayIndex(mdb::Position* record) override;
	virtual void UpdatePosition(mdb::Position* record) override;
	virtual void SelectPosition(std::list<mdb::Position*>& records) override;
	virtual void TruncatePosition() override;
	
	virtual void CreatePositionDetail() override;
	virtual void DropPositionDetail() override;
	virtual void InsertPositionDetail(mdb::PositionDetail* record) override;
	virtual void BatchInsertPositionDetail(std::list<mdb::PositionDetail*>* records) override;
	virtual void DeletePositionDetail(mdb::PositionDetail* record) override;
	virtual void DeletePositionDetailByTradeMatchIndex(mdb::PositionDetail* record) override;
	virtual void DeletePositionDetailByTradingDayIndex(mdb::PositionDetail* record) override;
	virtual void UpdatePositionDetail(mdb::PositionDetail* record) override;
	virtual void SelectPositionDetail(std::list<mdb::PositionDetail*>& records) override;
	virtual void TruncatePositionDetail() override;
	
	virtual void CreateOrder() override;
	virtual void DropOrder() override;
	virtual void InsertOrder(mdb::Order* record) override;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) override;
	virtual void DeleteOrder(mdb::Order* record) override;
	virtual void UpdateOrder(mdb::Order* record) override;
	virtual void SelectOrder(std::list<mdb::Order*>& records) override;
	virtual void TruncateOrder() override;
	
	virtual void CreateTrade() override;
	virtual void DropTrade() override;
	virtual void InsertTrade(mdb::Trade* record) override;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) override;
	virtual void DeleteTrade(mdb::Trade* record) override;
	virtual void UpdateTrade(mdb::Trade* record) override;
	virtual void SelectTrade(std::list<mdb::Trade*>& records) override;
	virtual void TruncateTrade() override;
	

private:
	void SetStatementForTradingDayRecord(mdb::TradingDay* record);
	void SetStatementForTradingDayRecordUpdate(mdb::TradingDay* record);
	void SetStatementForTradingDayPrimaryKey(const IntType& PK);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::TradingDay*>& records);
	void SetStatementForExchangeRecord(mdb::Exchange* record);
	void SetStatementForExchangeRecordUpdate(mdb::Exchange* record);
	void SetStatementForExchangePrimaryKey(const ExchangeIDType& ExchangeID);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Exchange*>& records);
	void SetStatementForProductRecord(mdb::Product* record);
	void SetStatementForProductRecordUpdate(mdb::Product* record);
	void SetStatementForProductPrimaryKey(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Product*>& records);
	void SetStatementForInstrumentRecord(mdb::Instrument* record);
	void SetStatementForInstrumentRecordUpdate(mdb::Instrument* record);
	void SetStatementForInstrumentPrimaryKey(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Instrument*>& records);
	void SetStatementForPrimaryAccountRecord(mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountRecordUpdate(mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountPrimaryKey(const AccountIDType& PrimaryAccountID);
	void SetStatementForPrimaryAccountIndexOfferID(mdb::PrimaryAccount* record);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::PrimaryAccount*>& records);
	void SetStatementForAccountRecord(mdb::Account* record);
	void SetStatementForAccountRecordUpdate(mdb::Account* record);
	void SetStatementForAccountPrimaryKey(const AccountIDType& AccountID);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Account*>& records);
	void SetStatementForCapitalRecord(mdb::Capital* record);
	void SetStatementForCapitalRecordUpdate(mdb::Capital* record);
	void SetStatementForCapitalPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID);
	void SetStatementForCapitalIndexTradingDay(mdb::Capital* record);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Capital*>& records);
	void SetStatementForPositionRecord(mdb::Position* record);
	void SetStatementForPositionRecordUpdate(mdb::Position* record);
	void SetStatementForPositionPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
	void SetStatementForPositionIndexAccount(mdb::Position* record);
	void SetStatementForPositionIndexTradingDay(mdb::Position* record);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Position*>& records);
	void SetStatementForPositionDetailRecord(mdb::PositionDetail* record);
	void SetStatementForPositionDetailRecordUpdate(mdb::PositionDetail* record);
	void SetStatementForPositionDetailPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID);
	void SetStatementForPositionDetailIndexTradeMatch(mdb::PositionDetail* record);
	void SetStatementForPositionDetailIndexTradingDay(mdb::PositionDetail* record);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::PositionDetail*>& records);
	void SetStatementForOrderRecord(mdb::Order* record);
	void SetStatementForOrderRecordUpdate(mdb::Order* record);
	void SetStatementForOrderPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Order*>& records);
	void SetStatementForTradeRecord(mdb::Trade* record);
	void SetStatementForTradeRecordUpdate(mdb::Trade* record);
	void SetStatementForTradePrimaryKey(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction);
	void ParseRecord(sql::ResultSet* result, std::list<mdb::Trade*>& records);


private:
	sql::Driver* m_Driver;
	std::unique_ptr<sql::Connection, SqlConnectionDeleter> m_DBConnection;
	std::unique_ptr<sql::Statement, SqlStatementDeleter> m_Statement;

	std::string m_Host;
	std::string m_User;
	std::string m_Passwd;
	char* m_SqlBuff;

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
