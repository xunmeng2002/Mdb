# Mdb

Mdb 是一个用于金融交易系统的内存数据库（In-Memory Database）框架，提供了对多种关系型数据库的统一访问接口。

## 项目简介

Mdb 为金融交易系统设计，支持多种数据库后端，包括 MySQL、MariaDB、SQLite 和 DuckDB。它提供了统一的数据库操作接口，支持交易数据（持仓、订单、成交、资金等）的存储、查询和管理。

## 主要特性

- **多数据库支持**：支持 MySQL、MariaDB、SQLite、DuckDB 四种数据库后端
- **统一接口**：提供统一的数据库操作接口，便于在不同数据库间切换
- **完整的交易数据模型**：包含完整的交易相关数据表
- **索引支持**：支持主键和多种索引的创建与查询
- **异步写入**：支持异步数据库写入操作

## 数据表模型

| 表名 | 说明 |
|------|------|
| TradingDay | 交易日 |
| Exchange | 交易所 |
| Product | 产品 |
| Instrument | 合约/证券 |
| PrimaryAccount | 主账户 |
| Account | 账户 |
| Capital | 资金 |
| Position | 持仓 |
| PositionDetail | 持仓明细 |
| Order | 订单 |
| Trade | 成交 |

## 项目结构

```
Mdb/
├── include/Mdb/           # 头文件
│   ├── Mdb/               # 核心数据库类
│   ├── MysqlWrapper/     # MySQL 包装类
│   ├── MariadbWrapper/    # MariaDB 包装类
│   ├── SqliteWrapper/    # SQLite 包装类
│   └── DuckdbWrapper/    # DuckDB 包装类
├── src/Mdb/               # 源代码
│   ├── Mdb/               # 核心实现
│   ├── MysqlWrapper/     # MySQL 实现
│   ├── MariadbWrapper/   # MariaDB 实现
│   ├── SqliteWrapper/    # SQLite 实现
│   └── DuckdbWrapper/    # DuckDB 实现
├── Sql/                   # SQL 脚本
│   ├── Mysql/            # MySQL 建表脚本
│   ├── Mariadb/          # MariaDB 建表脚本
│   ├── Sqlite/           # SQLite 建表脚本
│   └── Duckdb/          # DuckDB 建表脚本
├── Model/                # 数据模型定义
├── Test/                  # 测试代码
└── *.py                  # 辅助脚本
```

## 编译要求

- C++17 或更高版本
- CMake 3.15+
- MySQL Connector/C++ 或 MariaDB Connector/C++
- SQLite3 开发库
- DuckDB 开发库

## 使用示例

### 创建数据库连接

```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"

// 使用 MySQL
mdb::MysqlWrapper* db = new mdb::MysqlWrapper("localhost");
db->Connect();

// 创建表
db->CreateTables();
```

### 插入数据

```cpp
// 创建交易日记录
mdb::TradingDay* tradingDay = new mdb::TradingDay();
tradingDay->TradingDay = "20240101";

// 插入记录
db->InsertTradingDay(tradingDay);
```

### 查询数据

```cpp
std::list<mdb::TradingDay*> records;
db->SelectTradingDay(records);

for (auto* record : records) {
    std::cout << record->TradingDay << std::endl;
}
```

## Python 工具脚本

项目提供了多个 Python 脚本用于数据模型解析和代码生成：

- `ParsePackageModel.py` - 解析数据包模型
- `ParseTableModel.py` - 解析表模型
- `ParseShortField.py` - 解析短字段
- `geninc.py` - 生成增量代码

## 许可证

请查看 LICENSE 文件了解具体的许可证信息。

## 贡献指南

请阅读 .gitee/PULL_REQUEST_TEMPLATE_zh-CN.md 了解提交 PR 的要求。