// 本文件由 ../Templates/Cpp/Mdb/LockBased/InitMdbFromCsv.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Mdb.h"
#include "Model/TableList.h"

namespace Mdb
{
	class InitMdbFromCsv
	{
	public:
		static void LoadTables(Mdb* mdb, const char* dir, const TableList& tableList);
	private:
		static void LoadTradingDay(Mdb* mdb, const char* dir);
		static void LoadExchange(Mdb* mdb, const char* dir);
		static void LoadProduct(Mdb* mdb, const char* dir);
		static void LoadInstrument(Mdb* mdb, const char* dir);
		static void LoadPrimaryAccount(Mdb* mdb, const char* dir);
		static void LoadAccount(Mdb* mdb, const char* dir);
		static void LoadCapital(Mdb* mdb, const char* dir);
		static void LoadPosition(Mdb* mdb, const char* dir);
		static void LoadPositionDetail(Mdb* mdb, const char* dir);
		static void LoadOrder(Mdb* mdb, const char* dir);
		static void LoadTrade(Mdb* mdb, const char* dir);
	};
}
