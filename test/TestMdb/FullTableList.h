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
	inline const TableList fullTableList = { "full", kfullTableIDs, (int)(sizeof(kfullTableIDs) / sizeof(kfullTableIDs[0])) };
}
