// 三种实现的读扩展性对照：同一负载、同一断言，只改读线程数（2 / 4 / 8），写线程恒为一个。
// 不接物理库：表对象直接构造，不走 InitDb / AsyncDbWriter。
// 每轮跑固定时长：写侧完成多少条 Update 是观测量之一（读写锁版在读压下会骤降），不做时长断言。
// 读写两端按命令行给出的操作间隔（微秒）各做一次操作；缺省 1 ms，给 0 即满载自旋的饱和最坏情况。
#include "LockBased/MdbTables.h"
#include "Common/MdbStructs.h"
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include "Rcu/StableRow/StableRowTables.h"
#include "TestSupport/TestSupport.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <optional>
#include <thread>
#include <vector>

namespace
{
	using Mdb::TradingDay;
	using TestSupport::CheckReporter;
	using TestSupport::FillTradingDay;
	using TestSupport::HasPairedDates;

	constexpr int RowCount = 100000;
	constexpr std::chrono::milliseconds MeasurementDuration{ 1000 };
	// 读者在窗口后仍可能被写者的独占锁挡在 Select 里，给一段宽限让它们自行退出，
	// 否则读写锁版的写者要等到读者恰好让出锁才能完成最后一次 Update，运行时长不可控
	constexpr std::chrono::milliseconds ReaderGraceDuration{ 300 };
	// 读写两端的操作间隔：模拟真实业务里两次操作之间的计算与 IO，读者不再把共享锁占满。
	// 命令行第一个参数给微秒数，缺省 1 ms；给 0 即回到「满载自旋」的饱和最坏情况。
	// 用忙等而不是 sleep：Windows 的睡眠精度是毫秒级偏上，会把间隔撑大。
	std::chrono::microseconds ParseOperationInterval(int argc, char** argv)
	{
		if (argc < 2)
		{
			return std::chrono::microseconds{ 1000 };
		}
		const long long microseconds = std::strtoll(argv[1], nullptr, 10);
		return std::chrono::microseconds{ microseconds < 0 ? 0 : microseconds };
	}

	void SpinOperationInterval(std::chrono::microseconds operationInterval)
	{
		const auto deadline = std::chrono::steady_clock::now() + operationInterval;
		while (std::chrono::steady_clock::now() < deadline)
		{
		}
	}

	struct ScenarioOutcome
	{
		bool allRowsInserted = true;
		long long readCount = 0;
		int updateCount = 0;
		int tornReadCount = 0;
		int missingRowCount = 0;
		int failedUpdateCount = 0;
		int mismatchedRowCount = 0;
		int readerWithoutProgressCount = 0;
		double elapsedMilliseconds = 0;
	};

	// 读者按各自伪随机序列轮换主键，写者顺序改写全部行；行从不删除，任何读不到都是缺陷。
	template <typename TableType>
	ScenarioOutcome RunReaderScenario(int readerCount, std::chrono::microseconds operationInterval)
	{
		const bool hasOperationInterval = operationInterval.count() > 0;
		TableType table;
		bool allRowsInserted = true;
		for (int rowIndex = 0; rowIndex < RowCount; ++rowIndex)
		{
			TradingDay* row = TradingDay::Allocate();
			FillTradingDay(*row, rowIndex + 1, 0);
			// 被拒绝的记录由 Insert 自行归还对象池，这里不能重复释放
			allRowsInserted = table.Insert(row) && allRowsInserted;
		}

		std::atomic<bool> startSignal{ false };
		std::atomic<bool> stopSignal{ false };
		std::atomic<int> tornReadCount{ 0 };
		std::atomic<int> missingRowCount{ 0 };
		std::vector<long long> readCounts(readerCount, 0);
		std::vector<std::thread> readers;
		readers.reserve(readerCount);
		for (int readerIndex = 0; readerIndex < readerCount; ++readerIndex)
		{
			readers.emplace_back([&table, &startSignal, &stopSignal, &tornReadCount, &missingRowCount, &readCounts, readerIndex,
				hasOperationInterval, operationInterval]()
			{
				while (!startSignal.load(std::memory_order_acquire))
				{
					std::this_thread::yield();
				}
				const auto readerStartedAt = std::chrono::steady_clock::now();
				unsigned int keySeed = 2654435761u * static_cast<unsigned int>(readerIndex + 1);
				long long readCount = 0;
				for (;;)
				{
					// 无操作间隔时每 1024 次读才看一次时钟；有间隔时必须每次看，否则会多跑一整个间隔
					if ((hasOperationInterval || (readCount & 1023) == 0)
						&& (stopSignal.load(std::memory_order_relaxed)
							|| std::chrono::steady_clock::now() - readerStartedAt >= MeasurementDuration + ReaderGraceDuration))
					{
						break;
					}
					keySeed = keySeed * 1664525u + 1013904223u;
					const std::int32_t primaryKey = static_cast<std::int32_t>(keySeed % RowCount) + 1;
					const std::optional<TradingDay> copy = table.PrimaryKey->Select(primaryKey);
					if (!copy.has_value())
					{
						++missingRowCount;
					}
					else if (!HasPairedDates(*copy))
					{
						++tornReadCount;
					}
					++readCount;
					if (hasOperationInterval)
					{
						SpinOperationInterval(operationInterval);
					}
				}
				readCounts[readerIndex] = readCount;
			});
		}

		std::vector<int> lastWrittenValue(RowCount, 0);
		const auto startedAt = std::chrono::steady_clock::now();
		startSignal.store(true, std::memory_order_release);
		unsigned int writeSeed = 1013904223u;
		int updateCount = 0;
		int failedUpdateCount = 0;
		for (;;)
		{
			if (std::chrono::steady_clock::now() - startedAt >= MeasurementDuration)
			{
				break;
			}
			// 表大到十万行后，写者也按伪随机挑行：顺序写只会碰到前几千行，与读者的十万行扫读几乎不重叠
			writeSeed = writeSeed * 1664525u + 1013904223u;
			const int rowIndex = static_cast<int>(writeSeed % RowCount);
			TradingDay record{};
			FillTradingDay(record, rowIndex + 1, updateCount + 1);
			if (table.Update(record))
			{
				lastWrittenValue[rowIndex] = updateCount + 1;
			}
			else
			{
				++failedUpdateCount;
			}
			++updateCount;
			if (hasOperationInterval)
			{
				SpinOperationInterval(operationInterval);
			}
		}
		stopSignal.store(true, std::memory_order_release);
		for (std::thread& reader : readers)
		{
			reader.join();
		}
		const auto finishedAt = std::chrono::steady_clock::now();

		ScenarioOutcome outcome;
		outcome.allRowsInserted = allRowsInserted;
		outcome.updateCount = updateCount;
		outcome.failedUpdateCount = failedUpdateCount;
		outcome.tornReadCount = tornReadCount.load();
		outcome.missingRowCount = missingRowCount.load();
		outcome.elapsedMilliseconds = std::chrono::duration<double, std::milli>(finishedAt - startedAt).count();
		for (long long readCount : readCounts)
		{
			outcome.readCount += readCount;
			outcome.readerWithoutProgressCount += readCount > 0 ? 0 : 1;
		}
		for (int rowIndex = 0; rowIndex < RowCount; ++rowIndex)
		{
			char expectedText[16];
			std::snprintf(expectedText, sizeof(expectedText), "%d", lastWrittenValue[rowIndex]);
			const std::optional<TradingDay> finalRow = table.PrimaryKey->Select(rowIndex + 1);
			const bool matchesExpectedValue = finalRow.has_value()
				&& std::strcmp(finalRow->CurrTradingDay, expectedText) == 0
				&& std::strcmp(finalRow->PreTradingDay, expectedText) == 0;
			outcome.mismatchedRowCount += matchesExpectedValue ? 0 : 1;
		}
		return outcome;
	}

	template <typename TableType>
	int RunConfiguration(const char* implementationName, int readerCount, std::chrono::microseconds operationInterval)
	{
		char configurationName[96];
		std::snprintf(configurationName, sizeof(configurationName), "%s readers=%d interval=%lldus", implementationName,
			readerCount, static_cast<long long>(operationInterval.count()));
		CheckReporter report(configurationName);
		const ScenarioOutcome outcome = RunReaderScenario<TableType>(readerCount, operationInterval);
		std::printf("%s reads=%lld updates=%d torn=%d missing=%d failedUpdates=%d mismatchedRows=%d elapsedMs=%.1f\n",
			configurationName, outcome.readCount, outcome.updateCount, outcome.tornReadCount, outcome.missingRowCount,
			outcome.failedUpdateCount, outcome.mismatchedRowCount, outcome.elapsedMilliseconds);
		report.Check(outcome.allRowsInserted, "allRowsInserted");
		report.Check(outcome.tornReadCount == 0, "noTornReads");
		report.Check(outcome.missingRowCount == 0, "noMissingRows");
		report.Check(outcome.failedUpdateCount == 0, "allUpdatesApplied");
		report.Check(outcome.updateCount > 0, "writerMadeProgress");
		report.Check(outcome.readerWithoutProgressCount == 0, "everyReaderMadeProgress");
		report.Check(outcome.mismatchedRowCount == 0, "finalStateMatchesWriter");
		return report.FailureCount();
	}
}

int main(int argc, char** argv)
{
	std::setvbuf(stdout, nullptr, _IONBF, 0);
	const std::chrono::microseconds operationInterval = ParseOperationInterval(argc, argv);
	int failureCount = 0;
	for (int readerCount : { 2, 4, 8 })
	{
		failureCount += RunConfiguration<Mdb::TradingDayTable>("LockBased", readerCount, operationInterval);
		failureCount += RunConfiguration<Mdb::Rcu::PointerSwap::TradingDayTable>("PointerSwap", readerCount, operationInterval);
		failureCount += RunConfiguration<Mdb::Rcu::StableRow::TradingDayTable>("StableRow", readerCount, operationInterval);
	}
	std::printf("failures=%d\n", failureCount);
	return failureCount == 0 ? 0 : 1;
}
