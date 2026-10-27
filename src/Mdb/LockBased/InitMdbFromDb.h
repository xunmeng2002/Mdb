// 本文件由 ../Templates/Cpp/Mdb/LockBased/InitMdbFromDb.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "Model/TableList.h"
#include <DbAdapters/DbInterface/Db.h>

using DbAdapters::Db;
namespace Mdb
{
	class InitMdbFromDb
	{
	public:
		static void LoadTables(Mdb* mdb, Db* db, const TableList& tableList);

		static void LoadTradingDay(Mdb* mdb, Db* db);
		static void LoadExchange(Mdb* mdb, Db* db);
		static void LoadProduct(Mdb* mdb, Db* db);
		static void LoadInstrument(Mdb* mdb, Db* db);
		static void LoadPrimaryAccount(Mdb* mdb, Db* db);
		static void LoadAccount(Mdb* mdb, Db* db);
		static void LoadCapital(Mdb* mdb, Db* db);
		static void LoadPosition(Mdb* mdb, Db* db);
		static void LoadPositionDetail(Mdb* mdb, Db* db);
		static void LoadOrder(Mdb* mdb, Db* db);
		static void LoadTrade(Mdb* mdb, Db* db);
	};
}
