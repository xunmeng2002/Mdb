#pragma once

#include "Common/MdbStructs.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

// 并发测试程序共用的辅助：撕裂读检测用的成对日期写入，以及逐条打印的断言记录。
namespace TestSupport
{
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

	class CheckReporter
	{
	public:
		explicit CheckReporter(const char* configurationName) : configurationName_(configurationName)
		{
		}

		void Check(bool passed, const char* checkName)
		{
			std::printf("%s %s=%d\n", configurationName_, checkName, passed ? 1 : 0);
			failureCount_ += passed ? 0 : 1;
		}

		int FailureCount() const
		{
			return failureCount_;
		}

	private:
		const char* configurationName_;
		int failureCount_ = 0;
	};
}
