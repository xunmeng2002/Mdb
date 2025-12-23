# Mdb 项目说明

## 项目介绍

Mdb 是一个内存数据库项目，主要实现主键、唯一键、索引等特性。并根据数据库操作生成数据库日志，并同步到订阅了该数据库日志的物理数据库，支持了与多种数据库（MariaDB、MySQL、SQLite、DuckDB）之间的交互。它提供了一系列接口来创建、删除、查询和更新数据库中的表和记录。该项目适用于需要高性能并进行数据持久化处理的应用场景。

### 主要功能

- 内存数据库（Mdb），提供主键、唯一键、索引。
- 内存数据库提供了表级别的读写锁，写锁在进行增删改操作时自动添加。读锁由用户决定是否需要使用。
- 提供了基于主键的单数据查询、全表查询，以及基于索引的范围查询。
- 提供创建、删除、清空表的操作。
- 支持插入、批量插入、删除、更新记录。
- 支持多种物理数据库（MariaDB、MySQL、SQLite、DuckDB）。

### 目录结构

- `Source/DB/Mdb`: 内存数据库相关实现。
- `Source/DB/DBOperate`: 数据库操作的公共接口定义。
- `Source/DB/DuckDB`: 针对 DuckDB 的具体实现。
- `Source/DB/MariaDB`: 针对 MariaDB 的具体实现。
- `Source/DB/MysqlDB`: 针对 MySQL 的具体实现。
- `Source/DB/SqliteDB`: 针对 SQLite 的具体实现。
- `Sql`: 存放数据库脚本文件。
- `Test/TestDB`: 测试代码。

## 安装指南

### 依赖项

- C++ 编译器（支持 C++11 或更高版本）
- PersonalLib 个人的C++库，支持了线程封装、日志功能。
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

以下是一个使用 MysqlDB 的示例：

```cpp
MsqylDB* db = new MysqlDB("host", "user", "password");
DBWriter* dbWriter = new DBWriter(db);
mdb->Subscribe(dbWriter);//这里dbWrite订阅Mdb的数据库日志
dbWriter->Subscribe(mdb);//Mdb订阅DbWrite的数据库连接状态信息
mdb->SetInitStatus(true);
```

### 创建表

```cpp
mdb->CreateTables();
```

### 插入记录

```cpp
TradingDay* record = new TradingDay();
// 设置 record 的字段
memset(record , 0, sizeof(record ));
record ->PK = 1;
strcpy(record->PreTradingDay, "20251222");
strcpy(record->CurrTradingDay, "20251223");
mdb->t_TradingDay->Insert(record);
```

### 查询记录

```cpp
TradingDay* tradingDay = mdb->t_TradingDay->m_PrimaryKey->Select(1);
```

### 删除记录

```cpp
mdb->t_TradingDay->Erase(tradingDay);
```

### 清空表

```cpp
mdb->t_TradingDay->TruncateTable();
```

## 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 项目。
2. 创建新分支。
3. 提交你的修改。
4. 创建 Pull Request。

## 许可证

本项目采用 MIT 许可证。详情请参阅 LICENSE 文件。