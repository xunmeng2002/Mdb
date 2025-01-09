#pragma once
#include "MdbStructs.h"
#include <string>
#include <vector>
#include <list>


class DB
{
public:
	virtual bool Connect() = 0;
	virtual void DisConnect() = 0;
	virtual void InitDB() = 0;
	virtual void TruncateSessionTables() = 0;
	virtual void TruncateTables() = 0;
	
	virtual void InsertTradingDay(mdb::TradingDay* record) = 0;
	virtual void BatchInsertTradingDay(std::list<mdb::TradingDay*>* records) = 0;
	virtual void DeleteTradingDay(mdb::TradingDay* record) = 0;
	virtual void UpdateTradingDay(mdb::TradingDay* record) = 0;
	virtual void SelectTradingDay(std::vector<mdb::TradingDay*>& records) = 0;
	virtual void TruncateTradingDay() = 0;
	
	virtual void InsertExchange(mdb::Exchange* record) = 0;
	virtual void BatchInsertExchange(std::list<mdb::Exchange*>* records) = 0;
	virtual void DeleteExchange(mdb::Exchange* record) = 0;
	virtual void UpdateExchange(mdb::Exchange* record) = 0;
	virtual void SelectExchange(std::vector<mdb::Exchange*>& records) = 0;
	virtual void TruncateExchange() = 0;
	
	virtual void InsertProduct(mdb::Product* record) = 0;
	virtual void BatchInsertProduct(std::list<mdb::Product*>* records) = 0;
	virtual void DeleteProduct(mdb::Product* record) = 0;
	virtual void UpdateProduct(mdb::Product* record) = 0;
	virtual void SelectProduct(std::vector<mdb::Product*>& records) = 0;
	virtual void TruncateProduct() = 0;
	
	virtual void InsertInstrument(mdb::Instrument* record) = 0;
	virtual void BatchInsertInstrument(std::list<mdb::Instrument*>* records) = 0;
	virtual void DeleteInstrument(mdb::Instrument* record) = 0;
	virtual void UpdateInstrument(mdb::Instrument* record) = 0;
	virtual void SelectInstrument(std::vector<mdb::Instrument*>& records) = 0;
	virtual void TruncateInstrument() = 0;
	
	virtual void InsertAccount(mdb::Account* record) = 0;
	virtual void BatchInsertAccount(std::list<mdb::Account*>* records) = 0;
	virtual void DeleteAccount(mdb::Account* record) = 0;
	virtual void UpdateAccount(mdb::Account* record) = 0;
	virtual void SelectAccount(std::vector<mdb::Account*>& records) = 0;
	virtual void TruncateAccount() = 0;
	
	virtual void InsertPosition(mdb::Position* record) = 0;
	virtual void BatchInsertPosition(std::list<mdb::Position*>* records) = 0;
	virtual void DeletePosition(mdb::Position* record) = 0;
	virtual void DeletePositionByAccountIndex(mdb::Position* record) = 0;
	virtual void UpdatePosition(mdb::Position* record) = 0;
	virtual void SelectPosition(std::vector<mdb::Position*>& records) = 0;
	virtual void TruncatePosition() = 0;
	
	virtual void InsertOrder(mdb::Order* record) = 0;
	virtual void BatchInsertOrder(std::list<mdb::Order*>* records) = 0;
	virtual void DeleteOrder(mdb::Order* record) = 0;
	virtual void UpdateOrder(mdb::Order* record) = 0;
	virtual void SelectOrder(std::vector<mdb::Order*>& records) = 0;
	virtual void TruncateOrder() = 0;
	
	virtual void InsertTrade(mdb::Trade* record) = 0;
	virtual void BatchInsertTrade(std::list<mdb::Trade*>* records) = 0;
	virtual void DeleteTrade(mdb::Trade* record) = 0;
	virtual void UpdateTrade(mdb::Trade* record) = 0;
	virtual void SelectTrade(std::vector<mdb::Trade*>& records) = 0;
	virtual void TruncateTrade() = 0;
	
	virtual void InsertMdTick(mdb::MdTick* record) = 0;
	virtual void BatchInsertMdTick(std::list<mdb::MdTick*>* records) = 0;
	virtual void DeleteMdTick(mdb::MdTick* record) = 0;
	virtual void UpdateMdTick(mdb::MdTick* record) = 0;
	virtual void SelectMdTick(std::vector<mdb::MdTick*>& records) = 0;
	virtual void TruncateMdTick() = 0;
	
};
