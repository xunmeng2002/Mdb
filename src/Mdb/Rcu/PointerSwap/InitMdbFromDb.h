// 本文件由 ../Templates/Cpp/Mdb/Rcu/PointerSwap/InitMdbFromDb.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "Common/TableList.h"
#include <DbAdapters/DbInterface/Db.h>

using DbAdapters::Db;
namespace Mdb::Rcu::PointerSwap
{
	class InitMdbFromDb
	{
	public:
		static void LoadTables(Mdb* mdb, Db* db, const TableList& tableList);

		static void LoadTradingDays(Mdb* mdb, Db* db);
		static void LoadExchanges(Mdb* mdb, Db* db);
		static void LoadProducts(Mdb* mdb, Db* db);
		static void LoadInstruments(Mdb* mdb, Db* db);
		static void LoadPrimaryAccounts(Mdb* mdb, Db* db);
		static void LoadAccounts(Mdb* mdb, Db* db);
		static void LoadCapitals(Mdb* mdb, Db* db);
		static void LoadPositions(Mdb* mdb, Db* db);
		static void LoadPositionDetails(Mdb* mdb, Db* db);
		static void LoadOrders(Mdb* mdb, Db* db);
		static void LoadTrades(Mdb* mdb, Db* db);
	};
}
