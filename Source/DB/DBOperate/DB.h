#pragma once
#include "MdbStructs.h"
#include <string>
#include <list>


class DB
{
public:
	virtual bool Connect() = 0;
	virtual void DisConnect() = 0;
	virtual void InitDB() = 0;
	virtual void CreateTables() = 0;
	virtual void DropTables() = 0;
	virtual void TruncateTables() = 0;
	virtual void TruncateSessionTables() = 0;
	
	virtual void CreateTradingDay() = 0;
	virtual void DropTradingDay() = 0;
	virtual void InsertTradingDay(mdb::TradingDay* record) = 0;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) = 0;
	virtual void DeleteTradingDay(mdb::TradingDay* record) = 0;
	virtual void UpdateTradingDay(mdb::TradingDay* record) = 0;
	virtual void SelectTradingDay(std::list<mdb::TradingDay*>& records) = 0;
	virtual void TruncateTradingDay() = 0;

	virtual void CreateExchange() = 0;
	virtual void DropExchange() = 0;
	virtual void InsertExchange(mdb::Exchange* record) = 0;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) = 0;
	virtual void DeleteExchange(mdb::Exchange* record) = 0;
	virtual void UpdateExchange(mdb::Exchange* record) = 0;
	virtual void SelectExchange(std::list<mdb::Exchange*>& records) = 0;
	virtual void TruncateExchange() = 0;

	virtual void CreateProduct() = 0;
	virtual void DropProduct() = 0;
	virtual void InsertProduct(mdb::Product* record) = 0;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) = 0;
	virtual void DeleteProduct(mdb::Product* record) = 0;
	virtual void UpdateProduct(mdb::Product* record) = 0;
	virtual void SelectProduct(std::list<mdb::Product*>& records) = 0;
	virtual void TruncateProduct() = 0;

	virtual void CreateInstrument() = 0;
	virtual void DropInstrument() = 0;
	virtual void InsertInstrument(mdb::Instrument* record) = 0;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) = 0;
	virtual void DeleteInstrument(mdb::Instrument* record) = 0;
	virtual void UpdateInstrument(mdb::Instrument* record) = 0;
	virtual void SelectInstrument(std::list<mdb::Instrument*>& records) = 0;
	virtual void TruncateInstrument() = 0;

	virtual void CreatePrimaryAccount() = 0;
	virtual void DropPrimaryAccount() = 0;
	virtual void InsertPrimaryAccount(mdb::PrimaryAccount* record) = 0;
	virtual void BatchInsertPrimaryAccount(std::list<mdb::PrimaryAccount*>* records) = 0;
	virtual void DeletePrimaryAccount(mdb::PrimaryAccount* record) = 0;
	virtual void DeletePrimaryAccountByOfferIDIndex(mdb::PrimaryAccount* record) = 0;
	virtual void UpdatePrimaryAccount(mdb::PrimaryAccount* record) = 0;
	virtual void SelectPrimaryAccount(std::list<mdb::PrimaryAccount*>& records) = 0;
	virtual void TruncatePrimaryAccount() = 0;

	virtual void CreateAccount() = 0;
	virtual void DropAccount() = 0;
	virtual void InsertAccount(mdb::Account* record) = 0;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) = 0;
	virtual void DeleteAccount(mdb::Account* record) = 0;
	virtual void UpdateAccount(mdb::Account* record) = 0;
	virtual void SelectAccount(std::list<mdb::Account*>& records) = 0;
	virtual void TruncateAccount() = 0;

	virtual void CreateCapital() = 0;
	virtual void DropCapital() = 0;
	virtual void InsertCapital(mdb::Capital* record) = 0;
	virtual void BatchInsertCapital(std::list<mdb::Capital*>* records) = 0;
	virtual void DeleteCapital(mdb::Capital* record) = 0;
	virtual void DeleteCapitalByTradingDayIndex(mdb::Capital* record) = 0;
	virtual void UpdateCapital(mdb::Capital* record) = 0;
	virtual void SelectCapital(std::list<mdb::Capital*>& records) = 0;
	virtual void TruncateCapital() = 0;

	virtual void CreatePosition() = 0;
	virtual void DropPosition() = 0;
	virtual void InsertPosition(mdb::Position* record) = 0;
	virtual void BatchInsertPosition(std::list<mdb::Position*>* records) = 0;
	virtual void DeletePosition(mdb::Position* record) = 0;
	virtual void DeletePositionByAccountIndex(mdb::Position* record) = 0;
	virtual void DeletePositionByTradingDayIndex(mdb::Position* record) = 0;
	virtual void UpdatePosition(mdb::Position* record) = 0;
	virtual void SelectPosition(std::list<mdb::Position*>& records) = 0;
	virtual void TruncatePosition() = 0;

	virtual void CreatePositionDetail() = 0;
	virtual void DropPositionDetail() = 0;
	virtual void InsertPositionDetail(mdb::PositionDetail* record) = 0;
	virtual void BatchInsertPositionDetail(std::list<mdb::PositionDetail*>* records) = 0;
	virtual void DeletePositionDetail(mdb::PositionDetail* record) = 0;
	virtual void DeletePositionDetailByTradeMatchIndex(mdb::PositionDetail* record) = 0;
	virtual void DeletePositionDetailByTradingDayIndex(mdb::PositionDetail* record) = 0;
	virtual void UpdatePositionDetail(mdb::PositionDetail* record) = 0;
	virtual void SelectPositionDetail(std::list<mdb::PositionDetail*>& records) = 0;
	virtual void TruncatePositionDetail() = 0;

	virtual void CreateOrder() = 0;
	virtual void DropOrder() = 0;
	virtual void InsertOrder(mdb::Order* record) = 0;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) = 0;
	virtual void DeleteOrder(mdb::Order* record) = 0;
	virtual void UpdateOrder(mdb::Order* record) = 0;
	virtual void SelectOrder(std::list<mdb::Order*>& records) = 0;
	virtual void TruncateOrder() = 0;

	virtual void CreateTrade() = 0;
	virtual void DropTrade() = 0;
	virtual void InsertTrade(mdb::Trade* record) = 0;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) = 0;
	virtual void DeleteTrade(mdb::Trade* record) = 0;
	virtual void UpdateTrade(mdb::Trade* record) = 0;
	virtual void SelectTrade(std::list<mdb::Trade*>& records) = 0;
	virtual void TruncateTrade() = 0;

};
