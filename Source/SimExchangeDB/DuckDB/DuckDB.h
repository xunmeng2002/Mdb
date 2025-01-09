#pragma once
#include "DB.h"
#include "duckdb.hpp"
#include <string>
#include <vector>
#include <list>


class DuckDB : public DB
{
public:
	DuckDB(const std::string& dbName);
	~DuckDB();
	virtual bool Connect() override;
	virtual void DisConnect() override;
	virtual void InitDB() override;
	virtual void TruncateSessionTables() override;
	virtual void TruncateTables() override;
	
	bool Exec(const char* sql) const;
	virtual void InsertTradingDay(mdb::TradingDay* record) override;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) override;
	virtual void DeleteTradingDay(mdb::TradingDay* record) override;
	virtual void UpdateTradingDay(mdb::TradingDay* record) override;
	virtual void SelectTradingDay(std::vector<mdb::TradingDay*>& records) override;
	virtual void TruncateTradingDay() override;
	
	virtual void InsertExchange(mdb::Exchange* record) override;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) override;
	virtual void DeleteExchange(mdb::Exchange* record) override;
	virtual void UpdateExchange(mdb::Exchange* record) override;
	virtual void SelectExchange(std::vector<mdb::Exchange*>& records) override;
	virtual void TruncateExchange() override;
	
	virtual void InsertProduct(mdb::Product* record) override;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) override;
	virtual void DeleteProduct(mdb::Product* record) override;
	virtual void UpdateProduct(mdb::Product* record) override;
	virtual void SelectProduct(std::vector<mdb::Product*>& records) override;
	virtual void TruncateProduct() override;
	
	virtual void InsertInstrument(mdb::Instrument* record) override;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) override;
	virtual void DeleteInstrument(mdb::Instrument* record) override;
	virtual void UpdateInstrument(mdb::Instrument* record) override;
	virtual void SelectInstrument(std::vector<mdb::Instrument*>& records) override;
	virtual void TruncateInstrument() override;
	
	virtual void InsertAccount(mdb::Account* record) override;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) override;
	virtual void DeleteAccount(mdb::Account* record) override;
	virtual void UpdateAccount(mdb::Account* record) override;
	virtual void SelectAccount(std::vector<mdb::Account*>& records) override;
	virtual void TruncateAccount() override;
	
	virtual void InsertPosition(mdb::Position* record) override;
	virtual void BatchInsertPosition(std::list<mdb::Position*>* records) override;
	virtual void DeletePosition(mdb::Position* record) override;
	virtual void DeletePositionByAccountIndex(mdb::Position* record) override;
	virtual void UpdatePosition(mdb::Position* record) override;
	virtual void SelectPosition(std::vector<mdb::Position*>& records) override;
	virtual void TruncatePosition() override;
	
	virtual void InsertOrder(mdb::Order* record) override;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) override;
	virtual void DeleteOrder(mdb::Order* record) override;
	virtual void UpdateOrder(mdb::Order* record) override;
	virtual void SelectOrder(std::vector<mdb::Order*>& records) override;
	virtual void TruncateOrder() override;
	
	virtual void InsertTrade(mdb::Trade* record) override;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) override;
	virtual void DeleteTrade(mdb::Trade* record) override;
	virtual void UpdateTrade(mdb::Trade* record) override;
	virtual void SelectTrade(std::vector<mdb::Trade*>& records) override;
	virtual void TruncateTrade() override;
	
	virtual void InsertMdTick(mdb::MdTick* record) override;
	virtual void BatchInsertMdTick(std::list<mdb::MdTick*>* records) override;
	virtual void DeleteMdTick(mdb::MdTick* record) override;
	virtual void UpdateMdTick(mdb::MdTick* record) override;
	virtual void SelectMdTick(std::vector<mdb::MdTick*>& records) override;
	virtual void TruncateMdTick() override;
	

private:
	bool AppendForTradingDayRecord(duckdb_appender appender, mdb::TradingDay* record);
	void SetStatementForTradingDayRecord(duckdb_prepared_statement statement, mdb::TradingDay* record);
	void SetStatementForTradingDayRecordUpdate(duckdb_prepared_statement statement, mdb::TradingDay* record);
	void SetStatementForTradingDayPrimaryKey(duckdb_prepared_statement statement, mdb::TradingDay* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::TradingDay*>& records);
	bool AppendForExchangeRecord(duckdb_appender appender, mdb::Exchange* record);
	void SetStatementForExchangeRecord(duckdb_prepared_statement statement, mdb::Exchange* record);
	void SetStatementForExchangeRecordUpdate(duckdb_prepared_statement statement, mdb::Exchange* record);
	void SetStatementForExchangePrimaryKey(duckdb_prepared_statement statement, mdb::Exchange* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Exchange*>& records);
	bool AppendForProductRecord(duckdb_appender appender, mdb::Product* record);
	void SetStatementForProductRecord(duckdb_prepared_statement statement, mdb::Product* record);
	void SetStatementForProductRecordUpdate(duckdb_prepared_statement statement, mdb::Product* record);
	void SetStatementForProductPrimaryKey(duckdb_prepared_statement statement, mdb::Product* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Product*>& records);
	bool AppendForInstrumentRecord(duckdb_appender appender, mdb::Instrument* record);
	void SetStatementForInstrumentRecord(duckdb_prepared_statement statement, mdb::Instrument* record);
	void SetStatementForInstrumentRecordUpdate(duckdb_prepared_statement statement, mdb::Instrument* record);
	void SetStatementForInstrumentPrimaryKey(duckdb_prepared_statement statement, mdb::Instrument* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Instrument*>& records);
	bool AppendForAccountRecord(duckdb_appender appender, mdb::Account* record);
	void SetStatementForAccountRecord(duckdb_prepared_statement statement, mdb::Account* record);
	void SetStatementForAccountRecordUpdate(duckdb_prepared_statement statement, mdb::Account* record);
	void SetStatementForAccountPrimaryKey(duckdb_prepared_statement statement, mdb::Account* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Account*>& records);
	bool AppendForPositionRecord(duckdb_appender appender, mdb::Position* record);
	void SetStatementForPositionRecord(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionRecordUpdate(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionPrimaryKey(duckdb_prepared_statement statement, mdb::Position* record);
	void SetStatementForPositionIndexAccount(duckdb_prepared_statement statement, mdb::Position* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Position*>& records);
	bool AppendForOrderRecord(duckdb_appender appender, mdb::Order* record);
	void SetStatementForOrderRecord(duckdb_prepared_statement statement, mdb::Order* record);
	void SetStatementForOrderRecordUpdate(duckdb_prepared_statement statement, mdb::Order* record);
	void SetStatementForOrderPrimaryKey(duckdb_prepared_statement statement, mdb::Order* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Order*>& records);
	bool AppendForTradeRecord(duckdb_appender appender, mdb::Trade* record);
	void SetStatementForTradeRecord(duckdb_prepared_statement statement, mdb::Trade* record);
	void SetStatementForTradeRecordUpdate(duckdb_prepared_statement statement, mdb::Trade* record);
	void SetStatementForTradePrimaryKey(duckdb_prepared_statement statement, mdb::Trade* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::Trade*>& records);
	bool AppendForMdTickRecord(duckdb_appender appender, mdb::MdTick* record);
	void SetStatementForMdTickRecord(duckdb_prepared_statement statement, mdb::MdTick* record);
	void SetStatementForMdTickRecordUpdate(duckdb_prepared_statement statement, mdb::MdTick* record);
	void SetStatementForMdTickPrimaryKey(duckdb_prepared_statement statement, mdb::MdTick* record);
	void ParseRecord(duckdb_result& result, std::vector<mdb::MdTick*>& records);


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
	duckdb_prepared_statement m_AccountDeleteStatement;
	duckdb_prepared_statement m_AccountUpdateStatement;
	duckdb_prepared_statement m_AccountSelectStatement;
	duckdb_prepared_statement m_AccountTruncateStatement;
	duckdb_prepared_statement m_PositionDeleteStatement;
	duckdb_prepared_statement m_PositionDeleteByAccountIndexStatement;
	duckdb_prepared_statement m_PositionUpdateStatement;
	duckdb_prepared_statement m_PositionSelectStatement;
	duckdb_prepared_statement m_PositionTruncateStatement;
	duckdb_prepared_statement m_OrderDeleteStatement;
	duckdb_prepared_statement m_OrderUpdateStatement;
	duckdb_prepared_statement m_OrderSelectStatement;
	duckdb_prepared_statement m_OrderTruncateStatement;
	duckdb_prepared_statement m_TradeDeleteStatement;
	duckdb_prepared_statement m_TradeUpdateStatement;
	duckdb_prepared_statement m_TradeSelectStatement;
	duckdb_prepared_statement m_TradeTruncateStatement;
	duckdb_prepared_statement m_MdTickDeleteStatement;
	duckdb_prepared_statement m_MdTickUpdateStatement;
	duckdb_prepared_statement m_MdTickSelectStatement;
	duckdb_prepared_statement m_MdTickTruncateStatement;
};
