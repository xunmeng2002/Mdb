#pragma once
#include "MdbStructs.h"
#include <atomic>
#include <list>


class MdbSubscriber
{
public:
	MdbSubscriber()
		:m_Connected(false)
	{
	}
	virtual void OnCreateTables() {}
	virtual void OnDropTables() {}
	virtual void OnTruncateTables() {}
	
	virtual void OnTradingDayInsert(mdb::TradingDay* record) {}
	virtual void OnTradingDayBatchInsert(std::list<mdb::TradingDay*>* records) {}
	virtual void OnTradingDayErase(mdb::TradingDay* record) {}
	virtual void OnTradingDayUpdate(mdb::TradingDay* record) {}
	virtual void OnTradingDayTruncate() {}
	
	virtual void OnExchangeInsert(mdb::Exchange* record) {}
	virtual void OnExchangeBatchInsert(std::list<mdb::Exchange*>* records) {}
	virtual void OnExchangeErase(mdb::Exchange* record) {}
	virtual void OnExchangeUpdate(mdb::Exchange* record) {}
	virtual void OnExchangeTruncate() {}
	
	virtual void OnProductInsert(mdb::Product* record) {}
	virtual void OnProductBatchInsert(std::list<mdb::Product*>* records) {}
	virtual void OnProductErase(mdb::Product* record) {}
	virtual void OnProductUpdate(mdb::Product* record) {}
	virtual void OnProductTruncate() {}
	
	virtual void OnInstrumentInsert(mdb::Instrument* record) {}
	virtual void OnInstrumentBatchInsert(std::list<mdb::Instrument*>* records) {}
	virtual void OnInstrumentErase(mdb::Instrument* record) {}
	virtual void OnInstrumentUpdate(mdb::Instrument* record) {}
	virtual void OnInstrumentTruncate() {}
	
	virtual void OnPrimaryAccountInsert(mdb::PrimaryAccount* record) {}
	virtual void OnPrimaryAccountBatchInsert(std::list<mdb::PrimaryAccount*>* records) {}
	virtual void OnPrimaryAccountErase(mdb::PrimaryAccount* record) {}
	virtual void OnPrimaryAccountEraseByOfferIDIndex(mdb::PrimaryAccount* record) {}
	virtual void OnPrimaryAccountUpdate(mdb::PrimaryAccount* record) {}
	virtual void OnPrimaryAccountTruncate() {}
	
	virtual void OnAccountInsert(mdb::Account* record) {}
	virtual void OnAccountBatchInsert(std::list<mdb::Account*>* records) {}
	virtual void OnAccountErase(mdb::Account* record) {}
	virtual void OnAccountUpdate(mdb::Account* record) {}
	virtual void OnAccountTruncate() {}
	
	virtual void OnCapitalInsert(mdb::Capital* record) {}
	virtual void OnCapitalBatchInsert(std::list<mdb::Capital*>* records) {}
	virtual void OnCapitalErase(mdb::Capital* record) {}
	virtual void OnCapitalEraseByTradingDayIndex(mdb::Capital* record) {}
	virtual void OnCapitalUpdate(mdb::Capital* record) {}
	virtual void OnCapitalTruncate() {}
	
	virtual void OnPositionInsert(mdb::Position* record) {}
	virtual void OnPositionBatchInsert(std::list<mdb::Position*>* records) {}
	virtual void OnPositionErase(mdb::Position* record) {}
	virtual void OnPositionEraseByAccountIndex(mdb::Position* record) {}
	virtual void OnPositionEraseByTradingDayIndex(mdb::Position* record) {}
	virtual void OnPositionUpdate(mdb::Position* record) {}
	virtual void OnPositionTruncate() {}
	
	virtual void OnPositionDetailInsert(mdb::PositionDetail* record) {}
	virtual void OnPositionDetailBatchInsert(std::list<mdb::PositionDetail*>* records) {}
	virtual void OnPositionDetailErase(mdb::PositionDetail* record) {}
	virtual void OnPositionDetailEraseByTradeMatchIndex(mdb::PositionDetail* record) {}
	virtual void OnPositionDetailEraseByTradingDayIndex(mdb::PositionDetail* record) {}
	virtual void OnPositionDetailUpdate(mdb::PositionDetail* record) {}
	virtual void OnPositionDetailTruncate() {}
	
	virtual void OnOrderInsert(mdb::Order* record) {}
	virtual void OnOrderBatchInsert(std::list<mdb::Order*>* records) {}
	virtual void OnOrderErase(mdb::Order* record) {}
	virtual void OnOrderUpdate(mdb::Order* record) {}
	virtual void OnOrderTruncate() {}
	
	virtual void OnTradeInsert(mdb::Trade* record) {}
	virtual void OnTradeBatchInsert(std::list<mdb::Trade*>* records) {}
	virtual void OnTradeErase(mdb::Trade* record) {}
	virtual void OnTradeUpdate(mdb::Trade* record) {}
	virtual void OnTradeTruncate() {}
	

public:
	std::atomic<bool> m_Connected;
};

