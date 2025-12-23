#pragma once
#include "DB.h"
#include "sqlite3.h"
#include <string>
#include <list>


class SqliteDB : public DB
{
public:
	SqliteDB(const std::string& dbName);
	~SqliteDB();
	virtual bool Connect() override;
	virtual void DisConnect() override;
	virtual void InitDB() override;
	virtual void CreateTables() override;
	virtual void DropTables() override;
	virtual void TruncateTables() override;
	virtual void TruncateSessionTables() override;
	
	bool Exec(const char* sql);
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
	void SetStatementForTradingDayRecord(sqlite3_stmt* statement, mdb::TradingDay* record);
	void SetStatementForTradingDayRecordUpdate(sqlite3_stmt* statement, mdb::TradingDay* record);
	void SetStatementForTradingDayPrimaryKey(sqlite3_stmt* statement, const IntType& PK);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::TradingDay*>& records);
	void SetStatementForExchangeRecord(sqlite3_stmt* statement, mdb::Exchange* record);
	void SetStatementForExchangeRecordUpdate(sqlite3_stmt* statement, mdb::Exchange* record);
	void SetStatementForExchangePrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Exchange*>& records);
	void SetStatementForProductRecord(sqlite3_stmt* statement, mdb::Product* record);
	void SetStatementForProductRecordUpdate(sqlite3_stmt* statement, mdb::Product* record);
	void SetStatementForProductPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Product*>& records);
	void SetStatementForInstrumentRecord(sqlite3_stmt* statement, mdb::Instrument* record);
	void SetStatementForInstrumentRecordUpdate(sqlite3_stmt* statement, mdb::Instrument* record);
	void SetStatementForInstrumentPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Instrument*>& records);
	void SetStatementForPrimaryAccountRecord(sqlite3_stmt* statement, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountRecordUpdate(sqlite3_stmt* statement, mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountPrimaryKey(sqlite3_stmt* statement, const AccountIDType& PrimaryAccountID);
	void SetStatementForPrimaryAccountIndexOfferID(sqlite3_stmt* statement, mdb::PrimaryAccount* record);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::PrimaryAccount*>& records);
	void SetStatementForAccountRecord(sqlite3_stmt* statement, mdb::Account* record);
	void SetStatementForAccountRecordUpdate(sqlite3_stmt* statement, mdb::Account* record);
	void SetStatementForAccountPrimaryKey(sqlite3_stmt* statement, const AccountIDType& AccountID);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Account*>& records);
	void SetStatementForCapitalRecord(sqlite3_stmt* statement, mdb::Capital* record);
	void SetStatementForCapitalRecordUpdate(sqlite3_stmt* statement, mdb::Capital* record);
	void SetStatementForCapitalPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID);
	void SetStatementForCapitalIndexTradingDay(sqlite3_stmt* statement, mdb::Capital* record);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Capital*>& records);
	void SetStatementForPositionRecord(sqlite3_stmt* statement, mdb::Position* record);
	void SetStatementForPositionRecordUpdate(sqlite3_stmt* statement, mdb::Position* record);
	void SetStatementForPositionPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
	void SetStatementForPositionIndexAccount(sqlite3_stmt* statement, mdb::Position* record);
	void SetStatementForPositionIndexTradingDay(sqlite3_stmt* statement, mdb::Position* record);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Position*>& records);
	void SetStatementForPositionDetailRecord(sqlite3_stmt* statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailRecordUpdate(sqlite3_stmt* statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID);
	void SetStatementForPositionDetailIndexTradeMatch(sqlite3_stmt* statement, mdb::PositionDetail* record);
	void SetStatementForPositionDetailIndexTradingDay(sqlite3_stmt* statement, mdb::PositionDetail* record);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::PositionDetail*>& records);
	void SetStatementForOrderRecord(sqlite3_stmt* statement, mdb::Order* record);
	void SetStatementForOrderRecordUpdate(sqlite3_stmt* statement, mdb::Order* record);
	void SetStatementForOrderPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Order*>& records);
	void SetStatementForTradeRecord(sqlite3_stmt* statement, mdb::Trade* record);
	void SetStatementForTradeRecordUpdate(sqlite3_stmt* statement, mdb::Trade* record);
	void SetStatementForTradePrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction);
	void ParseRecord(sqlite3_stmt* statement, std::list<mdb::Trade*>& records);


private:
	std::string m_DBName;
	sqlite3* m_DB;
	char* m_SqlBuff;

	sqlite3_stmt* m_TradingDayInsertStatement;
	sqlite3_stmt* m_TradingDayDeleteStatement;
	sqlite3_stmt* m_TradingDayUpdateStatement;
	sqlite3_stmt* m_TradingDaySelectStatement;
	sqlite3_stmt* m_TradingDayTruncateStatement;
	sqlite3_stmt* m_ExchangeInsertStatement;
	sqlite3_stmt* m_ExchangeDeleteStatement;
	sqlite3_stmt* m_ExchangeUpdateStatement;
	sqlite3_stmt* m_ExchangeSelectStatement;
	sqlite3_stmt* m_ExchangeTruncateStatement;
	sqlite3_stmt* m_ProductInsertStatement;
	sqlite3_stmt* m_ProductDeleteStatement;
	sqlite3_stmt* m_ProductUpdateStatement;
	sqlite3_stmt* m_ProductSelectStatement;
	sqlite3_stmt* m_ProductTruncateStatement;
	sqlite3_stmt* m_InstrumentInsertStatement;
	sqlite3_stmt* m_InstrumentDeleteStatement;
	sqlite3_stmt* m_InstrumentUpdateStatement;
	sqlite3_stmt* m_InstrumentSelectStatement;
	sqlite3_stmt* m_InstrumentTruncateStatement;
	sqlite3_stmt* m_PrimaryAccountInsertStatement;
	sqlite3_stmt* m_PrimaryAccountDeleteStatement;
	sqlite3_stmt* m_PrimaryAccountDeleteByOfferIDIndexStatement;
	sqlite3_stmt* m_PrimaryAccountUpdateStatement;
	sqlite3_stmt* m_PrimaryAccountSelectStatement;
	sqlite3_stmt* m_PrimaryAccountTruncateStatement;
	sqlite3_stmt* m_AccountInsertStatement;
	sqlite3_stmt* m_AccountDeleteStatement;
	sqlite3_stmt* m_AccountUpdateStatement;
	sqlite3_stmt* m_AccountSelectStatement;
	sqlite3_stmt* m_AccountTruncateStatement;
	sqlite3_stmt* m_CapitalInsertStatement;
	sqlite3_stmt* m_CapitalDeleteStatement;
	sqlite3_stmt* m_CapitalDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_CapitalUpdateStatement;
	sqlite3_stmt* m_CapitalSelectStatement;
	sqlite3_stmt* m_CapitalTruncateStatement;
	sqlite3_stmt* m_PositionInsertStatement;
	sqlite3_stmt* m_PositionDeleteStatement;
	sqlite3_stmt* m_PositionDeleteByAccountIndexStatement;
	sqlite3_stmt* m_PositionDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_PositionUpdateStatement;
	sqlite3_stmt* m_PositionSelectStatement;
	sqlite3_stmt* m_PositionTruncateStatement;
	sqlite3_stmt* m_PositionDetailInsertStatement;
	sqlite3_stmt* m_PositionDetailDeleteStatement;
	sqlite3_stmt* m_PositionDetailDeleteByTradeMatchIndexStatement;
	sqlite3_stmt* m_PositionDetailDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_PositionDetailUpdateStatement;
	sqlite3_stmt* m_PositionDetailSelectStatement;
	sqlite3_stmt* m_PositionDetailTruncateStatement;
	sqlite3_stmt* m_OrderInsertStatement;
	sqlite3_stmt* m_OrderDeleteStatement;
	sqlite3_stmt* m_OrderUpdateStatement;
	sqlite3_stmt* m_OrderSelectStatement;
	sqlite3_stmt* m_OrderTruncateStatement;
	sqlite3_stmt* m_TradeInsertStatement;
	sqlite3_stmt* m_TradeDeleteStatement;
	sqlite3_stmt* m_TradeUpdateStatement;
	sqlite3_stmt* m_TradeSelectStatement;
	sqlite3_stmt* m_TradeTruncateStatement;
};
