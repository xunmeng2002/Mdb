// 三种表实现的单次操作成本：在 10 万行表上各测 Select / Update / InsertErase 一行。
// 计时器与 CSV 口径复用 Spark 的公开头；多读者扩展性与撕裂读仍由 TestConcurrency 负责。
#include "Common/MdbStructs.h"
#include "LockBased/MdbTables.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/StableRowTables.h"
#include "TestSupport/ImplementationNames.h"
#include "TestSupport/TestSupport.h"

#include <Spark/TemplateLib/Benchmark/BenchmarkRunner.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <string>
#include <utility>

namespace Benchmark = Spark::Benchmark;

namespace
{
	using Mdb::TradingDay;
	using TestSupport::FillTradingDay;
	using TestSupport::ImplementationName;
	using TestSupport::InsertTradingDayRows;

	// 与 TestConcurrency 同规模，两份读数可以直接对照
	constexpr int RowCount = 100000;

	// 空表也能跑出漂亮的数字：抽查主键读不回就当场失败，不给错误读数留出口
	template <typename TableType>
	void RequirePrefilledTable(TableType& table)
	{
		for (int primaryKey = 1; primaryKey <= RowCount; primaryKey += 1000)
		{
			if (!table.PrimaryKey->Select(primaryKey).has_value())
			{
				std::fprintf(stderr, "%s: primary key %d missing after prefill\n", ImplementationName<TableType>(), primaryKey);
				std::exit(1);
			}
		}
	}

	template <typename TableType>
	void RunBenchmarks()
	{
		TableType table;
		if (!InsertTradingDayRows(table, RowCount))
		{
			std::fprintf(stderr, "%s: prefill rejected rows\n", ImplementationName<TableType>());
			std::exit(1);
		}
		RequirePrefilledTable(table);

		const char* implementationName = ImplementationName<TableType>();
		auto measure = [implementationName](const char* operationName, auto&& measuredOperation)
		{
			const std::string benchmarkName = std::string(implementationName) + "." + operationName;
			Benchmark::Measure(benchmarkName.c_str(), std::forward<decltype(measuredOperation)>(measuredOperation));
		};

		// 键在 1..RowCount 内轮换，命中表内已有行
		unsigned int keySeed = 1013904223u;
		auto nextPrimaryKey = [&keySeed](std::int32_t keyBase)
		{
			keySeed = keySeed * 1664525u + 1013904223u;
			return static_cast<std::int32_t>(keySeed % RowCount) + keyBase;
		};

		auto select = [&table, &nextPrimaryKey]() -> std::size_t
		{
			return table.PrimaryKey->Select(nextPrimaryKey(1)).has_value() ? 1u : 0u;
		};

		// 计时循环里只改主键，其余字段共用一份模板，避免把格式化成本算进表操作
		TradingDay updateRecord{};
		FillTradingDay(updateRecord, 1, 1);
		auto update = [&table, &nextPrimaryKey, &updateRecord]() -> std::size_t
		{
			updateRecord.Pk = nextPrimaryKey(1);
			return table.Update(updateRecord, false) ? 1u : 0u;
		};

		// Insert 后立删同一键，行数不变；键取在预置范围之外，保证是空位
		TradingDay eraseKey{};
		auto insertErase = [&table, &nextPrimaryKey, &updateRecord, &eraseKey]() -> std::size_t
		{
			const std::int32_t primaryKey = nextPrimaryKey(RowCount + 1);
			TradingDay* row = TradingDay::Allocate();
			*row = updateRecord;
			row->Pk = primaryKey;
			if (!table.Insert(row))
			{
				return 0u;
			}
			eraseKey.Pk = primaryKey;
			return table.Erase(eraseKey) ? 1u : 0u;
		};

		measure("Select", select);
		measure("Update", update);
		measure("InsertErase", insertErase);
	}
}

int main()
{
	std::setvbuf(stdout, nullptr, _IONBF, 0);
	Benchmark::PrintHeader();
	RunBenchmarks<Mdb::TradingDayTable>();
	RunBenchmarks<Mdb::Rcu::PointerSwap::TradingDayTable>();
	RunBenchmarks<Mdb::Rcu::StableRow::TradingDayTable>();
	return 0;
}
