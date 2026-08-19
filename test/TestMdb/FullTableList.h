#pragma once
#include "TableList.h"
#include "MdbStructs.h"


namespace mdb
{
	inline const unsigned int kFullTableIDs[] =
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
	inline const TableList FullTableList = { "Full", kFullTableIDs, (int)(sizeof(kFullTableIDs) / sizeof(kFullTableIDs[0])) };
}
