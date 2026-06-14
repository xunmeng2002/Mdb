#pragma once
#include <Mdb/MariadbWrapper/MariadbWrapperExport.h>
#include <Mdb/Mdb/DB.h>
#include <string>
#include <memory>
#include <list>


class MARIADBWRAPPER_EXPORTS MariadbWrapper : public DB
{
public:
	MariadbWrapper(const std::string& host, const std::string& user, const std::string& passwd);
	~MariadbWrapper();
	virtual bool Connect() override;
	virtual void DisConnect() override;
	virtual void InitDB() override;
	virtual void CreateTables() override;
	virtual void DropTables() override;
	virtual void TruncateTables() override;
	virtual void TruncateSessionTables() override;
	virtual void Exec(const char* sql) override;
	
	virtual void CreateTradingDay() override;
	virtual void DropTradingDay() override;
	virtual void InsertTradingDay(mdb::TradingDay* record) override;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) override;
	virtual void DeleteTradingDay(mdb::TradingDay* record) override;
	virtual void UpdateTradingDay(mdb::TradingDay* record) override;
	virtual void SelectTradingDay(std::list<mdb::TradingDay*>& records) override;
	virtual void SelectTradingDayWithSql(const char* sql, std::list<mdb::TradingDay*>& records) override;
	virtual void TruncateTradingDay() override;
	
	virtual void CreateExchange() override;
	virtual void DropExchange() override;
	virtual void InsertExchange(mdb::Exchange* record) override;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) override;
	virtual void DeleteExchange(mdb::Exchange* record) override;
	virtual void UpdateExchange(mdb::Exchange* record) override;
	virtual void SelectExchange(std::list<mdb::Exchange*>& records) override;
	virtual void SelectExchangeWithSql(const char* sql, std::list<mdb::Exchange*>& records) override;
	virtual void TruncateExchange() override;
	
	virtual void CreateProduct() override;
	virtual void DropProduct() override;
	virtual void InsertProduct(mdb::Product* record) override;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) override;
	virtual void DeleteProduct(mdb::Product* record) override;
	virtual void UpdateProduct(mdb::Product* record) override;
	virtual void SelectProduct(std::list<mdb::Product*>& records) override;
	virtual void SelectProductWithSql(const char* sql, std::list<mdb::Product*>& records) override;
	virtual void TruncateProduct() override;
	
	virtual void CreateInstrument() override;
	virtual void DropInstrument() override;
	virtual void InsertInstrument(mdb::Instrument* record) override;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) override;
	virtual void DeleteInstrument(mdb::Instrument* record) override;
	virtual void UpdateInstrument(mdb::Instrument* record) override;
	virtual void SelectInstrument(std::list<mdb::Instrument*>& records) override;
	virtual void SelectInstrumentWithSql(const char* sql, std::list<mdb::Instrument*>& records) override;
	virtual void TruncateInstrument() override;
	
	virtual void CreatePrimaryAccount() override;
	virtual void DropPrimaryAccount() override;
	virtual void InsertPrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void BatchInsertPrimaryAccount(std::list<mdb::PrimaryAccount*>* records) override;
	virtual void DeletePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void DeletePrimaryAccountByOfferIDIndex(mdb::PrimaryAccount* record) override;
	virtual void UpdatePrimaryAccount(mdb::PrimaryAccount* record) override;
	virtual void SelectPrimaryAccount(std::list<mdb::PrimaryAccount*>& records) override;
	virtual void SelectPrimaryAccountWithSql(const char* sql, std::list<mdb::PrimaryAccount*>& records) override;
	virtual void TruncatePrimaryAccount() override;
	
	virtual void CreateAccount() override;
	virtual void DropAccount() override;
	virtual void InsertAccount(mdb::Account* record) override;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) override;
	virtual void DeleteAccount(mdb::Account* record) override;
	virtual void UpdateAccount(mdb::Account* record) override;
	virtual void SelectAccount(std::list<mdb::Account*>& records) override;
	virtual void SelectAccountWithSql(const char* sql, std::list<mdb::Account*>& records) override;
	virtual void TruncateAccount() override;
	
	virtual void CreateCapital() override;
	virtual void DropCapital() override;
	virtual void InsertCapital(mdb::Capital* record) override;
	virtual void BatchInsertCapital(std::list<mdb::Capital*>* records) override;
	virtual void DeleteCapital(mdb::Capital* record) override;
	virtual void DeleteCapitalByTradingDayIndex(mdb::Capital* record) override;
	virtual void UpdateCapital(mdb::Capital* record) override;
	virtual void SelectCapital(std::list<mdb::Capital*>& records) override;
	virtual void SelectCapitalWithSql(const char* sql, std::list<mdb::Capital*>& records) override;
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
	virtual void SelectPositionWithSql(const char* sql, std::list<mdb::Position*>& records) override;
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
	virtual void SelectPositionDetailWithSql(const char* sql, std::list<mdb::PositionDetail*>& records) override;
	virtual void TruncatePositionDetail() override;
	
	virtual void CreateOrder() override;
	virtual void DropOrder() override;
	virtual void InsertOrder(mdb::Order* record) override;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) override;
	virtual void DeleteOrder(mdb::Order* record) override;
	virtual void UpdateOrder(mdb::Order* record) override;
	virtual void SelectOrder(std::list<mdb::Order*>& records) override;
	virtual void SelectOrderWithSql(const char* sql, std::list<mdb::Order*>& records) override;
	virtual void TruncateOrder() override;
	
	virtual void CreateTrade() override;
	virtual void DropTrade() override;
	virtual void InsertTrade(mdb::Trade* record) override;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) override;
	virtual void DeleteTrade(mdb::Trade* record) override;
	virtual void UpdateTrade(mdb::Trade* record) override;
	virtual void SelectTrade(std::list<mdb::Trade*>& records) override;
	virtual void SelectTradeWithSql(const char* sql, std::list<mdb::Trade*>& records) override;
	virtual void TruncateTrade() override;
	

private:
	void SetStatementForTradingDayRecordInsert(mdb::TradingDay* record);
	void SetStatementForTradingDayRecordUpdate(mdb::TradingDay* record);
	void SetStatementForTradingDayPrimaryKey(const IntType& PK);
	void SetStatementForExchangeRecordInsert(mdb::Exchange* record);
	void SetStatementForExchangeRecordUpdate(mdb::Exchange* record);
	void SetStatementForExchangePrimaryKey(const ExchangeIDType& ExchangeID);
	void SetStatementForProductRecordInsert(mdb::Product* record);
	void SetStatementForProductRecordUpdate(mdb::Product* record);
	void SetStatementForProductPrimaryKey(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
	void SetStatementForInstrumentRecordInsert(mdb::Instrument* record);
	void SetStatementForInstrumentRecordUpdate(mdb::Instrument* record);
	void SetStatementForInstrumentPrimaryKey(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID);
	void SetStatementForPrimaryAccountRecordInsert(mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountRecordUpdate(mdb::PrimaryAccount* record);
	void SetStatementForPrimaryAccountPrimaryKey(const AccountIDType& PrimaryAccountID);
	void SetStatementForPrimaryAccountIndexOfferID(mdb::PrimaryAccount* record);
	void SetStatementForAccountRecordInsert(mdb::Account* record);
	void SetStatementForAccountRecordUpdate(mdb::Account* record);
	void SetStatementForAccountPrimaryKey(const AccountIDType& AccountID);
	void SetStatementForCapitalRecordInsert(mdb::Capital* record);
	void SetStatementForCapitalRecordUpdate(mdb::Capital* record);
	void SetStatementForCapitalPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID);
	void SetStatementForCapitalIndexTradingDay(mdb::Capital* record);
	void SetStatementForPositionRecordInsert(mdb::Position* record);
	void SetStatementForPositionRecordUpdate(mdb::Position* record);
	void SetStatementForPositionPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
	void SetStatementForPositionIndexAccount(mdb::Position* record);
	void SetStatementForPositionIndexTradingDay(mdb::Position* record);
	void SetStatementForPositionDetailRecordInsert(mdb::PositionDetail* record);
	void SetStatementForPositionDetailRecordUpdate(mdb::PositionDetail* record);
	void SetStatementForPositionDetailPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID);
	void SetStatementForPositionDetailIndexTradeMatch(mdb::PositionDetail* record);
	void SetStatementForPositionDetailIndexTradingDay(mdb::PositionDetail* record);
	void SetStatementForOrderRecordInsert(mdb::Order* record);
	void SetStatementForOrderRecordUpdate(mdb::Order* record);
	void SetStatementForOrderPrimaryKey(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID);
	void SetStatementForTradeRecordInsert(mdb::Trade* record);
	void SetStatementForTradeRecordUpdate(mdb::Trade* record);
	void SetStatementForTradePrimaryKey(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction);


private:
	struct Impl;
	std::unique_ptr<Impl> m_Impl;
	std::string m_Host;
	std::string m_User;
	std::string m_Passwd;
	char* m_SqlBuff;
};
