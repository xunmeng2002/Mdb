// 本文件由 ../Templates/Cpp/Mdb/Rcu/PointerSwap/InitMdbFromCsv.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "Common/TableList.h"

namespace Mdb::Rcu::PointerSwap
{
	class InitMdbFromCsv
	{
	public:
		static void LoadTables(Mdb* mdb, const char* dir, const TableList& tableList);
	private:
		static void LoadTradingDays(Mdb* mdb, const char* dir);
		static void LoadExchanges(Mdb* mdb, const char* dir);
		static void LoadProducts(Mdb* mdb, const char* dir);
		static void LoadInstruments(Mdb* mdb, const char* dir);
		static void LoadPrimaryAccounts(Mdb* mdb, const char* dir);
		static void LoadAccounts(Mdb* mdb, const char* dir);
		static void LoadCapitals(Mdb* mdb, const char* dir);
		static void LoadPositions(Mdb* mdb, const char* dir);
		static void LoadPositionDetails(Mdb* mdb, const char* dir);
		static void LoadOrders(Mdb* mdb, const char* dir);
		static void LoadTrades(Mdb* mdb, const char* dir);
	};
}
