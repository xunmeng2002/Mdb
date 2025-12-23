# Mdb Project Description

## Project Introduction

Mdb is an in-memory database project that mainly implements features such as primary keys, unique keys, and indexes. It generates database logs based on database operations and synchronizes them to the physical database that has subscribed to these database logs, supporting interaction with multiple databases (MariaDB, MySQL, SQLite, DuckDB). It provides a series of interfaces for creating, deleting, querying and updating tables and records in the database. This project is suitable for application scenarios that require high performance and data persistence processing.


### Key Features

- In-memory database (Mdb) provides primary keys, unique keys, and indexes.
- In-memory databases provide table-level read and write locks, and write locks are automatically added when operations such as adding, deleting, or modifying are performed. The read lock is decided by the user whether to use it or not.
- It provides single-data queries based on primary keys, full table queries, and range queries based on indexes.
- Provide operations for creating, deleting, and clearing tables.
- Supports insertion, batch insertion, deletion and update of records.
- Supports multiple physical databases (MariaDB, MySQL, SQLite, DuckDB).


### Directory Structure

- `Source/DB/Mdb`: In-memory database-related implementations.
- `Source/DB/DBOperate`: Public interface definitions for database operations.
- `Source/DB/DuckDB`: Specific implementation for DuckDB.
- `Source/DB/MariaDB`: Specific implementation for MariaDB.
- `Source/DB/MysqlDB`: Specific implementation for MySQL.
- `Source/DB/SqliteDB`: Specific implementation for SQLite.
- `Sql`: Stores database script files.
- `Test/TestDB`: Test code.

## Installation Guide

### Dependencies

- C++ compiler (supporting C++11 or higher)
- PersonalLib's C++ library supports thread encapsulation and logging functions.
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

### Example of using MysqlDB

```cpp
MsqylDB* db = new MysqlDB("host", "user", "password");
DBWriter* dbWriter = new DBWriter(db);
mdb->Subscribe(dbWriter); Here, dbWrite subscribes to the database log of Mdb
dbWriter->Subscribe(mdb); //Mdb subscribes to the database connection status information of DbWrite
mdb->SetInitStatus(true);
```

### Create Tables

```cpp
mdb->CreateTables();
```

### Insert Records

```cpp
TradingDay* record = new TradingDay();
// Set the fields of record
memset(record , 0, sizeof(record ));
int pkTradingDay = 1;
record ->PK = pkTradingDay;
strcpy(record->PreTradingDay, "20251222");
strcpy(record->CurrTradingDay, "20251223");
mdb->t_TradingDay->Insert(record);
```

### Query Records

```cpp
TradingDay* tradingDay = mdb->t_TradingDay->m_PrimaryKey->Select(pkTradingDay);
```

### Delete Records

```cpp
mdb->t_TradingDay->Erase(tradingDay);
```

### Truncate Tables

```cpp
mdb->t_TradingDay->TruncateTable();
```

## Contribution Guidelines

Contributions are welcome! Please follow these steps:

1. Fork the project.
2. Create a new branch.
3. Commit your changes.
4. Create a Pull Request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.