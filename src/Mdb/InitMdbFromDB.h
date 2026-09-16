// 本文件由 ../Templates/Cpp/Mdb/InitMdbFromDb.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "TableList.h"
#include <DBAdapters/DbInterface/Db.h>

using DbAdapters::Db;
namespace Mdb
{
	class InitMdbFromDB
	{
	public:
		static void LoadTables(Mdb* mdb, Db* db, const TableList& tableList);

		static void LoadTradingDayTable(Mdb* mdb, Db* db);
		static void LoadExchangeTable(Mdb* mdb, Db* db);
		static void LoadProductTable(Mdb* mdb, Db* db);
		static void LoadInstrumentTable(Mdb* mdb, Db* db);
		static void LoadPrimaryAccountTable(Mdb* mdb, Db* db);
		static void LoadAccountTable(Mdb* mdb, Db* db);
		static void LoadCapitalTable(Mdb* mdb, Db* db);
		static void LoadPositionTable(Mdb* mdb, Db* db);
		static void LoadPositionDetailTable(Mdb* mdb, Db* db);
		static void LoadOrderTable(Mdb* mdb, Db* db);
		static void LoadTradeTable(Mdb* mdb, Db* db);
	};
}
