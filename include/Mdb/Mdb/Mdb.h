#pragma once
#include <Mdb/Mdb/MdbExport.h>
#include <Mdb/Mdb/MdbTables.h>
#include <Mdb/Mdb/MdbSubscriber.h>
#include <PersonalLib/Core/DB/DBSubscriber.h>


namespace mdb
{
	class MDB_EXPORTS Mdb : public DBSubscriber
	{
	public:
		Mdb();
		void Subscribe(MdbSubscriber* mdbSubscriber);
		void UnSubscribe();
		void InitDB();
		void SetInitStatus(bool initStatus);
		void Dump(const char* dir);
		void CreateTables();
		void DropTables();
		void TruncateTables();
		
		virtual void OnDBConnected() override;
		virtual void OnDBDisConnected() override;

	public:
		TradingDayTable* t_TradingDay;
		ExchangeTable* t_Exchange;
		ProductTable* t_Product;
		InstrumentTable* t_Instrument;
		PrimaryAccountTable* t_PrimaryAccount;
		AccountTable* t_Account;
		CapitalTable* t_Capital;
		PositionTable* t_Position;
		PositionDetailTable* t_PositionDetail;
		OrderTable* t_Order;
		TradeTable* t_Trade;
	private:
		MdbSubscriber* m_MdbSubscriber;
	};
}

