// 本文件由 ../Templates/Cpp/Mdb/ModuleTableList.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "TableList.h"
#include "MdbStructs.h"

using namespace mdb;
namespace mdb::full
{
	inline const unsigned int kfullTableIDs[] =
	{
		TradingDay::TableID,
		Exchange::TableID,
		Product::TableID,
		Instrument::TableID,
		PrimaryAccount::TableID,
		Account::TableID,
		Capital::TableID,
		Position::TableID,
		PositionDetail::TableID,
		Order::TableID,
		Trade::TableID,
	};
	inline const TableList fullTableList = { "full", kfullTableIDs, static_cast<int>(sizeof(kfullTableIDs) / sizeof(kfullTableIDs[0])) };
}
