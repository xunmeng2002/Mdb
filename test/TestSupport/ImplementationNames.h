#pragma once

#include "LockBased/MdbTables.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/StableRowTables.h"

// 读数与基准名要标明是哪种实现，类型本身不带这个名字
namespace TestSupport
{
	template <typename TableType>
	const char* ImplementationName();

	template <>
	inline const char* ImplementationName<Mdb::TradingDayTable>()
	{
		return "LockBased";
	}

	template <>
	inline const char* ImplementationName<Mdb::Rcu::PointerSwap::TradingDayTable>()
	{
		return "PointerSwap";
	}

	template <>
	inline const char* ImplementationName<Mdb::Rcu::StableRow::TradingDayTable>()
	{
		return "StableRow";
	}
}
