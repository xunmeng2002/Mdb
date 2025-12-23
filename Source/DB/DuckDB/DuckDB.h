#pragma once
#include "DB.h"
#include "duckdb.hpp"
#include <string>
#include <list>


class DuckDB : public DB
{
public:
	DuckDB(const std::string& dbName);
	~DuckDB();
	virtual bool Connect() override;
	virtual void DisConnect() override;
	virtual void InitDB() override;
	virtual void CreateTables() override;
	virtual void DropTables() override;
	virtual void TruncateTables() override;
	virtual void TruncateSessionTables() override;
	
	bool Exec(const char* sql) const;
	virtual void CreateTradingDay() override;
	virtual void DropTradingDay() override;
	virtual void InsertTradingDay(mdb::TradingDay* record) override;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) override;
	virtual void DeleteTradingDay(mdb::TradingDay* record) override;
	virtual void UpdateTradingDay(mdb::TradingDay* record) override;
	virtual void SelectTradingDay(std::list<mdb::TradingDay*>& records) override;
	virtual void TruncateTradingDay() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::TradingDay*>& records);
	
	virtual void CreateExchange() override;
	virtual void DropExchange() override;
	virtual void InsertExchange(mdb::Exchange* record) override;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) override;
	virtual void DeleteExchange(mdb::Exchange* record) override;
	virtual void UpdateExchange(mdb::Exchange* record) override;
	virtual void SelectExchange(std::list<mdb::Exchange*>& records) override;
	virtual void TruncateExchange() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Exchange*>& records);
	
	virtual void CreateProduct() override;
	virtual void DropProduct() override;
	virtual void InsertProduct(mdb::Product* record) override;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) override;
	virtual void DeleteProduct(mdb::Product* record) override;
	virtual void UpdateProduct(mdb::Product* record) override;
	virtual void SelectProduct(std::list<mdb::Product*>& records) override;
	virtual void TruncateProduct() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Product*>& records);
	
	virtual void CreateInstrument() override;
	virtual void DropInstrument() override;
	virtual void InsertInstrument(mdb::Instrument* record) override;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) override;
	virtual void DeleteInstrument(mdb::Instrument* record) override;
	virtual void UpdateInstrument(mdb::Instrument* record) override;
	virtual void SelectInstrument(std::list<mdb::Instrument*>& records) override;
	virtual void TruncateInstrument() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Instrument*>& records);
	
	virtual void CreatePrimaryAccount() override;
	virtual void DropPrimaryAccount() override;
	virtual void InsertPrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void BatchInsertPrimaryAccount(std::list<mdb::PrimaryAccount*>* records) override;
	virtual void DeletePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void DeletePrimaryAccountByOfferIDIndex(mdb::PrimaryAccount* record) override;
	virtual void UpdatePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void SelectPrimaryAccount(std::list<mdb::PrimaryAccount*>& records) override;
	virtual void TruncatePrimaryAccount() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::PrimaryAccount*>& records);
	
	virtual void CreateAccount() override;
	virtual void DropAccount() override;
	virtual void InsertAccount(mdb::Account* record) override;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) override;
	virtual void DeleteAccount(mdb::Account* record) override;
	virtual void UpdateAccount(mdb::Account* record) override;
	virtual void SelectAccount(std::list<mdb::Account*>& records) override;
	virtual void TruncateAccount() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Account*>& records);
	
	virtual void CreateCapital() override;
	virtual void DropCapital() override;
	virtual void InsertCapital(mdb::Capital* record) override;
	virtual void BatchInsertCapital(std::list<mdb::Capital*>* records) override;
	virtual void DeleteCapital(mdb::Capital* record) override;
	virtual void DeleteCapitalByTradingDayIndex(mdb::Capital* record) override;
	virtual void UpdateCapital(mdb::Capital* record) override;
	virtual void SelectCapital(std::list<mdb::Capital*>& records) override;
	virtual void TruncateCapital() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Capital*>& records);
	
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
	static void ParseRecord(duckdb_result& result, std::list<mdb::Position*>& records);
	
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
	static void ParseRecord(duckdb_result& result, std::list<mdb::PositionDetail*>& records);
	
	virtual void CreateOrder() override;
	virtual void DropOrder() override;
	virtual void InsertOrder(mdb::Order* record) override;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) override;
	virtual void DeleteOrder(mdb::Order* record) override;
	virtual void UpdateOrder(mdb::Order* record) override;
	virtual void SelectOrder(std::list<mdb::Order*>& records) override;
	virtual void TruncateOrder() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Order*>& records);
	
	virtual void CreateTrade() override;
	virtual void DropTrade() override;
	virtual void InsertTrade(mdb::Trade* record) override;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) override;
	virtual void DeleteTrade(mdb::Trade* record) override;
	virtual void UpdateTrade(mdb::Trade* record) override;
	virtual void SelectTrade(std::list<mdb::Trade*>& records) override;
	virtual void TruncateTrade() override;
	static void ParseRecord(duckdb_result& result, std::list<mdb::Trade*>& records);
	

private:
	bool AppendForTradingDayRecord(duckdb_appender appender, mdb::TradingDay* record);
	void SetStatementForTradingDayRecord(duckdb_prepared_statement statement, mdb::TradingDay* record);
	void SetStatementForTradingDayRecordUpdate(duckdb_prepared_statement statement, mdb::TradingDay* record);
	void SetStatementForTradingDayPrimaryKey(duckdb_prepared_statement statement, mdb::TradingDay* record);
	bool AppendForExchangeRecord(duckdb_appender appender, mdb::Exchange* record);
	void SetStatementForExchangeRecord(duckdb_prepared_statement statement, mdb::Exchange* record);
	void SetStatementForExchangeRecordUpdate(duckdb_prepared_statement statement, mdb::Exchange* record);
	void SetStatementForExchangePrimaryKey(duckdb_prepared_statement statement, mdb::Exchange* record);
	bool AppendForProductRecord(duckdb_appender appender, mdb::Product* record);
	void SetStatementForProductRecord(duckdb_prepared_statement statement, mdb::Product* record);
	void SetStatementForProductRecordUpdate(duckdb_prepared_statement statement, mdb::Product* record);
	void SetStatementForProductPrimaryKey(duckdb_prepared_statement statement, mdb::Product* record);
	bool AppendForInstrumentRecord(duckdb_appender appender, mdb::Instrument* record);
	void SetStatementForInstrumentRecord(duckdb_prepared_statement statement, mdb::Instrument* record);
	void SetStatementForInstrumentRecordUpdate(duckdb_prepared_statement statement, mdb::Instrument* record);
	void SetStatementForInstrumentPrimaryKey(duckdb_prepared_statement statement, mdb::Instrument* record);
	bool AppendForPrimaryAccountRecord(duckdb_appender appender, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountRecord(duckdb_prepared_statement statement, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountRecordUpdate(duckdb_prepared_statement statement, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountPrimaryKey(duckdb_prepared_statement statement, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountIndexOfferID(duckdb_prepared_statement statement, mdb::PrimaryAccount* record);
	bool AppendForAccountRecord(duckdb_appender appender, mdb::Account* record);
	void SetStatementForAccountRecord(duckdb_prepared_statement statement, mdb::Account* record);
	void SetStatementForAccountRecordUpdate(duckdb_prepared_statement statement, mdb::Account* record);
	void SetStatementForAccountPrimaryKey(duckdb_prepared_statement statement, mdb::Account* record);
	bool AppendForCapitalRecord(duckdb_appender appender, mdb::Capital* record);
	void SetStatementForCapitalRecord(duckdb_prepared_statement statement, mdb::Capital* record);
	void SetStatementForCapitalRecordUpdate(duckdb_prepared_statement statement, mdb::Capital* record);
	void SetStatementForCapitalPrimaryKey(duckdb_prepared_statement statement, mdb::Capital* record);
	void SetStatementForCapitalIndexTradingDay(duckdb_prepared_statement statement, mdb::Capital* record);
	bool AppendForPositionRecord(duckdb_appender appender, mdb::Position* record);
	void SetStatementForPositionRecord(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionRecordUpdate(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionPrimaryKey(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionIndexAccount(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionIndexTradingDay(duckdb_prepared_statement statement, mdb::Position* record);
	bool AppendForPositionDetailRecord(duckdb_appender appender, mdb::PositionDetail* record);
	void SetStatementForPositionDetailRecord(duckdb_prepared_statement statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailRecordUpdate(duckdb_prepared_statement statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailPrimaryKey(duckdb_prepared_statement statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailIndexTradeMatch(duckdb_prepared_statement statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailIndexTradingDay(duckdb_prepared_statement statement, mdb::PositionDetail* record);
	bool AppendForOrderRecord(duckdb_appender appender, mdb::Order* record);
	void SetStatementForOrderRecord(duckdb_prepared_statement statement, mdb::Order* record);
	void SetStatementForOrderRecordUpdate(duckdb_prepared_statement statement, mdb::Order* record);
	void SetStatementForOrderPrimaryKey(duckdb_prepared_statement statement, mdb::Order* record);
	bool AppendForTradeRecord(duckdb_appender appender, mdb::Trade* record);
	void SetStatementForTradeRecord(duckdb_prepared_statement statement, mdb::Trade* record);
	void SetStatementForTradeRecordUpdate(duckdb_prepared_statement statement, mdb::Trade* record);
	void SetStatementForTradePrimaryKey(duckdb_prepared_statement statement, mdb::Trade* record);


private:
	std::string m_DBName;
	duckdb_database m_DB;
	duckdb_connection m_Connection;
	char* m_SqlBuff;

	duckdb_prepared_statement m_TradingDayDeleteStatement;
	duckdb_prepared_statement m_TradingDayUpdateStatement;
	duckdb_prepared_statement m_TradingDaySelectStatement;
	duckdb_prepared_statement m_TradingDayTruncateStatement;
	duckdb_prepared_statement m_ExchangeDeleteStatement;
	duckdb_prepared_statement m_ExchangeUpdateStatement;
	duckdb_prepared_statement m_ExchangeSelectStatement;
	duckdb_prepared_statement m_ExchangeTruncateStatement;
	duckdb_prepared_statement m_ProductDeleteStatement;
	duckdb_prepared_statement m_ProductUpdateStatement;
	duckdb_prepared_statement m_ProductSelectStatement;
	duckdb_prepared_statement m_ProductTruncateStatement;
	duckdb_prepared_statement m_InstrumentDeleteStatement;
	duckdb_prepared_statement m_InstrumentUpdateStatement;
	duckdb_prepared_statement m_InstrumentSelectStatement;
	duckdb_prepared_statement m_InstrumentTruncateStatement;
	duckdb_prepared_statement m_PrimaryAccountDeleteStatement;
	duckdb_prepared_statement m_PrimaryAccountDeleteByOfferIDIndexStatement;
	duckdb_prepared_statement m_PrimaryAccountUpdateStatement;
	duckdb_prepared_statement m_PrimaryAccountSelectStatement;
	duckdb_prepared_statement m_PrimaryAccountTruncateStatement;
	duckdb_prepared_statement m_AccountDeleteStatement;
	duckdb_prepared_statement m_AccountUpdateStatement;
	duckdb_prepared_statement m_AccountSelectStatement;
	duckdb_prepared_statement m_AccountTruncateStatement;
	duckdb_prepared_statement m_CapitalDeleteStatement;
	duckdb_prepared_statement m_CapitalDeleteByTradingDayIndexStatement;
	duckdb_prepared_statement m_CapitalUpdateStatement;
	duckdb_prepared_statement m_CapitalSelectStatement;
	duckdb_prepared_statement m_CapitalTruncateStatement;
	duckdb_prepared_statement m_PositionDeleteStatement;
	duckdb_prepared_statement m_PositionDeleteByAccountIndexStatement;
	duckdb_prepared_statement m_PositionDeleteByTradingDayIndexStatement;
	duckdb_prepared_statement m_PositionUpdateStatement;
	duckdb_prepared_statement m_PositionSelectStatement;
	duckdb_prepared_statement m_PositionTruncateStatement;
	duckdb_prepared_statement m_PositionDetailDeleteStatement;
	duckdb_prepared_statement m_PositionDetailDeleteByTradeMatchIndexStatement;
	duckdb_prepared_statement m_PositionDetailDeleteByTradingDayIndexStatement;
	duckdb_prepared_statement m_PositionDetailUpdateStatement;
	duckdb_prepared_statement m_PositionDetailSelectStatement;
	duckdb_prepared_statement m_PositionDetailTruncateStatement;
	duckdb_prepared_statement m_OrderDeleteStatement;
	duckdb_prepared_statement m_OrderUpdateStatement;
	duckdb_prepared_statement m_OrderSelectStatement;
	duckdb_prepared_statement m_OrderTruncateStatement;
	duckdb_prepared_statement m_TradeDeleteStatement;
	duckdb_prepared_statement m_TradeUpdateStatement;
	duckdb_prepared_statement m_TradeSelectStatement;
	duckdb_prepared_statement m_TradeTruncateStatement;
};
