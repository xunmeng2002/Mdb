#pragma once
#include "MdbTables.h"
#include "MdbTableBase.h"
#include "TableList.h"
#include <DBAdapters/DBInterface/MdbSubscriber.h>
#include <DBAdapters/DBInterface/DBSubscriber.h>
#include <vector>


namespace mdb
{
	class Mdb : public DBSubscriber
	{
	public:
		explicit Mdb(const TableList& tableList);
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
		TradingDayTable* t_TradingDay = nullptr;
		ExchangeTable* t_Exchange = nullptr;
		ProductTable* t_Product = nullptr;
		InstrumentTable* t_Instrument = nullptr;
		PrimaryAccountTable* t_PrimaryAccount = nullptr;
		AccountTable* t_Account = nullptr;
		CapitalTable* t_Capital = nullptr;
		PositionTable* t_Position = nullptr;
		PositionDetailTable* t_PositionDetail = nullptr;
		OrderTable* t_Order = nullptr;
		TradeTable* t_Trade = nullptr;
	private:
		MdbSubscriber* m_MdbSubscriber;
		std::vector<MdbTableBase*> m_Tables;
	};
}
