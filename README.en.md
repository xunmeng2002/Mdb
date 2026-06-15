# Mdb
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C++20+-orange.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake3.15+-green.svg)]()

**Mdb** is a C++ in-memory database framework designed for **financial trading systems**, built on an **in-memory priority read/write + asynchronous log persistence** architecture. It provides a unified interface compatible with multiple mainstream databases, purpose-built for storing and managing core financial business data such as trades, positions, capital, and orders, delivering both high performance and data reliability.

Created by [Fireseeker](https://fireseeker.cn/)

## 1. Introduction
In high-frequency financial trading scenarios, traditional disk-based databases suffer from high read/write latency, while pure in-memory databases risk data loss. Mdb addresses these pain points:
1.  Business reads and writes operate on in-memory data first, ensuring microsecond/millisecond-level response times for low-latency financial requirements;
2.  Database operation logs are automatically generated and **asynchronously persisted to physical databases**, guaranteeing data integrity without loss;
3.  It abstracts away the syntax and interface differences across databases, providing a unified C++ API that seamlessly switches between MySQL, MariaDB, SQLite, and DuckDB backends.

Built with C++20, the project uses CMake for cross-platform builds and Python scripts for automated data model parsing and code generation. It comes with complete table creation scripts and test cases, suitable for trading systems, risk management systems, market data systems, and other financial back-end scenarios.

## 2. Key Features
- ✅ **Multi-Database Compatibility**: Natively supports four database backends: MySQL, MariaDB, SQLite, and DuckDB
- ✅ **Unified Access Interface**: A single API works across all databases — switching storage engines requires no business logic changes
- ✅ **In-Memory + Disk Dual-Write Architecture**: High-speed in-memory reads/writes with asynchronous disk persistence, balancing performance and safety
- ✅ **Financial Domain Data Tables**: Built-in complete trading business data models covering trading days, contracts, accounts, orders, positions, and trade executions
- ✅ **Indexing Capabilities**: Supports primary key and regular index creation and queries, accelerating massive financial data retrieval
- ✅ **Asynchronous Writes**: Database persistence operations execute asynchronously, avoiding blocking of core trading paths
- ✅ **Automated Toolchain**: Python scripts for table model parsing and code generation, improving development efficiency

## 3. Built-in Financial Data Table Models
The framework comes with pre-built financial trading data tables, ready to use out of the box:

| Table Name | Description | Business Use |
| ---- | ---- | ---- |
| TradingDay | Trading Day Table | Records market trading days |
| Exchange | Exchange Table | Stores information for exchanges (SSE, SZSE, futures exchanges, etc.) |
| Product | Product Table | Financial product categories (stocks, futures, options, etc.) |
| Instrument | Contract/Security Table | Specific instrument codes, names, trading parameters, etc. |
| PrimaryAccount | Primary Account Table | Top-level capital account information |
| Account | Sub-Account Table | Trading sub-accounts and tenant account management |
| Capital | Capital Table | Available funds, frozen funds, P&L data, etc. |
| Position | Position Table | Summary data of total position quantities, average prices, etc. |
| PositionDetail | Position Detail Table | Per-trade position detail records |
| Order | Order Table | Full client order information |
| Trade | Trade Execution Table | Completed trade execution details |

## 4. Project Directory Structure
```
Mdb/
├── include/Mdb/              # Public header files
│   ├── Mdb/                  # Core framework base classes and common interfaces
│   ├── MysqlWrapper/         # MySQL database wrapper layer
│   ├── MariadbWrapper/       # MariaDB database wrapper layer
│   ├── SqliteWrapper/        # SQLite database wrapper layer
│   └── DuckdbWrapper/        # DuckDB database wrapper layer
├── src/Mdb/                  # Source code implementation
│   ├── Mdb/                  # Core logic implementation
│   ├── MysqlWrapper/         # MySQL implementation
│   ├── MariadbWrapper/       # MariaDB implementation
│   ├── SqliteWrapper/        # SQLite implementation
│   └── DuckdbWrapper/        # DuckDB implementation
├── Sql/                      # Database-specific SQL table creation scripts
│   ├── Mysql/
│   ├── Mariadb/
│   ├── Sqlite/
│   └── Duckdb/
├── Model/                    # Financial data table model definitions
├── Test/                     # Unit tests & functional tests
├── submodules/               # Dependent submodule: CMakeCommon
├── *.py                      # Python automation tool scripts
├── vcpkg.json                # vcpkg dependency management configuration
├── CMakeLists.txt            # CMake main build configuration
├── CMakeSettings.json        # Visual Studio CMake configuration
├── UpdateSubmodule.bat/sh    # Submodule update scripts (Windows/Linux)
├── .gitmodules               # Git submodule configuration
├── .gitignore                # Git ignore rules
└── LICENSE                   # MIT open-source license
```

## 5. Build Environment & Dependencies
### 5.1 Prerequisites
| Dependency | Version | Notes |
| ---- | ---- | ---- |
| C++ Compiler | C++20 or later | GCC, Clang, MSVC all supported |
| CMake | 3.15 or later | Cross-platform build system |
| Python | 3.6+ | Automation scripts only, not required for runtime |

### 5.2 Database Libraries
Install the corresponding development libraries based on your chosen database backend:
- MySQL / MariaDB: MySQL Connector/C++, MariaDB Connector/C++
- SQLite: SQLite3 development library
- DuckDB: DuckDB official development library
- It is recommended to use `vcpkg` for unified third-party dependency management (a `vcpkg.json` is provided)

### 5.3 Submodule Dependency
The project depends on the `CMakeCommon` submodule. You must pull submodules after cloning.

## 6. Build & Deployment Steps
### 6.1 Clone Repository (Recommended: recursive clone to pull submodules automatically)
```bash
git clone --recursive https://gitee.com/xunmeng200/Mdb.git
cd Mdb
```

### 6.2 Update Submodules Manually (if cloned non-recursively)
```bash
# Linux / macOS
sh UpdateSubmodule.sh

# Windows (CMD/PowerShell)
UpdateSubmodule.bat
```

### 6.3 Standard CMake Build
```bash
# 1. Create build directory
mkdir build && cd build

# 2. Configure CMake project
cmake ..

# 3. Build Release version
cmake --build . --config Release
```
After compilation, library files and test programs are output to the corresponding subdirectories under `build/`.

## 7. Complete Usage Examples
### Example 1: MySQL Connection & Table Initialization
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"

int main()
{
    // Instantiate MySQL wrapper with database address
    mdb::MysqlWrapper db("127.0.0.1");

    // Establish database connection
    if (!db.Connect())
    {
        return -1;
    }

    // Automatically execute table creation for all financial tables
    db.CreateTables();

    return 0;
}
```

### Example 2: Insert Trading Day Data (Single Record)
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"
#include <iostream>

int main()
{
    mdb::MysqlWrapper db("127.0.0.1");
    db.Connect();
    db.CreateTables();

    // Build data model object
    mdb::TradingDay* day = new mdb::TradingDay();
    day->TradingDay = "20260615"; // Trading day string

    // Insert data (in-memory write immediately, async disk persistence)
    db->InsertTradingDay(day);

    delete day;
    return 0;
}
```

### Example 3: Batch Query Table Data
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"
#include <iostream>
#include <list>

int main()
{
    mdb::MysqlWrapper db("127.0.0.1");
    db.Connect();

    // Define container for query results
    std::list<mdb::TradingDay*> resultList;

    // Query all trading day records
    db.SelectTradingDay(resultList);

    // Iterate and print results
    for (auto* item : resultList)
    {
        std::cout << "Trading Day: " << item->TradingDay << std::endl;
    }

    return 0;
}
```

### Example 4: Switching Databases (Using SQLite — Identical Interface)
Only the wrapper class changes; business code remains untouched:
```cpp
#include "Mdb/Mdb.h"
#include "SqliteWrapper/SqliteWrapper.h"

int main()
{
    // Switch to SQLite database
    mdb::SqliteWrapper db("./finance.db");
    db.Connect();
    db.CreateTables();

    // All subsequent CRUD operations are identical to MySQL
    return 0;
}
```

## 8. Python Automation Scripts
The Python scripts in the project root are used for data model parsing and automated code generation to reduce repetitive work:

- `ParsePackageModel.py`: Parses network packet models and generates corresponding data structure code
- `ParseTableModel.py`: Parses data table models and auto-generates C++ entity classes and database operation code
- `ParseShortField.py`: Parses short field configurations and generates field mapping logic in batches
- `geninc.py`: Incremental code generation script for iterative development
- `copyheader.py` / `copymodel.py`: Batch copy for header files and data models
- `clearall.py`: Cleans build artifacts and cache files
- `pump.py` / `pumpall.py`: Bulk data import and synchronization scripts

## 9. Test Programs
Test code is located in the `Test/` directory and can be run directly to verify functionality:

- **Database Connectivity Tests**: Verify connections and table creation for all four databases
- **CRUD Tests**: Verify single and batch data read/write operations
- **Async Write Tests**: Verify in-memory writes and asynchronous disk persistence logic
- **Model Mapping Tests**: Validate field mapping correctness for financial data tables

## 10. License & Disclaimer
- **License**: This project is open-sourced under the [MIT License](LICENSE). Free to use, modify, and redistribute.
- **Use Cases**: Primarily designed for financial trading systems, risk management, and market data analysis; also serves as a C++ database middleware learning reference.
- **Disclaimer**: This is a personal open-source project. It is recommended to conduct thorough stress testing and functional validation in a staging environment before evaluating production readiness. For high-frequency core trading scenarios, perform your own performance tuning and disaster recovery adaptations.

## 11. Contribution Guidelines
Issues and Pull Requests are welcome:

- For bug reports or feature requests, please describe the reproduction steps and use case in detail in the Issue.
- Before submitting a PR, please follow the existing code style and ensure unit tests pass.
- For detailed PR submission guidelines, see `.gitee/PULL_REQUEST_TEMPLATE_zh-CN.md`.
