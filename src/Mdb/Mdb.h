// 本文件由 ../Templates/Cpp/Mdb/Mdb.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbTables.h"
#include "MdbTableBase.h"
#include "TableList.h"
#include <DBAdapters/DBInterface/MdbSubscriber.h>
#include <DBAdapters/DBInterface/DBSubscriber.h>
#include <vector>


namespace mdb
{
	class Mdb : public dbadapters::DBSubscriber
	{
	public:
		explicit Mdb(const TableList& tableList);
		void Subscribe(MdbSubscriber* subscriber);
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
		TradingDayTable* tradingDay = nullptr;
		ExchangeTable* exchange = nullptr;
		ProductTable* product = nullptr;
		InstrumentTable* instrument = nullptr;
		PrimaryAccountTable* primaryAccount = nullptr;
		AccountTable* account = nullptr;
		CapitalTable* capital = nullptr;
		PositionTable* position = nullptr;
		PositionDetailTable* positionDetail = nullptr;
		OrderTable* order = nullptr;
		TradeTable* trade = nullptr;
	private:
		MdbSubscriber* mdbSubscriber;
		std::vector<MdbTableBase*> tables;
	};
}
