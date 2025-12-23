# Mdb 项目说明

## 项目介绍

Mdb 是一个数据库操作项目，主要实现了与多种数据库（MariaDB、MySQL、SQLite、DuckDB）之间的交互。它提供了一系列接口来创建、删除、查询和更新数据库中的表和记录。该项目适用于需要高效地进行数据持久化处理的应用场景。

### 主要功能

- 支持多种数据库（MariaDB、MySQL、SQLite、DuckDB）。
- 提供创建、删除、清空表的操作。
- 支持插入、批量插入、删除、更新记录。
- 提供对交易日、交易所、产品、合约、账户、资金、持仓等金融领域常用数据的管理。

### 目录结构

- `Source/DB/DBOperate`: 数据库操作的公共接口定义。
- `Source/DB/DuckDB`: 针对 DuckDB 的具体实现。
- `Source/DB/MariaDB`: 针对 MariaDB 的具体实现。
- `Source/DB/MysqlDB`: 针对 MySQL 的具体实现。
- `Source/DB/SqliteDB`: 针对 SQLite 的具体实现。
- `Source/DB/Mdb`: 内存数据库相关实现。
- `Sql`: 存放数据库脚本文件。
- `Test/TestDB`: 测试代码。

## 安装指南

### 依赖项

- C++ 编译器（支持 C++11 或更高版本）
- MariaDB、MySQL、SQLite、DuckDB 开发库
- CMake

### 编译步骤

1. 安装依赖库。
2. 使用 CMake 生成构建文件。
3. 编译项目。

```bash
mkdir build
cd build
cmake ..
make
```

## 使用示例

### 初始化数据库

你可以通过 `InitMdbFromCsv` 或 `InitMdbFromDB` 来从 CSV 文件或已有数据库中加载数据。

```cpp
mdb::Mdb* mdb = new mdb::Mdb();
mdb::InitMdbFromCsv::LoadTables(mdb, "data_directory");
```

### 连接数据库

以下是一个连接到 MariaDB 的示例：

```cpp
MariaDB* db = new MariaDB("host", "user", "password");
db->Connect();
```

### 创建表

```cpp
db->CreateTables();
```

### 插入记录

```cpp
TradingDay* record = new TradingDay();
// 设置 record 的字段
db->InsertTradingDay(record);
```

### 查询记录

```cpp
std::list<TradingDay*> records;
db->SelectTradingDay(records);
```

### 删除记录

```cpp
db->DeleteTradingDay(record);
```

### 清空表

```cpp
db->TruncateTradingDay();
```

## 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 项目。
2. 创建新分支。
3. 提交你的修改。
4. 创建 Pull Request。

## 许可证

本项目采用 MIT 许可证。详情请参阅 LICENSE 文件。