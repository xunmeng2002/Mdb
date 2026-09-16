// 本文件由 ../Templates/Cpp/Mdb/ModuleTableList.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "TableList.h"
#include "MdbStructs.h"

using namespace Mdb;
namespace Mdb::Full
{
	inline const unsigned int FullTableIds[] =
	{
		TradingDay::TableId,
		Exchange::TableId,
		Product::TableId,
		Instrument::TableId,
		PrimaryAccount::TableId,
		Account::TableId,
		Capital::TableId,
		Position::TableId,
		PositionDetail::TableId,
		Order::TableId,
		Trade::TableId,
	};
	inline const TableList FullTableList = { "Full", FullTableIds, static_cast<int>(sizeof(FullTableIds) / sizeof(FullTableIds[0])) };
}
