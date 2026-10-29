#pragma once

#include "Common/MdbStructs.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

// 测试程序共用的辅助：定长字符串写入，以及撕裂读检测用的成对日期写入。
namespace TestSupport
{
	template <std::size_t Size>
	void FillFixedString(char (&fixedString)[Size], const char* value)
	{
		std::snprintf(fixedString, Size, "%s", value);
	}

	inline void FillTradingDay(Mdb::TradingDay& record, std::int32_t primaryKey, int value)
	{
		record.Pk = primaryKey;
		std::snprintf(record.CurrTradingDay, sizeof(record.CurrTradingDay), "%d", value);
		std::snprintf(record.PreTradingDay, sizeof(record.PreTradingDay), "%d", value);
	}

	inline bool HasPairedDates(const Mdb::TradingDay& record)
	{
		return std::strcmp(record.CurrTradingDay, record.PreTradingDay) == 0;
	}

	// 按主键 1..rowCount 灌满一张交易日表；被拒绝的记录由 Insert 自行归还对象池
	template <typename TableType>
	bool InsertTradingDayRows(TableType& table, int rowCount)
	{
		bool allInserted = true;
		for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex)
		{
			Mdb::TradingDay* row = Mdb::TradingDay::Allocate();
			FillTradingDay(*row, rowIndex + 1, 0);
			allInserted = table.Insert(row) && allInserted;
		}
		return allInserted;
	}
}
