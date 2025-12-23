# Mdb Project Description

## Project Introduction

Mdb is a database operation project that primarily implements interactions with multiple databases (MariaDB, MySQL, SQLite, DuckDB). It provides a series of interfaces to create, delete, query, and update tables and records within databases. This project is suitable for applications requiring efficient data persistence processing.

### Key Features

- Supports multiple databases (MariaDB, MySQL, SQLite, DuckDB).
- Provides operations to create, delete, and truncate tables.
- Supports inserting, bulk inserting, deleting, and updating records.
- Manages commonly used financial data such as trading days, exchanges, products, contracts, accounts, funds, and positions.

### Directory Structure

- `Source/DB/DBOperate`: Public interface definitions for database operations.
- `Source/DB/DuckDB`: Specific implementation for DuckDB.
- `Source/DB/MariaDB`: Specific implementation for MariaDB.
- `Source/DB/MysqlDB`: Specific implementation for MySQL.
- `Source/DB/SqliteDB`: Specific implementation for SQLite.
- `Source/DB/Mdb`: In-memory database-related implementations.
- `Sql`: Stores database script files.
- `Test/TestDB`: Test code.

## Installation Guide

### Dependencies

- C++ compiler (supporting C++11 or higher)
- Development libraries for MariaDB, MySQL, SQLite, and DuckDB
- CMake

### Compilation Steps

1. Install the required dependencies.
2. Generate build files using CMake.
3. Compile the project.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage Examples

### Initialize Database

You can load data from a CSV file or an existing database using `InitMdbFromCsv` or `InitMdbFromDB`.

```cpp
mdb::Mdb* mdb = new mdb::Mdb();
mdb::InitMdbFromCsv::LoadTables(mdb, "data_directory");
```

### Connect to Database

Example of connecting to MariaDB:

```cpp
MariaDB* db = new MariaDB("host", "user", "password");
db->Connect();
```

### Create Tables

```cpp
db->CreateTables();
```

### Insert Records

```cpp
TradingDay* record = new TradingDay();
// Set fields of record
db->InsertTradingDay(record);
```

### Query Records

```cpp
std::list<TradingDay*> records;
db->SelectTradingDay(records);
```

### Delete Records

```cpp
db->DeleteTradingDay(record);
```

### Truncate Tables

```cpp
db->TruncateTradingDay();
```

## Contribution Guidelines

Contributions are welcome! Please follow these steps:

1. Fork the project.
2. Create a new branch.
3. Commit your changes.
4. Create a Pull Request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.