// 本文件由 ../Templates/Cpp/Mdb/Mdb.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbTables.h"
#include "MdbTableBase.h"
#include "TableList.h"
#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <DbAdapters/DbInterface/DbSubscriber.h>
#include <vector>


namespace Mdb
{
	class Mdb : public DbAdapters::DbSubscriber
	{
	public:
		explicit Mdb(const TableList& tableList);
		void Subscribe(MdbSubscriber* subscriber);
		void UnSubscribe();
		void InitDb();
		void SetInitStatus(bool initStatus);
		void Dump(const char* dir);
		void CreateTables();
		void DropTables();
		void TruncateTables();

		virtual void OnDbConnected() override;
		virtual void OnDbDisConnected() override;

	public:
		TradingDayTable* TradingDay = nullptr;
		ExchangeTable* Exchange = nullptr;
		ProductTable* Product = nullptr;
		InstrumentTable* Instrument = nullptr;
		PrimaryAccountTable* PrimaryAccount = nullptr;
		AccountTable* Account = nullptr;
		CapitalTable* Capital = nullptr;
		PositionTable* Position = nullptr;
		PositionDetailTable* PositionDetail = nullptr;
		OrderTable* Order = nullptr;
		TradeTable* Trade = nullptr;
	private:
		MdbSubscriber* mdbSubscriber_;
		std::vector<MdbTableBase*> tables_;
	};
}
