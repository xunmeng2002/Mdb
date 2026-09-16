// 本文件由 ../Templates/Cpp/Mdb/InitMdbFromCsv.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "TableList.h"

namespace Mdb
{
	class InitMdbFromCsv
	{
	public:
		static void LoadTables(Mdb* mdb, const char* dir, const TableList& tableList);
	private:
		static void LoadTradingDayTable(Mdb* mdb, const char* dir);
		static void LoadExchangeTable(Mdb* mdb, const char* dir);
		static void LoadProductTable(Mdb* mdb, const char* dir);
		static void LoadInstrumentTable(Mdb* mdb, const char* dir);
		static void LoadPrimaryAccountTable(Mdb* mdb, const char* dir);
		static void LoadAccountTable(Mdb* mdb, const char* dir);
		static void LoadCapitalTable(Mdb* mdb, const char* dir);
		static void LoadPositionTable(Mdb* mdb, const char* dir);
		static void LoadPositionDetailTable(Mdb* mdb, const char* dir);
		static void LoadOrderTable(Mdb* mdb, const char* dir);
		static void LoadTradeTable(Mdb* mdb, const char* dir);
	};
}
