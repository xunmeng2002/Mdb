// 集成测试：接物理库，验证异步写库、断连重连、从库加载。
// 判据一律是「另开连接读回」——内存表写对了不算数，落库了才算。
// 不接物理库的契约测试在 test/UnitTests。
#include "DatabaseSession.h"

#include "Common/MdbStructs.h"
#include "LockBased/InitMdbFromDb.h"

#include "doctest/doctest.h"

#include <DbAdapters/DbInterface/Db.h>
#include <DbAdapters/DbInterface/TypedTable.h>
#include <DbAdapters/DuckdbWrapper/DuckdbWrapper.h>
#include <DbAdapters/SqliteWrapper/SqliteWrapper.h>
#include <Spark/Core/Core.h>
#include <atomic>
#include <chrono>
#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace
{
	using DbAdapters::Db;
	using DbAdapters::DuckdbWrapper;
	using DbAdapters::SqliteWrapper;
	using DbAdapters::TypedTable;
	using ::Mdb::Exchange;
	using ::Mdb::TradingDay;

	const char* SqliteDatabasePath = "./Test.sqlitedb";
	const char* DuckdbDatabasePath = "./Test.duckdb";
	const char* ReconnectDatabasePath = "./TestReconnect.sqlitedb";

	constexpr std::chrono::milliseconds PendingDrainTimeout{ 5000 };
	// 写库失败后 DbAdapters 自己要退避 5 秒再重连，重连后的全量快照会比它更晚完成
	constexpr std::chrono::milliseconds ResyncTimeout{ 20000 };

	struct SqliteBackend
	{
		static std::unique_ptr<Db> Open() { return std::make_unique<SqliteWrapper>(SqliteDatabasePath); }
	};

	struct DuckdbBackend
	{
		static std::unique_ptr<Db> Open() { return std::make_unique<DuckdbWrapper>(DuckdbDatabasePath); }
	};

	TradingDay* AllocateTradingDay()
	{
		const std::string currentDate = Spark::Core::TimeUtility::GetLocalDate();
		TradingDay* tradingDay = TradingDay::Allocate();
		std::memset(tradingDay, 0, sizeof(TradingDay));
		tradingDay->Pk = 1;
		Spark::Core::TimeUtility::GetPreTradingDay(currentDate.c_str(), tradingDay->PreTradingDay);
		Spark::Core::TimeUtility::GetNextTradingDay(tradingDay->PreTradingDay, tradingDay->CurrTradingDay);
		return tradingDay;
	}

	Exchange* AllocateExchange(const char* exchangeId, const char* exchangeName)
	{
		Exchange* exchange = Exchange::Allocate();
		std::memset(exchange, 0, sizeof(Exchange));
		std::strcpy(exchange->ExchangeId, exchangeId);
		std::strcpy(exchange->ExchangeName, exchangeName);
		return exchange;
	}

	std::vector<Exchange*> AllocateExchanges()
	{
		return {
			AllocateExchange("SHFE", "上海期货交易所"),
			AllocateExchange("INE", "上海国际能源交易中心"),
			AllocateExchange("CFFEX", "中国金融期货交易所"),
			AllocateExchange("CZCE", "郑州商品期货交易所"),
			AllocateExchange("DCE", "大连商品期货交易所"),
			AllocateExchange("GFE", "广期所"),
		};
	}

	// 调用方负责连接与断开；取出的记录是库里的副本，用完归还对象池
	template <typename Record>
	std::vector<Record> ReadStoredRows(Db* database)
	{
		TypedTable<Record> table(database);
		std::vector<Record*> selectedRows;
		table.SelectAll(selectedRows);
		std::vector<Record> storedRows;
		storedRows.reserve(selectedRows.size());
		for (Record* selectedRow : selectedRows)
		{
			storedRows.push_back(*selectedRow);
			selectedRow->Deallocate();
		}
		return storedRows;
	}

	bool HasStoredExchangeId(const std::vector<Exchange>& storedExchanges, const char* exchangeId)
	{
		for (const Exchange& storedExchange : storedExchanges)
		{
			if (std::strcmp(storedExchange.ExchangeId, exchangeId) == 0)
			{
				return true;
			}
		}
		return false;
	}

	// 可控失败的 Db 装饰器：让写库线程从自己的 HandleDbOperate 里走到「操作抛异常 -> DisConnect ->
	// 退避 5 秒 -> 重连」这条生产路径。若从测试线程外部调 DisConnect，写库线程可能在句柄关闭之前
	// 把 connected_ 重新置真，此后所有语句都打在已关闭的句柄上，且再也不会重连。
	// 顺带记录 Exchanges 的批量写入行数：重连后的全量快照以它为准，不必另开连接去"看"
	// ——读连接会拿到文件锁，把写库端的批删挡下来。
	class FailureInjectingDb : public Db
	{
	public:
		explicit FailureInjectingDb(std::unique_ptr<Db> inner) : inner_(std::move(inner)) {}

		bool Connect() override { return inner_->Connect(); }
		void DisConnect() override
		{
			disconnectCount.fetch_add(1);
			inner_->DisConnect();
		}
		void Exec(const char* sql) override { inner_->Exec(sql); }
		void CreateTable(const TableSchema* schema) override { inner_->CreateTable(schema); }
		void DropTable(const char* tableName) override { inner_->DropTable(tableName); }
		void TruncateTable(const char* tableName) override { inner_->TruncateTable(tableName); }
		void CreateTables(const TableSchema* const* schemas, int count) override { inner_->CreateTables(schemas, count); }
		void DropTables(const TableSchema* const* schemas, int count) override { inner_->DropTables(schemas, count); }
		void TruncateTables(const TableSchema* const* schemas, int count) override { inner_->TruncateTables(schemas, count); }
		void Insert(const TableSchema* schema, const void* record) override
		{
			ThrowIfRequested();
			inner_->Insert(schema, record);
		}
		void BatchInsert(const TableSchema* schema, const void* const* records, int count) override
		{
			ThrowIfRequested();
			inner_->BatchInsert(schema, records, count);
			if (std::strcmp(schema->tableName, "Exchanges") == 0)
			{
				batchInsertedExchangeCount.fetch_add(count);
			}
		}
		void Update(const TableSchema* schema, const void* record) override { inner_->Update(schema, record); }
		void Delete(const TableSchema* schema, const void* record, const int* keyFieldIndices, int keyFieldCount) override
		{
			inner_->Delete(schema, record, keyFieldIndices, keyFieldCount);
		}
		void SelectAll(const TableSchema* schema, void* recordsList, const DbAdapters::RecordFactory& factory) override
		{
			inner_->SelectAll(schema, recordsList, factory);
		}
		void SelectWithSql(const char* sql, const TableSchema* schema, void* recordsList, const DbAdapters::RecordFactory& factory) override
		{
			inner_->SelectWithSql(sql, schema, recordsList, factory);
		}

		void ThrowOnNextWrite()
		{
			pendingFailure_.fetch_add(1);
		}

		bool WaitForDisconnect(int timeoutMilliseconds)
		{
			return WaitUntil([this] { return disconnectCount.load() >= 1; }, timeoutMilliseconds);
		}

		bool WaitForBatchInsertedExchangeCount(int expectedCount, int timeoutMilliseconds)
		{
			return WaitUntil([this, expectedCount] { return batchInsertedExchangeCount.load() >= expectedCount; },
				timeoutMilliseconds);
		}

		std::atomic<int> disconnectCount{ 0 };
		std::atomic<int> batchInsertedExchangeCount{ 0 };

	private:
		template <typename Condition>
		bool WaitUntil(Condition condition, int timeoutMilliseconds)
		{
			const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMilliseconds);
			while (!condition() && std::chrono::steady_clock::now() < deadline)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			return condition();
		}

		void ThrowIfRequested()
		{
			if (pendingFailure_.load() > 0)
			{
				pendingFailure_.fetch_sub(1);
				throw std::runtime_error("FailureInjectingDb: write fails by request");
			}
		}

		std::unique_ptr<Db> inner_;
		std::atomic<int> pendingFailure_{ 0 };
	};
}

TEST_CASE_TEMPLATE("写入经异步队列落库，另开连接逐行读回", Backend, SqliteBackend, DuckdbBackend)
{
	{
		DatabaseSession session(Backend::Open());
		REQUIRE(session.Start());
		session.Tables().TradingDays->Insert(AllocateTradingDay());
		for (Exchange* exchange : AllocateExchanges())
		{
			session.Tables().Exchanges->Insert(exchange);
		}
		CHECK(session.WaitForPendingZero(PendingDrainTimeout));
		session.Stop();
	}

	std::unique_ptr<Db> verificationDatabase(Backend::Open());
	REQUIRE(verificationDatabase->Connect());
	const std::vector<TradingDay> storedTradingDays = ReadStoredRows<TradingDay>(verificationDatabase.get());
	const std::vector<Exchange> storedExchanges = ReadStoredRows<Exchange>(verificationDatabase.get());
	verificationDatabase->DisConnect();

	CHECK(storedTradingDays.size() == 1);
	CHECK(storedExchanges.size() == 6);
	for (const char* exchangeId : { "SHFE", "INE", "CFFEX", "CZCE", "DCE", "GFE" })
	{
		CHECK(HasStoredExchangeId(storedExchanges, exchangeId));
	}
}

TEST_CASE_TEMPLATE("删除落库，重开连接只剩未删的行", Backend, SqliteBackend, DuckdbBackend)
{
	{
		DatabaseSession session(Backend::Open());
		REQUIRE(session.Start());
		session.Tables().Exchanges->Insert(AllocateExchange("SHFE", "上海期货交易所"));
		session.Tables().Exchanges->Insert(AllocateExchange("INE", "上海国际能源交易中心"));

		::Mdb::ExchangeIdType ineExchangeId = "INE";
		const std::optional<Exchange> ineExchange = session.Tables().Exchanges->PrimaryKey->Select(ineExchangeId);
		REQUIRE(ineExchange.has_value());
		CHECK(session.Tables().Exchanges->Erase(*ineExchange));

		CHECK(session.WaitForPendingZero(PendingDrainTimeout));
		session.Stop();
	}

	std::unique_ptr<Db> verificationDatabase(Backend::Open());
	REQUIRE(verificationDatabase->Connect());
	const std::vector<Exchange> storedExchanges = ReadStoredRows<Exchange>(verificationDatabase.get());
	verificationDatabase->DisConnect();

	REQUIRE(storedExchanges.size() == 1);
	CHECK(std::strcmp(storedExchanges.front().ExchangeId, "SHFE") == 0);
}

TEST_CASE("修改落库，重开连接读到新值（DuckDB 的 UPDATE 绑定参数缺失，暂只跑 SQLite）")
{
	{
		DatabaseSession session(SqliteBackend::Open());
		REQUIRE(session.Start());
		session.Tables().Exchanges->Insert(AllocateExchange("SHFE", "上海期货交易所"));

		::Mdb::ExchangeIdType shfeExchangeId = "SHFE";
		std::optional<Exchange> shfeExchange = session.Tables().Exchanges->PrimaryKey->Select(shfeExchangeId);
		REQUIRE(shfeExchange.has_value());
		std::strcpy(shfeExchange->ExchangeName, "Renamed");
		CHECK(session.Tables().Exchanges->Update(*shfeExchange));

		CHECK(session.WaitForPendingZero(PendingDrainTimeout));
		session.Stop();
	}

	std::unique_ptr<Db> verificationDatabase(SqliteBackend::Open());
	REQUIRE(verificationDatabase->Connect());
	const std::vector<Exchange> storedExchanges = ReadStoredRows<Exchange>(verificationDatabase.get());
	verificationDatabase->DisConnect();

	REQUIRE(storedExchanges.size() == 1);
	CHECK(std::strcmp(storedExchanges.front().ExchangeId, "SHFE") == 0);
	CHECK(std::strcmp(storedExchanges.front().ExchangeName, "Renamed") == 0);
}

TEST_CASE("断连期间的增量就地丢弃，重连后由全量快照补齐")
{
	std::unique_ptr<FailureInjectingDb> failingDatabase =
		std::make_unique<FailureInjectingDb>(std::make_unique<SqliteWrapper>(ReconnectDatabasePath));
	FailureInjectingDb& failureRecorder = *failingDatabase;
	DatabaseSession session(std::move(failingDatabase));
	REQUIRE(session.Start());

	session.Tables().Exchanges->Insert(AllocateExchange("SHFE", "上海期货交易所"));
	CHECK(session.WaitForPendingZero(PendingDrainTimeout));

	failureRecorder.ThrowOnNextWrite();
	session.Tables().Exchanges->Insert(AllocateExchange("INE", "上海国际能源交易中心"));
	CHECK(failureRecorder.WaitForDisconnect(5000));

	session.Tables().Exchanges->Insert(AllocateExchange("CFFEX", "中国金融期货交易所"));
	CHECK(session.Writer().PendingOperateCount() == 0);

	// 全量快照是重连唯一的数据来源：等它把 3 行整批写进库，再看队列排空
	CHECK(failureRecorder.WaitForBatchInsertedExchangeCount(3, static_cast<int>(ResyncTimeout.count())));
	CHECK(session.WaitForPendingZero(PendingDrainTimeout));
	session.Stop();

	SqliteWrapper verificationDatabase(ReconnectDatabasePath);
	REQUIRE(verificationDatabase.Connect());
	const std::vector<Exchange> storedExchanges = ReadStoredRows<Exchange>(&verificationDatabase);
	verificationDatabase.DisConnect();

	CHECK(storedExchanges.size() == 3);
	for (const char* exchangeId : { "SHFE", "INE", "CFFEX" })
	{
		CHECK(HasStoredExchangeId(storedExchanges, exchangeId));
	}
}

TEST_CASE_TEMPLATE("从库加载：InitMdbFromDb 把落库的行灌回内存表", Backend, SqliteBackend, DuckdbBackend)
{
	{
		DatabaseSession session(Backend::Open());
		REQUIRE(session.Start());
		for (Exchange* exchange : AllocateExchanges())
		{
			session.Tables().Exchanges->Insert(exchange);
		}
		CHECK(session.WaitForPendingZero(PendingDrainTimeout));
		session.Stop();
	}

	std::unique_ptr<Db> sourceDatabase(Backend::Open());
	REQUIRE(sourceDatabase->Connect());
	::Mdb::Mdb loadedTables(::Mdb::Full::FullTableList);
	::Mdb::InitMdbFromDb::LoadTables(&loadedTables, sourceDatabase.get(), ::Mdb::Full::FullTableList);
	sourceDatabase->DisConnect();

	int loadedExchangeCount = 0;
	loadedTables.Exchanges->ForEach([&loadedExchangeCount](const Exchange*)
	{
		loadedExchangeCount += 1;
		return true;
	});
	::Mdb::ExchangeIdType shfeExchangeId = "SHFE";
	CHECK(loadedExchangeCount == 6);
	CHECK(loadedTables.Exchanges->PrimaryKey->Select(shfeExchangeId).has_value());
	loadedTables.Exchanges->TruncateTable();
}
