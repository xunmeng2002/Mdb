# Mdb
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C++20+-orange.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake3.20+-green.svg)]()

**Mdb** is a C++ **in-memory database reference implementation (Demo)** built on top of **Spark** and **DbAdapters**, demonstrating how to **implement and use an in-memory database**: data is read and written in memory at high speed, changes are broadcast through a subscription interface, and asynchronously synced to persistent databases (SQLite / DuckDB / MySQL / MariaDB) via DbAdapters' `AsyncDbWriter`. The built-in financial tables are **example data only** — the point is the in-memory database architecture and how it is implemented.

Created by [Fireseeker](https://fireseeker.cn/)

## 1. Introduction

Mdb is an **implementation example of an in-memory database**; the focus is on demonstrating *how to implement and use* an in-memory database, not on specific business data. It shows the complete implementation path:

1. **Schema-driven**: table models are defined in `Model/*.xml`, and Python scripts (`pump.py` + `pumplist.xml`) auto-generate all table-structure code (record structs, typed table containers, primary-key containers, secondary-index containers) — no manual code;
2. **In-memory-first + asynchronous persistence**: reads/writes operate on memory first; changes are broadcast through a subscription interface (`MdbSubscriber`) and **asynchronously synced to physical databases** (SQLite / DuckDB / MySQL / MariaDB) via DbAdapters' `AsyncDbWriter`, balancing low latency with data reliability;
3. **Retrieval & concurrency**: each table ships with primary-key and secondary-index containers (`Select` / `SelectAll` / `EqualRange` / `Range`). A single-record `Select` returns a **record copy** (`std::optional`): the copy is taken under the shared lock and the lock is already released on return, so the copy has no further connection to the live record. Range queries return a **range lease** (`TableRangeLease`): the lock travels with the result, so writers stay blocked while it is alive and iterators/record contents stay stable during traversal.

Therefore, the 11 built-in financial tables (trading days, accounts, orders, positions, etc.) are **example data only**, used to demonstrate how to describe, create, index, and query different field types (fixed-length strings, enums, dates, integers). To move to your own business, define your own `Model/*.xml` and regenerate the code.

Built with C++20, the project uses CMake for cross-platform builds and ships with built-in table creation scripts and integration tests. It serves both as a tutorial for learning in-memory database implementation and as a starting point for building your own in-memory data layer.

## 2. Key Features

The features below illustrate the key problems to solve when implementing an in-memory database; Mdb provides a reference solution for each:

- ✅ **In-Memory + Disk Dual-Write Architecture**: high-speed in-memory reads/writes with asynchronous disk persistence, balancing performance and safety
- ✅ **Multi-Database Compatibility**: built on DbAdapters, natively supports MySQL, MariaDB, SQLite, and DuckDB backends
- ✅ **Unified Access Interface**: a single in-memory API works across all databases — switching storage engines requires no business logic changes
- ✅ **Financial Domain Data Tables**: 11 built-in trading business tables covering trading days, contracts, accounts, capital, orders, positions, and trade executions
- ✅ **Primary Key & Secondary Index Queries**: each table ships with a primary-key container and secondary-index containers, supporting efficient retrieval via `Select` / `SelectAll` / `EqualRange` / `Range`
- ✅ **Thread Safety**: each table embeds `std::shared_mutex`; single-record reads copy under the shared lock and release it on return, range reads return an RAII range lease, and writes take the exclusive lock
- ✅ **Asynchronous Writes**: database persistence runs asynchronously, avoiding blocking of core trading paths; auto-reconnects on disconnection
- ✅ **Import & Export**: bulk-load in-memory data from a database (`InitMdbFromDb`) or from CSV (`InitMdbFromCsv`), and dump all tables via `Dump`
- ✅ **Automated Toolchain**: table-model-driven code generation (`MdbStructs` / `MdbTables` / `MdbPrimaryKeys` / `MdbIndexes` are all template-generated)

## 3. Example Data Table Models

The framework ships with 11 financial trading example tables (table structures are defined in `Model/Tables/Tables.xml` and auto-generated). **Note**: these tables and their business fields are for demonstration only — they are not the point of this project. They cover a variety of field types (fixed-length strings, enums, dates, integers) to show how an in-memory database describes, creates, indexes, and queries different fields.

| Table Name | Table ID | Description | Business Use |
| ---- | ---- | ---- | ---- |
| TradingDay | 0x0001 | Trading Day Table | Records market trading days |
| Exchange | 0x0002 | Exchange Table | Stores information for exchanges (SSE, SZSE, futures exchanges, etc.) |
| Product | 0x0003 | Product Table | Financial product categories (stocks, futures, options, etc.) |
| Instrument | 0x0004 | Contract / Security Table | Specific instrument codes, names, trading parameters, etc. |
| PrimaryAccount | 0x0005 | Primary Account Table | Top-level capital account information |
| Account | 0x0006 | Sub-Account Table | Trading sub-accounts and tenant account management |
| Capital | 0x0007 | Capital Table | Available funds, frozen funds, P&L data, etc. |
| Position | 0x0008 | Position Table | Summary data of total position quantities, average prices, etc. |
| PositionDetail | 0x0009 | Position Detail Table | Per-trade position detail records |
| Order | 0x000A | Order Table | Full client order information |
| Trade | 0x000B | Trade Execution Table | Completed trade execution details |

## 4. Project Directory Structure

```
Mdb/
├── include/
│   └── Mdb/
│       └── Types.h                # Public types (fixed-length strings, dates, enums)
├── src/Mdb/                       # Sources for all three implementations (generated from ../Templates by pumpall.py)
│   ├── Common/                    # Shared data contract (MdbCommonStatic)
│   │   ├── MdbStructs.h/.cpp      # Record structs + TableSchema for the 11 tables
│   │   ├── MdbTableBase.h         # Table base class (Subscribe / InitDB / Truncate / Dump)
│   │   ├── MdbReadLease.h         # Range lease (TableRangeLease: lock travels with the result)
│   │   ├── MdbTableRegistry.h/.cpp  # SchemaRegistry implementation for AsyncDbWriter
│   │   ├── MdbPrimaryKeyComp.h/.cpp # Primary-key hash / equality comparators
│   │   ├── MdbIndexComp.h/.cpp    # Index comparators
│   │   └── TableList.h            # Table list struct (TableList)
│   ├── LockBased/                 # shared_mutex implementation (MdbLockBasedStatic, namespace Mdb)
│   │   ├── Mdb.h/.cpp             # Database entry point (owns all typed tables)
│   │   ├── MdbTables.h/.cpp       # Typed table containers (TradingDayTable / ExchangeTable / ...)
│   │   ├── MdbPrimaryKeys.h/.cpp  # Primary-key containers (Select / SelectAll / uniqueness check)
│   │   ├── MdbIndexes.h/.cpp      # Secondary-index containers (Range / EqualRange)
│   │   ├── InitMdbFromDb.h/.cpp   # Bulk-load in-memory data from a persistent database
│   │   └── InitMdbFromCsv.h/.cpp  # Bulk-load in-memory data from a CSV directory
│   └── Rcu/
│       ├── RcuEpoch.h/.cpp        # Epoch-based reclamation shared by both RCU forms (MdbRcuCoreStatic)
│       ├── PointerSwap/           # Implementation B: primary keys only (MdbRcuPointerSwapStatic)
│       │   ├── Mdb.h/.cpp
│       │   ├── PointerSwapTables.h/.cpp
│       │   ├── PointerSwapPrimaryKeys.h/.cpp
│       │   ├── InitMdbFromDb.h/.cpp
│       │   └── InitMdbFromCsv.h/.cpp
│       └── StableRow/             # Implementation B2: with indexes / unique keys (MdbRcuStableRowStatic)
│           ├── Mdb.h/.cpp
│           ├── StableRowTables.h/.cpp
│           ├── StableRowPrimaryKeys.h/.cpp
│           ├── StableRowIndexes.h/.cpp
│           ├── InitMdbFromDb.h/.cpp
│           └── InitMdbFromCsv.h/.cpp
├── test/
│   ├── UnitTests/                 # Unit tests (no physical DB): TableTests.cpp / ContractTests.cpp / Main.cpp
│   ├── TestMdb/                   # Integration tests (physical DB: async writes, reconnect resync, load-back)
│   │   ├── DatabaseSession.h/.cpp # RAII combination of physical DB + in-memory tables + writer thread
│   │   ├── FullTableList.h        # TableList definition for all 11 tables
│   │   ├── Main.cpp               # doctest entry point (initializes the Logger)
│   │   └── TestMdb.cpp            # Integration cases
│   ├── TestConcurrency/           # Read-scaling comparison (2 / 4 / 8 readers × 1 writer)
│   │   ├── OperationInterval.h    # Command-line operation interval
│   │   ├── TestConcurrency.cpp    # Comparison cases
│   │   └── Main.cpp               # doctest entry point
│   ├── TestSupport/               # Shared test helpers
│   │   ├── TestSupport.h          # Record filling, fill-table-by-primary-key
│   │   └── ImplementationNames.h  # Display names for the three implementations
│   └── Benchmarks/                # Per-operation cost benchmark (BenchTableOps)
│       ├── CMakeLists.txt
│       └── BenchTableOps/BenchTableOps.cpp
├── Model/                         # Table models (input to code generation)
│   ├── Types.xml                  # Field type definitions
│   ├── ShortItems.xml             # Concise item list (hand-maintained)
│   ├── Items.xml                  # Item registry with explicit ids (generated by ParseShortItem.py)
│   ├── parselist.xml              # Model parsing manifest (drives parseall.py)
│   ├── TableNames/
│   │   └── FullTableNames.xml     # Table name list (generates FullTableList.h)
│   └── Tables/
│       ├── ShortTables.xml        # Concise table definitions (hand-maintained)
│       ├── Tables.xml             # Full table definitions (generated by parsing)
│       └── TableNames.xml         # Table names and descriptions (generated by parsing)
├── Sql/                           # Per-database table scripts (generated from Sql/pumplist.xml)
│   ├── pumplist.xml               # SQL generation manifest
│   ├── Duckdb/                    # CreateDBTables.sql / DropDBTables.sql
│   ├── Mysql/                     # CreateDBTables.sql / DropDBTables.sql
│   └── Sqlite/                    # CreateDBTables.sql / DropDBTables.sql
├── docs/                          # Environment setup and RCU design docs
├── submodules/                    # Dependent submodule (CMakeCommon)
├── bin/                           # Build outputs: executables / shared libraries (per config)
├── lib/                           # Build outputs: static libraries / import libraries (per config)
├── out/                           # CMake Presets build directory
├── *.py                           # Parsing / code-generation scripts (see section 8)
├── pumplist.xml                   # C++ code generation manifest (drives pumpall.py)
├── CMakeLists.txt                 # CMake main build configuration
├── CMakePresets.json              # CMake preset configuration (VS / command line)
├── vcpkg.json                     # vcpkg manifest (third-party drivers)
├── UpdateSubmodule.bat/sh         # Submodule update scripts
├── .gitmodules                    # Git submodule configuration
├── .gitignore                     # Git ignore rules
└── LICENSE                        # MIT open-source license
```

## 5. Environment & Dependencies

### 5.1 Prerequisites

| Dependency | Version | Notes |
| ---- | ---- | ---- |
| C++ Compiler | C++20 or later | GCC, Clang, MSVC all supported |
| CMake | 3.20 or later | Presets require 3.21+ |
| vcpkg | Latest | `VCPKG_ROOT` env var required for CMake to locate the toolchain |
| Python | 3.6+ | Code generation scripts only, not required at runtime |

### 5.2 Prebuilt Dependencies (prepared manually)

The build depends on three **prebuilt third-party libraries**, which must be installed under the `Libs/` directory of the project's parent (matching the `CMakePresets.json` layout):

| Dependency | Install Location | Provides |
| ---- | ---- | ---- |
| **Spark** foundational library | `../Libs/Spark/<triplet>` | `Spark::Core` (threads, logging), `Spark/Types.h` type definitions |
| **DbAdapters** database access layer | `../Libs/DbAdapters/<triplet>` | `DbInterface` / `AsyncDbWriter` / four database wrappers |
| **DuckDB** | `../Libs/duckdb/<triplet>` | `duckdb::duckdb` (DuckDB backend runtime) |

`<triplet>` is `x64-windows` on Windows and `x64-linux` on Linux / WSL. See each repository's README for how to build and install Spark and DbAdapters.

### 5.3 vcpkg Dependencies

Dependencies declared in `vcpkg.json` (resolved automatically at build time, including transitive deps such as protobuf / lz4 / zstd / zlib):

- `sqlite3` — SQLite driver
- `mysql-connector-cpp` — MySQL X DevAPI driver
- `mariadb-connector-cpp` — MariaDB driver

### 5.4 Submodule Dependency

- **CMakeCommon**: common CMake macro collection; pull the submodule after cloning.

> For detailed environment setup steps (proxy configuration, vcpkg installation, WSL mirrored networking, etc.), see the [Environment Setup Guide](docs/environment-setup.en.md).

## 6. Quick Build & Compilation

### 6.1 Clone the Repository (including submodules)

```bash
git clone --recursive https://gitee.com/xunmeng2002/Mdb.git
cd Mdb
```

### 6.2 Update Submodules (if not cloned recursively)

```bash
# Linux / macOS
sh UpdateSubmodule.sh

# Windows
UpdateSubmodule.bat
```

### 6.3 Prepare Dependencies

```bash
# Make sure VCPKG_ROOT is configured (system env var on Windows, ~/.bashrc on Linux)
# Make sure Spark, DbAdapters, and duckdb are installed into ../Libs/ (see "Prebuilt Dependencies" above)
```

### 6.4 CMake Build (Presets recommended)

```bash
# Windows (MSVC, Ninja)
cmake --preset x64-Release
cmake --build out/build/x64-Release

# Linux / WSL (GCC)
cmake --preset WSL-GCC-Release
cmake --build out/build/WSL-GCC-Release
```

After building, the five static libraries (`MdbCommonStatic` / `MdbLockBasedStatic` / `MdbRcuCoreStatic` / `MdbRcuPointerSwapStatic` / `MdbRcuStableRowStatic`) are output to `lib/<Config>`, and the test and benchmark programs `UnitTests`, `TestMdb`, `TestConcurrency`, and `BenchTableOps` to `bin/<Config>` (e.g. `bin/Release/UnitTests.exe`). Runtime DLLs required by the executables (Spark / DbAdapters / DuckDB / SQLite / MySQL / MariaDB) are copied next to them by `copy_dll_file` after linking.

### 6.5 Run Tests

```bash
# Unit tests: table-level direct tests and aggregate contracts on all three implementations, no external dependency
./bin/Release/UnitTests.exe

# Integration tests: async writes, reconnect resync, and load-back on SQLite / DuckDB files
./bin/Release/TestMdb.exe

# Read-scaling comparison: 2 / 4 / 8 readers × 1 writer; pass the operation interval in microseconds
./bin/Release/TestConcurrency.exe 10
```

These targets need no MySQL / MariaDB service by default; once the environment variables are set, `TestMdb` also runs the server cases (see section 9).

## 7. Usage Examples

### Example 1: In-Memory DB + Async Persistence (Full Wiring)

```cpp
#include "LockBased/Mdb.h"
#include "Common/MdbTableRegistry.h"
#include "FullTableList.h"
#include <DbAdapters/AsyncDbWriter/AsyncDbWriter.h>
#include <DbAdapters/SqliteWrapper/SqliteWrapper.h>
#include <Spark/Core/Core.h>
#include <cstring>

using namespace Mdb::Full;
using namespace DbAdapters;
using namespace spark::core;

// The in-memory DB's table list (all 11 tables) is not hand-written: it is generated
// from Model/TableNames/FullTableNames.xml into FullTableList.h by pumpall.py — do not edit

int main(int argc, const char* argv[])
{
    Logger::GetInstance().Init(argv[0]);
    Logger::GetInstance().SetLogLevel(LogLevel::Info, LogLevel::Info);
    Logger::GetInstance().Start();

    // 1. In-memory DB: create 11 typed tables from the table list
    Mdb* mdb = new Mdb(FullTableList);

    // 2. Persistence chain: DB adapter + schema registry + async writer
    DB* db = new SqliteWrapper("./Test.sqlitedb");          // persist to SQLite
    MdbTableRegistry* schemaRegistry = new MdbTableRegistry(FullTableList);
    AsyncDbWriter* dbWriter = new AsyncDbWriter(db, schemaRegistry);

    // 3. Bidirectional wiring
    //    mdb->SubscribeReplica(dbWriter): row changes and full snapshots both broadcast (one replica subscriber per DB)
    //    dbWriter->Subscribe(mdb)       : on connect, Mdb::OnDbConnected is called, auto InitDB (create + init all tables)
    mdb->SubscribeReplica(dbWriter);
    dbWriter->Subscribe(mdb);
    dbWriter->Connect();                                    // synchronous connect, triggers table creation
    dbWriter->Start();                                      // start background consumer thread

    // 4. Write to memory; persisted asynchronously
    Exchange* exchange = Exchange::Allocate();
    std::memset(exchange, 0, sizeof(Exchange));
    std::strcpy(exchange->ExchangeId, "SHFE");
    std::strcpy(exchange->ExchangeName, "Shanghai Futures Exchange");
    mdb->Exchange->Insert(exchange);                      // returns false on primary-key conflict

    Account* account = Account::Allocate();
    std::memset(account, 0, sizeof(Account));
    std::strcpy(account->AccountId, "A001");
    std::strcpy(account->AccountName, "Alice");
    account->AccountType = AccountTypeType::Primary;
    account->AccountStatus = AccountStatusType::Normal;
    mdb->Account->Insert(account);

    // 5. Cleanup: stop the writer thread, unsubscribe
    dbWriter->Stop();
    dbWriter->Join();
    mdb->UnSubscribe();
    delete mdb;
    delete dbWriter;
    delete schemaRegistry;

    Logger::GetInstance().Stop();
    Logger::GetInstance().Join();
    return 0;
}
```

### Example 2: Primary Key & Secondary Index Queries

```cpp
// Primary-key lookup: returns a record copy, detached from the live record; the shared lock is already released
ExchangeIdType exchangeId = "CFFEX";
std::optional<Exchange> exchange = mdb->Exchange->PrimaryKey->Select(exchangeId);
if (exchange.has_value())
{
    WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());
}

// Full scan over primary key: the range lease keeps begin/end and record contents stable
auto all = mdb->Exchange->PrimaryKey->SelectAll();
for (auto it = all.begin(); it != all.end(); ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// Secondary-index equality range query (e.g. primary accounts by OfferId)
OfferIdType offerId = 10001;
auto range = mdb->PrimaryAccount->OfferIdIndex->EqualRange(offerId);
for (auto it = range.begin(); it != range.end(); ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// Secondary-index bounded range query: both bounds are taken under one lock
DateType startTradingDay = "20261001";
DateType endTradingDay = "20261031";
auto monthRange = mdb->Capital->TradingDayIndex->Range(startTradingDay, endTradingDay);

// Batch delete by secondary index (e.g. clear capital records for a trading day)
mdb->Capital->EraseByTradingDayIndex(startTradingDay);
```

### Example 3: Update / Delete / Clear Tables

```cpp
// Update: Select already hands back a copy; modify it and hand it back. Update locates the live record by primary key and overwrites it whole
AccountIdType accountId = "A001";
std::optional<Account> account = mdb->Account->PrimaryKey->Select(accountId);
if (!account.has_value())
{
    return;
}
std::strcpy(account->AccountName, "Bob");
mdb->Account->Update(*account);                          // false means the primary key is no longer in the table

// Delete a single record: also located by primary key; the copy only supplies the key, the live record is what gets erased
mdb->Account->Erase(*account);

// Clear a single table
mdb->Account->TruncateTable();

// Clear all tables (broadcast TruncateTables + locally clear each table)
mdb->TruncateTables();
```

### Example 4: Load from DB / CSV and Dump

```cpp
#include "LockBased/InitMdbFromDb.h"
#include "LockBased/InitMdbFromCsv.h"

// Bulk-load from a persistent database into memory (after tables are created)
InitMdbFromDb::LoadTables(mdb, db, FullTableList);

// Bulk-load from a CSV directory into memory
InitMdbFromCsv::LoadTables(mdb, "./dump", FullTableList);

// Dump all in-memory tables to a directory
mdb->Dump("./dump");
```

## 8. Python Scripts

Table models are defined in `Model/*.xml`; the Python scripts in the project root handle code generation and automation:

| Script | Description |
| ---- | ---- |
| pump.py / pumpall.py / pumptemp.py | Template code generation engine (driven by pumplist.xml), generates Mdb table-structure code from `Model/*.xml` |
| pumplist.xml | Code generation manifest: template → target file mapping |
| ParseTableModel.py | Data table model parsing |
| ParsePackageModel.py | Network packet model parsing |
| ParseShortField.py / ParseShortItem.py | Short field / short item parsing |
| geninc.py | Header include generation |
| ConvertToUtf8Bom.py | Convert file encoding to UTF-8 BOM |
| parseall.py | Batch parsing entry point |
| clearall.py | Clean temporary files and caches |

## 9. Test Programs

Three targets, split by dependency; all use doctest, support `--test-case=` filtering, and exit
with 0 when everything passes:

| Target | Coverage |
| ---- | ---- |
| `UnitTests` | No external dependency. Table-level direct tests on all three implementations (single-row interface, batch insert and read lease, subscribers, tear-free concurrent read, indexes and unique keys, truncate, CSV export), plus aggregate-layer contracts (subscriber fan-out, snapshot under read lease, publish barrier, write validation, replica semantics) |
| `TestMdb` | Physical database (SQLite / DuckDB files by default; MySQL / MariaDB optional, see below). Async write-through, update and delete persistence, incremental loss while disconnected and full resync on reconnect, loading back via `InitMdbFromDb` |
| `TestConcurrency` | Read-scaling comparison of the three implementations (2 / 4 / 8 readers × 1 writer); prints torn-read and missing-row counts for same-machine comparison only |

### Optional Server Backends (MySQL / MariaDB)

`TestMdb` runs SQLite / DuckDB files by default; the MySQL and MariaDB cases run only when the
variables below are set. Otherwise they print a `[skip] ...` line and return (doctest counts them
as passed):

| Environment Variable | Meaning |
| ---- | ---- |
| `MDB_TEST_ALLOW_DESTRUCTIVE=1` | Required; without it all server cases are skipped |
| `MDB_TEST_MYSQL_URL` | MySQL X DevAPI URL, e.g. `mysqlx://user:pass@host:33060/mdb_test` |
| `MDB_TEST_MARIADB_URL` | MariaDB URL, e.g. `tcp://host:3306/mdb_test` |
| `MDB_TEST_MARIADB_USER` / `MDB_TEST_MARIADB_PASSWORD` | MariaDB credentials |

> **Warning**: at startup the tests truncate all 11 tables in the target database before loading
> the snapshot (`InitDb` behavior). Always point them at a dedicated test database.

```bash
# Linux / WSL
MDB_TEST_ALLOW_DESTRUCTIVE=1 MDB_TEST_MYSQL_URL="mysqlx://user:pass@localhost:33060/mdb_test" ./bin/Debug/TestMdb
```

On Windows set `$env:MDB_TEST_ALLOW_DESTRUCTIVE=1` and `$env:MDB_TEST_MYSQL_URL="..."` first, then
run `TestMdb.exe`. Server adapters and their client DLLs are copied into `bin/<Config>` by
`copy_dll_file` during the build, so no manual deployment is needed.

The `BenchTableOps` benchmark (`test/Benchmarks`, `BUILD_BENCHMARKS` defaults to ON) makes no
assertions and is not gated in CI: on a 100k-row trading-day table it measures the per-call cost
of `Select` / `Update` / `InsertErase` for all three implementations, printed as CSV by Spark's
`Benchmark::Measure`. **Use it for absolute costs; read scaling and torn reads still come from
`TestConcurrency`.**

```text
./bin/Release/BenchTableOps
# name,ns_per_call,ns_slowest,calls_per_round
LockBased.Select,155.0,162.9,202511
LockBased.Update,334.8,370.1,131891
LockBased.InsertErase,550.0,658.4,127844
PointerSwap.Select,79.5,104.1,759878
PointerSwap.Update,305.8,344.4,186776
PointerSwap.InsertErase,235.3,293.7,267522
StableRow.Select,78.3,133.1,857632
StableRow.Update,131.7,188.3,554323
StableRow.InsertErase,261.6,270.6,148676
```

> **Note**: measured locally on 2026-10-29 with a Release build; numbers vary with machine and
> scheduling and serve only for same-machine comparison across commits. They are meaningful only in
> Release — a Debug run prints a warning to stderr first. `InsertErase` is a pair (insert, then
> erase the same key so the row count stays constant); it measures the round trip of that pair.

**The batch-insert contract assertion has no automated test**: by definition an assertion
terminates the process, so it cannot both fire and continue in the same process; a child-process
death test, in turn, is treated by Windows as a crash and pops up the "has stopped working"
dialog, so that mechanism was removed entirely. To watch it fire by hand, temporarily make every
record in `UnitTests`' batch-insert case share one primary key (pass `20000` as the second argument
of `FillTradingDay(*row, 20000 + index, index)`); a Debug build then prints
`Assertion failed: insertedIntoPrimaryKey, ...` and terminates, while the same input runs clean in
Release.

## 10. License & Disclaimer

- **License**: This project is open-sourced under the [MIT License](LICENSE). Free to use, modify, and redistribute.
- **Use Cases**: An **implementation example** and tutorial for an in-memory database — reference its architecture to build your own in-memory data layer; the built-in financial tables can be replaced with any business tables.
- **Disclaimer**: This is a personal open-source project. It is recommended to conduct thorough stress testing and functional validation in a staging environment before evaluating production readiness. For high-frequency core trading scenarios, perform your own performance tuning and disaster recovery adaptations.

## 11. Additional Notes

- **Project Positioning**: this project is an **implementation example** of an in-memory database — the focus is on "how to implement and use"; the built-in financial tables are for demonstration only, and new businesses can define their own tables via `Model/*.xml` + code generation
- **Dependencies**: Mdb is built on [Spark](https://gitee.com/xunmeng2002/Spark.git) (foundational capabilities) and [DbAdapters](https://gitee.com/xunmeng2002/DbAdapters.git) (unified database access layer); both are prebuilt dependencies
- **Include Paths**: public type definitions live in `include/Mdb/Types.h`; implementations and remaining headers live in `src/Mdb/{Common,LockBased,Rcu}/`, referenced as `#include "LockBased/Mdb.h"`
- **Namespaces**: the lock-based implementation lives in the `Mdb` namespace, the two RCU implementations in `Mdb::Rcu::PointerSwap` / `Mdb::Rcu::StableRow`; database interfaces reuse the `DbAdapters` namespace
- **Concurrency**: in-memory tables are thread-safe by design; a single-record `Select` returns a copy you can use freely — it holds no lock and does not block writers. `SelectAll` / `Range` / `EqualRange` return a range lease holding the shared lock: **never write to the same table while it is alive** (Insert / Update / Erase / TruncateTable) — that self-deadlocks. Subscriber callbacks run inside the write transaction's publication barrier: they may read the table, but **must not write the same table** (that also self-deadlocks). Copy what you need out of the loop body and write after releasing the lease
- **Code Generation**: after editing `Model/*.xml`, run `python pumpall.py` to regenerate table-structure code without manual edits
