#pragma once
#include "MdbTables.h"
#include "MdbSubscriber.h"
#include "DBSubscriber.h"


namespace mdb
{
	class Mdb : public DBSubscriber
	{
	public:
		Mdb();
		void Subscribe(MdbSubscriber* mdbSubscriber);
		void UnSubscribe();
		void InitDB();
		void SetInitStatus(bool initStatus);
		void Dump(const char* dir);
		void TruncateTables();
		
		virtual void OnDBConnected() override;
		virtual void OnDBDisConnected() override;

	public:
		TradingDayTable* t_TradingDay;
		ExchangeTable* t_Exchange;
		ProductTable* t_Product;
		InstrumentTable* t_Instrument;
		AccountTable* t_Account;
		PositionTable* t_Position;
		OrderTable* t_Order;
		TradeTable* t_Trade;
		MdTickTable* t_MdTick;
	};
}

