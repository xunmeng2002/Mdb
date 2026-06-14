# Mdb

Mdb is an in-memory database (In-Memory Database) framework designed for financial trading systems, providing a unified access interface for multiple relational databases.

## Project Overview

Mdb is designed for financial trading systems and supports multiple database backends, including MySQL, MariaDB, SQLite, and DuckDB. It provides a unified database operation interface to support storage, querying, and management of trading data (positions, orders, trades, capital, etc.).

## Key Features

- **Multi-database Support**: Supports four database backends: MySQL, MariaDB, SQLite, and DuckDB
- **Unified Interface**: Offers a consistent database operation interface for easy switching between different databases
- **Comprehensive Trading Data Model**: Includes complete data tables for trading-related information
- **Index Support**: Supports creation and querying of primary keys and multiple types of indexes
- **Asynchronous Writes**: Supports asynchronous database write operations

## Data Table Models

| Table Name | Description |
|------------|-------------|
| TradingDay | Trading Day |
| Exchange   | Exchange |
| Product    | Product |
| Instrument | Contract/Security |
| PrimaryAccount | Primary Account |
| Account    | Account |
| Capital    | Capital |
| Position   | Position |
| PositionDetail | Position Detail |
| Order      | Order |
| Trade      | Trade |

## Project Structure

```
Mdb/
├── include/Mdb/           # Header files
│   ├── Mdb/               # Core database classes
│   ├── MysqlWrapper/     # MySQL wrapper class
│   ├── MariadbWrapper/   # MariaDB wrapper class
│   ├── SqliteWrapper/    # SQLite wrapper class
│   └── DuckdbWrapper/    # DuckDB wrapper class
├── src/Mdb/               # Source code
│   ├── Mdb/               # Core implementation
│   ├── MysqlWrapper/     # MySQL implementation
│   ├── MariadbWrapper/   # MariaDB implementation
│   ├── SqliteWrapper/    # SQLite implementation
│   └── DuckdbWrapper/    # DuckDB implementation
├── Sql/                   # SQL scripts
│   ├── Mysql/            # MySQL table creation scripts
│   ├── Mariadb/          # MariaDB table creation scripts
│   ├── Sqlite/           # SQLite table creation scripts
│   └── Duckdb/           # DuckDB table creation scripts
├── Model/                # Data model definitions
├── Test/                  # Test code
└── *.py                  # Auxiliary scripts
```

## Compilation Requirements

- C++17 or higher
- CMake 3.15+
- MySQL Connector/C++ or MariaDB Connector/C++
- SQLite3 development library
- DuckDB development library

## Usage Example

### Create Database Connection

```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"

// Use MySQL
mdb::MysqlWrapper* db = new mdb::MysqlWrapper("localhost");
db->Connect();

// Create tables
db->CreateTables();
```

### Insert Data

```cpp
// Create a trading day record
mdb::TradingDay* tradingDay = new mdb::TradingDay();
tradingDay->TradingDay = "20240101";

// Insert the record
db->InsertTradingDay(tradingDay);
```

### Query Data

```cpp
std::list<mdb::TradingDay*> records;
db->SelectTradingDay(records);

for (auto* record : records) {
    std::cout << record->TradingDay << std::endl;
}
```

## Python Utility Scripts

The project provides several Python scripts for data model parsing and code generation:

- `ParsePackageModel.py` - Parse package model
- `ParseTableModel.py` - Parse table model
- `ParseShortField.py` - Parse short fields
- `geninc.py` - Generate incremental code

## License

Please refer to the LICENSE file for specific licensing information.

## Contribution Guidelines

Please read .gitee/PULL_REQUEST_TEMPLATE_zh-CN.md for requirements on submitting pull requests.