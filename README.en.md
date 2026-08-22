# Mdb
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C++20+-orange.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake3.20+-green.svg)]()

**Mdb** is a C++ **in-memory database reference implementation (Demo)** built on top of **Spark** and **DBAdapters**, demonstrating how to **implement and use an in-memory database**: data is read and written in memory at high speed, changes are broadcast through a subscription interface, and asynchronously synced to persistent databases (SQLite / DuckDB / MySQL / MariaDB) via DBAdapters' `AsyncDBWriter`. The built-in financial tables are **example data only** — the point is the in-memory database architecture and how it is implemented.

Created by [Fireseeker](https://fireseeker.cn/)

## 1. Introduction

Mdb is an **implementation example of an in-memory database**; the focus is on demonstrating *how to implement and use* an in-memory database, not on specific business data. It shows the complete implementation path:

1. **Schema-driven**: table models are defined in `model/*.xml`, and Python scripts (`pump.py` + `pumplist.xml`) auto-generate all table-structure code (record structs, typed table containers, primary-key containers, secondary-index containers) — no manual code;
2. **In-memory-first + asynchronous persistence**: reads/writes operate on memory first; changes are broadcast through a subscription interface (`MdbSubscriber`) and **asynchronously synced to physical databases** (SQLite / DuckDB / MySQL / MariaDB) via DBAdapters' `AsyncDBWriter`, balancing low latency with data reliability;
3. **Retrieval & concurrency**: each table ships with primary-key and secondary-index containers (`Select` / `SelectAll` / `EqualRange`) and supports concurrent reads via `std::shared_mutex`.

Therefore, the 11 built-in financial tables (trading days, accounts, orders, positions, etc.) are **example data only**, used to demonstrate how to describe, create, index, and query different field types (fixed-length strings, enums, dates, integers). To move to your own business, define your own `model/*.xml` and regenerate the code.

Built with C++20, the project uses CMake for cross-platform builds and ships with built-in table creation scripts and integration tests. It serves both as a tutorial for learning in-memory database implementation and as a starting point for building your own in-memory data layer.

## 2. Key Features

The features below illustrate the key problems to solve when implementing an in-memory database; Mdb provides a reference solution for each:

- ✅ **In-Memory + Disk Dual-Write Architecture**: high-speed in-memory reads/writes with asynchronous disk persistence, balancing performance and safety
- ✅ **Multi-Database Compatibility**: built on DBAdapters, natively supports MySQL, MariaDB, SQLite, and DuckDB backends
- ✅ **Unified Access Interface**: a single in-memory API works across all databases — switching storage engines requires no business logic changes
- ✅ **Financial Domain Data Tables**: 11 built-in trading business tables covering trading days, contracts, accounts, capital, orders, positions, and trade executions
- ✅ **Primary Key & Secondary Index Queries**: each table ships with a primary-key container and secondary-index containers, supporting efficient retrieval via `Select` / `SelectAll` / `EqualRange`
- ✅ **Thread Safety**: each table embeds `std::shared_mutex` with `LockShared` / `UnlockShared` for concurrent reads
- ✅ **Asynchronous Writes**: database persistence runs asynchronously, avoiding blocking of core trading paths; auto-reconnects on disconnection
- ✅ **Import & Export**: bulk-load in-memory data from a database (`InitMdbFromDB`) or from CSV (`InitMdbFromCsv`), and dump all tables via `Dump`
- ✅ **Automated Toolchain**: table-model-driven code generation (`MdbStructs` / `MdbTables` / `MdbPrimaryKeys` / `MdbIndexes` are all template-generated)

## 3. Example Data Table Models

The framework ships with 11 financial trading example tables (table structures are defined in `model/Tables/Tables.xml` and auto-generated). **Note**: these tables and their business fields are for demonstration only — they are not the point of this project. They cover a variety of field types (fixed-length strings, enums, dates, integers) to show how an in-memory database describes, creates, indexes, and queries different fields.

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
├── src/Mdb/                       # Source code & public headers (includes generated code)
│   ├── Mdb.h / Mdb.cpp            # In-memory database entry point (Mdb class, owns all typed tables)
│   ├── MdbStructs.h/.cpp          # Record structs + TableSchema for the 11 tables (generated)
│   ├── MdbTables.h/.cpp           # Typed table containers (TradingDayTable / ExchangeTable / ...)
│   ├── MdbTableBase.h             # Table base class (MdbTableBase: Subscribe / InitDB / Truncate / Dump)
│   ├── MdbTableRegistry.h/.cpp    # SchemaRegistry implementation for AsyncDBWriter to resolve table schemas
│   ├── MdbPrimaryKeys.h/.cpp      # Primary-key index containers (Select / SelectAll / uniqueness check)
│   ├── MdbIndexes.h/.cpp          # Secondary-index containers (LowerBound / UpperBound / EqualRange)
│   ├── MdbPrimaryKeyComp.h/.cpp   # Primary-key hash / equality comparators (generated)
│   ├── MdbIndexComp.h/.cpp        # Index comparators (generated)
│   ├── TableList.h                # Table list struct (TableList)
│   ├── InitMdbFromDB.h/.cpp       # Bulk-load in-memory data from a persistent database
│   └── InitMdbFromCsv.h/.cpp      # Bulk-load in-memory data from a CSV directory
├── test/TestMdb/                  # Integration tests (TestDB: full flow across four DBs + async writes)
│   ├── FullTableList.h            # TableList definition for all 11 tables
│   └── TestDB.cpp
├── model/                         # Data table model definitions (Tables.xml / TableNames.xml, etc.)
│   ├── TableNames/                # Table name lists (FullTableNames.xml)
│   └── Tables/                    # Table structure definitions (Tables.xml / ShortTables.xml / TableNames.xml)
├── sql/                           # Per-database table creation scripts
│   ├── Duckdb/                    # CreateDBTables.sql / DropDBTables.sql
│   ├── Mysql/                     # CreateDBTables.sql / DropDBTables.sql
│   └── Sqlite/                    # CreateDBTables.sql / DropDBTables.sql
├── submodules/                    # Dependent submodule (CMakeCommon)
├── bin/                           # Build outputs: executables / shared libraries (per config)
├── lib/                           # Build outputs: static libraries / import libraries (per config)
├── out/                           # CMake Presets build directory
├── *.py                           # Python automation scripts
├── pumplist.xml                   # Code generation manifest (drives pump.py)
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
| **DBAdapters** database access layer | `../Libs/DBAdapters/<triplet>` | `DBInterface` / `AsyncDBWriter` / four database wrappers |
| **DuckDB** | `../Libs/duckdb/<triplet>` | `duckdb::duckdb` (DuckDB backend runtime) |

`<triplet>` is `x64-windows` on Windows and `x64-linux` on Linux / WSL. See each repository's README for how to build and install Spark and DBAdapters.

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
# Make sure Spark, DBAdapters, and duckdb are installed into ../Libs/ (see "Prebuilt Dependencies" above)
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

After building, the `MdbStatic` static library is output to `lib/<Config>` and the `TestDB` test program to `bin/<Config>` (e.g. `bin/Release/TestDB.exe`). On Windows, vcpkg's applocal mechanism copies runtime DLLs such as `sqlite3.dll`, `mysqlcppconnx-*.dll`, `mariadbcpp.dll`, and `duckdb.dll` next to the executable automatically.

### 6.5 Run Integration Tests

```bash
# Windows
./bin/Release/TestDB.exe

# Linux
./bin/Release/TestDB
```

The test program runs the "in-memory DB + async write" full flow against SQLite / DuckDB / MySQL in sequence (MariaDB is commented out by default and needs a local service), verifying in-memory reads/writes, async persistence, and primary-key / index queries.

## 7. Usage Examples

### Example 1: In-Memory DB + Async Persistence (Full Wiring)

```cpp
#include "Mdb.h"
#include "MdbTableRegistry.h"
#include <DBAdapters/AsyncDBWriter/AsyncDBWriter.h>
#include <DBAdapters/SqliteWrapper/SqliteWrapper.h>
#include <Spark/Core/Core.h>
#include <cstring>

using namespace mdb;
using namespace dbadapters;
using namespace spark::core;

// Define the financial table list contained in the in-memory DB (all 11 tables)
inline const unsigned int kFullTableIDs[] = {
    TradingDay::TableID,      Exchange::TableID,       Product::TableID,
    Instrument::TableID,      PrimaryAccount::TableID, Account::TableID,
    Capital::TableID,         Position::TableID,       PositionDetail::TableID,
    Order::TableID,           Trade::TableID,
};
const TableList FullTableList = { "Full", kFullTableIDs, 11 };

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
    AsyncDBWriter* dbWriter = new AsyncDBWriter(db, schemaRegistry);

    // 3. Bidirectional wiring
    //    mdb->Subscribe(dbWriter)  : in-memory changes broadcast to AsyncDBWriter for async writes
    //    dbWriter->Subscribe(mdb)  : on connect, Mdb::OnDBConnected is called, auto InitDB (create + init)
    mdb->Subscribe(dbWriter);
    dbWriter->Subscribe(mdb);
    dbWriter->Connect();                                    // synchronous connect, triggers table creation
    dbWriter->Start();                                      // start background consumer thread

    // 4. Write to memory; persisted asynchronously
    Exchange* exchange = new Exchange();
    std::memset(exchange, 0, sizeof(Exchange));
    std::strcpy(exchange->ExchangeID, "SHFE");
    std::strcpy(exchange->ExchangeName, "Shanghai Futures Exchange");
    mdb->t_Exchange->Insert(exchange);                      // returns false on primary-key conflict

    Account* account = new Account();
    std::memset(account, 0, sizeof(Account));
    std::strcpy(account->AccountID, "A001");
    std::strcpy(account->AccountName, "Alice");
    account->AccountType = AccountTypeType::Primary;
    account->AccountStatus = AccountStatusType::Normal;
    mdb->t_Account->Insert(account);

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
// Primary-key lookup: locate a single record by unique primary key
ExchangeIDType exchangeID("CFFEX");
Exchange* exchange = mdb->t_Exchange->m_PrimaryKey->Select(exchangeID);
WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());

// Full scan over primary key: returns an iterator pair [begin, end)
auto all = mdb->t_Exchange->m_PrimaryKey->SelectAll();
for (auto it = all.first; it != all.second; ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// Secondary-index equality range query (e.g. primary accounts by OfferID)
OfferIDType offerID = 10001;
auto range = mdb->t_PrimaryAccount->m_OfferIDIndex->EqualRange(offerID);
for (auto it = range.first; it != range.second; ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// Batch delete by secondary index (e.g. clear capital records for a trading day)
mdb->t_Capital->EraseByTradingDayIndex(tradingDay->PreTradingDay);
```

### Example 3: Update / Delete / Clear Tables

```cpp
// Update: sync to memory and broadcast to AsyncDBWriter via MdbSubscriber for async persistence
AccountIDType accountID;
std::strcpy(accountID, "A001");
Account* oldAccount = mdb->t_Account->m_PrimaryKey->Select(accountID);
Account* newAccount = new Account();
*newAccount = *oldAccount;                              // copy then modify
std::strcpy(newAccount->AccountName, "Bob");
mdb->t_Account->Update(oldAccount, newAccount);

// Delete a single record (removes from memory and broadcasts the delete for persistence)
mdb->t_Account->Erase(oldAccount);

// Clear a single table
mdb->t_Account->TruncateTable();

// Clear all tables (broadcast TruncateTables + locally clear each table)
mdb->TruncateTables();
```

### Example 4: Load from DB / CSV and Dump

```cpp
#include "InitMdbFromDB.h"
#include "InitMdbFromCsv.h"

// Bulk-load from a persistent database into memory (after tables are created)
InitMdbFromDB::LoadTables(mdb, db, FullTableList);

// Bulk-load from a CSV directory into memory
InitMdbFromCsv::LoadTables(mdb, "./dump", FullTableList);

// Dump all in-memory tables to a directory
mdb->Dump("./dump");
```

## 8. Python Scripts

Table models are defined in `model/*.xml`; the Python scripts in the project root handle code generation and automation:

| Script | Description |
| ---- | ---- |
| pump.py / pumpall.py / pumptemp.py | Template code generation engine (driven by pumplist.xml), generates Mdb table-structure code from `model/*.xml` |
| pumplist.xml | Code generation manifest: template → target file mapping |
| ParseTableModel.py | Data table model parsing |
| ParsePackageModel.py | Network packet model parsing |
| ParseShortField.py / ParseShortItem.py | Short field / short item parsing |
| geninc.py | Header include generation |
| ConvertToUtf8Bom.py | Convert file encoding to UTF-8 BOM |
| parseall.py | Batch parsing entry point |
| clearall.py | Clean temporary files and caches |

## 9. Test Programs

The `test/TestMdb/TestDB.cpp` integration test covers:

| Test Item | Description |
| ---- | ---- |
| `TestSqlite` | SQLite backend: full in-memory + async write flow |
| `TestDuckdb` | DuckDB backend: full in-memory + async write flow |
| `TestMysql` | MySQL backend full flow (requires local MySQL X Plugin; commented out by default) |
| `TestMariadb` | MariaDB backend full flow (requires local MariaDB; commented out by default) |
| Primary key / index | `m_PrimaryKey->Select` / `SelectAll`, `m_OfferIDIndex->EqualRange`, etc. |
| In-memory ops | Insert / Update / Erase / TruncateTable with broadcast persistence verification |

## 10. License & Disclaimer

- **License**: This project is open-sourced under the [MIT License](LICENSE). Free to use, modify, and redistribute.
- **Use Cases**: An **implementation example** and tutorial for an in-memory database — reference its architecture to build your own in-memory data layer; the built-in financial tables can be replaced with any business tables.
- **Disclaimer**: This is a personal open-source project. It is recommended to conduct thorough stress testing and functional validation in a staging environment before evaluating production readiness. For high-frequency core trading scenarios, perform your own performance tuning and disaster recovery adaptations.

## 11. Additional Notes

- **Project Positioning**: this project is an **implementation example** of an in-memory database — the focus is on "how to implement and use"; the built-in financial tables are for demonstration only, and new businesses can define their own tables via `model/*.xml` + code generation
- **Dependencies**: Mdb is built on [Spark](https://gitee.com/xunmeng2002/Spark.git) (foundational capabilities) and [DBAdapters](https://gitee.com/xunmeng2002/DBAdapters.git) (unified database access layer); both are prebuilt dependencies
- **Include Paths**: Mdb headers live in `src/Mdb/`, referenced as `#include "Mdb.h"`
- **Namespaces**: all interfaces are in the `mdb` namespace; database interfaces reuse the `dbadapters` namespace
- **Concurrency**: in-memory tables are thread-safe by design; for reads, consider calling `LockShared()` before accessing primary-key / index containers to avoid contention with writer threads
- **Code Generation**: after editing `model/*.xml`, run `python pumpall.py` to regenerate table-structure code without manual edits
