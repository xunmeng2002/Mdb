# Mdb
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C++20+-orange.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake3.20+-green.svg)]()

**Mdb** 是一个基于 **Spark** 与 **DBAdapters** 构建的 C++ **内存数据库参考实现（Demo）**，完整演示"**如何实现并使用内存数据库**"：数据在内存中高速读写，通过订阅接口广播变更，再由 DBAdapters 的 `AsyncDBWriter` 异步同步到 SQLite / DuckDB / MySQL / MariaDB 等持久化数据库。项目内置的金融数据表**仅作为示例数据**，重点展示的是内存数据库的架构与实现方法。

Created by [Fireseeker](https://fireseeker.cn/)

## 一、项目简介

Mdb 是一个**内存数据库的实现示例**，重点是演示"如何实现并使用内存数据库"，而不是具体的业务数据。它展示了内存数据库的完整实现路径：

1. **表结构驱动**：数据表模型由 `Model/*.xml` 定义，通过 Python 脚本（`pump.py` + `pumplist.xml`）自动生成全部表结构代码（记录结构体、类型化表容器、主键容器、二级索引容器），无需手写；
2. **内存优先 + 异步落库**：业务读写优先操作内存，变更通过订阅接口（`MdbSubscriber`）广播，由 DBAdapters 的 `AsyncDBWriter` **异步同步至物理数据库**（SQLite / DuckDB / MySQL / MariaDB），兼顾低时延与数据可靠；
3. **检索与并发**：每张表内置主键容器与二级索引容器（`Select` / `SelectAll` / `EqualRange`），并以 `std::shared_mutex` 支持并发读。

因此，内置的 11 张金融数据表（交易日、账户、订单、持仓等）**仅作为示例数据**，用于演示不同类型字段（定长字符串、枚举、日期、整数）的建表与检索方式；要迁移到自己的业务，只需照葫芦画瓢定义自己的 `Model/*.xml` 并重新生成代码即可。

本项目基于 C++20 开发，采用 CMake 跨平台构建，内置建表脚本与集成测试，既可作为学习内存数据库实现的教程，也可作为搭建自己内存数据层的起点。

## 二、核心特性

以下特性展示了内存数据库实现中需要解决的关键问题，Mdb 均给出了示例方案：

- ✅ **内存 + 磁盘双写架构**：内存高速读写，异步日志落地磁盘，性能与安全兼顾
- ✅ **多数据库兼容**：基于 DBAdapters，原生支持 MySQL、MariaDB、SQLite、DuckDB 四大数据库后端
- ✅ **统一访问接口**：一套内存接口对接所有数据库，切换存储引擎无需修改业务逻辑
- ✅ **金融专属数据表**：内置 11 张交易业务数据表，覆盖交易日、合约、账户、资金、订单、持仓、成交等核心场景
- ✅ **主键 & 二级索引查询**：每张表内置主键容器与二级索引容器，支持 `Select` / `SelectAll` / `EqualRange` 等高效检索
- ✅ **线程安全**：每张表内置 `std::shared_mutex`，提供 `LockShared` / `UnlockShared` 支持并发读
- ✅ **异步写入**：数据库落地操作异步执行，避免阻塞核心交易链路；断线自动重连
- ✅ **导入导出**：支持从数据库（`InitMdbFromDB`）或 CSV（`InitMdbFromCsv`）批量载入内存，支持 `Dump` 全量导出
- ✅ **自动化工具链**：数据表模型驱动代码生成（`MdbStructs` / `MdbTables` / `MdbPrimaryKeys` / `MdbIndexes` 均由模板自动生成）

## 三、示例数据表模型

框架内置 11 张金融交易示例数据表（表结构由 `Model/Tables/Tables.xml` 定义，代码自动生成）。**注意**：这些表与业务字段仅用于演示，不是本项目的重点——它们覆盖了定长字符串、枚举、日期、整数等多种字段类型，用于展示内存数据库如何描述、建表、索引与检索不同类型的字段。

| 表名 | 表 ID | 中文说明 | 业务用途 |
| ---- | ---- | ---- | ---- |
| TradingDay | 0x0001 | 交易日表 | 记录市场正常交易日期 |
| Exchange | 0x0002 | 交易所表 | 存储上交所、深交所、期货交易所等机构信息 |
| Product | 0x0003 | 产品表 | 金融品类分类（股票、期货、期权等） |
| Instrument | 0x0004 | 合约 / 证券表 | 具体标的代码、名称、交易参数等 |
| PrimaryAccount | 0x0005 | 主账户表 | 顶层资金账户信息 |
| Account | 0x0006 | 子账户表 | 交易子账户、租户账户管理 |
| Capital | 0x0007 | 资金表 | 账户可用资金、冻结资金、盈亏等数据 |
| Position | 0x0008 | 持仓表 | 标的总持仓数量、均价等汇总数据 |
| PositionDetail | 0x0009 | 持仓明细表 | 逐笔持仓明细记录 |
| Order | 0x000A | 委托订单表 | 客户委托单全量信息 |
| Trade | 0x000B | 成交记录表 | 撮合完成的成交明细 |

## 四、项目目录结构

```
Mdb/
├── src/Mdb/                       # 源码与对外头文件（含代码生成产物）
│   ├── Mdb.h / Mdb.cpp            # 内存数据库入口（Mdb 类，持有全部类型化表）
│   ├── MdbStructs.h/.cpp          # 11 张表的记录结构体 + TableSchema（代码生成）
│   ├── MdbTables.h/.cpp           # 类型化表容器（TradingDayTable / ExchangeTable / ...）
│   ├── MdbTableBase.h             # 表基类（MdbTableBase：订阅 / InitDB / Truncate / Dump）
│   ├── MdbTableRegistry.h/.cpp    # SchemaRegistry 实现，供 AsyncDBWriter 反查表结构
│   ├── MdbPrimaryKeys.h/.cpp      # 主键索引容器（Select / SelectAll / 唯一性校验）
│   ├── MdbIndexes.h/.cpp          # 二级索引容器（LowerBound / UpperBound / EqualRange）
│   ├── MdbPrimaryKeyComp.h/.cpp   # 主键哈希 / 相等比较器（代码生成）
│   ├── MdbIndexComp.h/.cpp        # 索引比较器（代码生成）
│   ├── TableList.h                # 表清单结构体（TableList）
│   ├── InitMdbFromDB.h/.cpp       # 从持久化数据库批量载入内存
│   └── InitMdbFromCsv.h/.cpp      # 从 CSV 目录批量载入内存
├── test/TestMdb/                  # 集成测试（TestDB：四库 + 异步写库全流程）
│   ├── FullTableList.h            # 全量 11 表的 TableList 定义
│   └── TestDB.cpp
├── Model/                         # 数据表模型定义（Tables.xml / TableNames.xml 等）
│   ├── TableNames/                # 表名清单（FullTableNames.xml）
│   └── Tables/                    # 表结构定义（Tables.xml / ShortTables.xml / TableNames.xml）
├── sql/                           # 各数据库建表脚本
│   ├── Duckdb/                    # CreateDBTables.sql / DropDBTables.sql
│   ├── Mysql/                     # CreateDBTables.sql / DropDBTables.sql
│   └── Sqlite/                    # CreateDBTables.sql / DropDBTables.sql
├── submodules/                    # 子模块依赖（CMakeCommon）
├── bin/                           # 构建产物：可执行文件 / 动态库（按配置分目录）
├── lib/                           # 构建产物：静态库 / 导入库（按配置分目录）
├── out/                           # CMake Presets 构建目录
├── *.py                           # Python 自动化脚本
├── pumplist.xml                   # 代码生成清单（pump.py 驱动）
├── CMakeLists.txt                 # CMake 主构建配置
├── CMakePresets.json              # CMake 预设配置（VS / 命令行）
├── vcpkg.json                     # vcpkg 清单（第三方驱动）
├── UpdateSubmodule.bat/sh         # 子模块更新脚本
├── .gitmodules                    # Git 子模块配置
├── .gitignore                     # Git 忽略规则
└── LICENSE                        # MIT 开源许可证
```

## 五、环境依赖

### 1. 基础要求

| 依赖项 | 版本要求 | 备注 |
| ---- | ---- | ---- |
| C++ 编译器 | C++20 及以上 | GCC、Clang、MSVC 均可 |
| CMake | 3.20 及以上 | 本项目 Presets 需 3.21+ |
| vcpkg | 最新 | `VCPKG_ROOT` 环境变量必需，供 CMake 定位 toolchain |
| Python | 3.6+ | 仅用于代码生成脚本，非运行依赖 |

### 2. 预编译依赖（需手动准备）

项目构建依赖三个**预编译第三方库**，需先安装到项目父目录的 `Libs/` 下（与 `CMakePresets.json` 布局一致）：

| 依赖 | 安装位置 | 提供内容 |
| ---- | ---- | ---- |
| **Spark** 基础库 | `../Libs/Spark/<triplet>` | `Spark::Core`（线程、日志）、`Spark/Types.h` 类型定义 |
| **DBAdapters** 数据库访问层 | `../Libs/DBAdapters/<triplet>` | `DBInterface` / `AsyncDBWriter` / 四种数据库 Wrapper |
| **DuckDB** | `../Libs/duckdb/<triplet>` | `duckdb::duckdb`（DuckDB 后端运行时） |

`<triplet>` 在 Windows 下为 `x64-windows`，Linux / WSL 下为 `x64-linux`。Spark 与 DBAdapters 的构建与安装方法参见各自仓库的 README。

### 3. vcpkg 第三方依赖

`vcpkg.json` 声明的依赖（构建时自动解析，含 protobuf / lz4 / zstd / zlib 等传递依赖）：

- `sqlite3` —— SQLite 驱动
- `mysql-connector-cpp` —— MySQL X DevAPI 驱动
- `mariadb-connector-cpp` —— MariaDB 驱动

### 4. 子模块依赖

- **CMakeCommon**：公共 CMake 宏集合，克隆后需同步拉取子模块。

> 详细的环境搭建步骤（代理配置、vcpkg 安装、WSL 镜像网络等）请参见 [环境准备指南](docs/environment-setup.md)。

## 六、快速构建 & 编译

### 1. 克隆代码（含子模块）

```bash
git clone --recursive https://gitee.com/xunmeng2002/Mdb.git
cd Mdb
```

### 2. 更新子模块（若未递归克隆）

```bash
# Linux / Mac
sh UpdateSubmodule.sh

# Windows
UpdateSubmodule.bat
```

### 3. 准备依赖

```bash
# 确保 VCPKG_ROOT 已配置（Windows 设为系统环境变量，Linux 写入 ~/.bashrc）
# 确保 Spark、DBAdapters、duckdb 已安装到 ../Libs/ 对应目录（见上文"预编译依赖"）
```

### 4. CMake 编译（推荐使用 Presets）

```bash
# Windows（MSVC，Ninja）
cmake --preset x64-Release
cmake --build out/build/x64-Release

# Linux / WSL（GCC）
cmake --preset WSL-GCC-Release
cmake --build out/build/WSL-GCC-Release
```

编译完成后，`MdbStatic` 静态库输出至 `lib/<Config>`，测试程序 `TestDB` 输出至 `bin/<Config>`（如 `bin/Release/TestDB.exe`）。Windows 下 vcpkg 的 applocal 机制会把 `sqlite3.dll`、`mysqlcppconnx-*.dll`、`mariadbcpp.dll`、`duckdb.dll` 等运行时 DLL 自动拷贝到可执行文件旁。

### 5. 运行集成测试

```bash
# Windows
./bin/Release/TestDB.exe

# Linux
./bin/Release/TestDB
```

测试程序依次对 SQLite / DuckDB / MySQL 执行"内存库 + 异步写库"全流程（MariaDB 默认注释关闭，需本地服务），验证内存读写、异步落库、主键 / 索引查询。

## 七、使用示例

### 示例 1：内存库 + 异步落库（完整接线）

```cpp
#include "Mdb.h"
#include "MdbTableRegistry.h"
#include "FullTableList.h"
#include <DBAdapters/AsyncDBWriter/AsyncDBWriter.h>
#include <DBAdapters/SqliteWrapper/SqliteWrapper.h>
#include <Spark/Core/Core.h>
#include <cstring>

using namespace mdb;
using namespace mdb::full;
using namespace dbadapters;
using namespace spark::core;

// 内存库包含的金融数据表清单（全量 11 表）不手写：由 Model/TableNames/FullTableNames.xml
// 经 pumpall.py 生成到 FullTableList.h，勿手改

int main(int argc, const char* argv[])
{
    Logger::GetInstance().Init(argv[0]);
    Logger::GetInstance().SetLogLevel(LogLevel::Info, LogLevel::Info);
    Logger::GetInstance().Start();

    // 1. 内存数据库：按表清单创建 11 张类型化表
    Mdb* mdb = new Mdb(fullTableList);

    // 2. 持久化链路：DB 适配器 + Schema 注册表 + 异步写库器
    DB* db = new SqliteWrapper("./Test.sqlitedb");          // 以 SQLite 落库
    MdbTableRegistry* schemaRegistry = new MdbTableRegistry(fullTableList);
    AsyncDBWriter* dbWriter = new AsyncDBWriter(db, schemaRegistry);

    // 3. 双向接线
    //    mdb->Subscribe(dbWriter)  ：内存表变更广播给 AsyncDBWriter，异步写库
    //    dbWriter->Subscribe(mdb)  ：连接成功回调 Mdb::OnDBConnected，自动 InitDB（建表 + 初始化）
    mdb->Subscribe(dbWriter);
    dbWriter->Subscribe(mdb);
    dbWriter->Connect();                                    // 同步连接并触发建表
    dbWriter->Start();                                      // 启动后台消费线程

    // 4. 内存写入，异步落库
    Exchange* exchange = new Exchange();
    std::memset(exchange, 0, sizeof(Exchange));
    std::strcpy(exchange->ExchangeID, "SHFE");
    std::strcpy(exchange->ExchangeName, u8"上海期货交易所");
    mdb->exchange->Insert(exchange);                      // 返回 false 表示主键冲突

    Account* account = new Account();
    std::memset(account, 0, sizeof(Account));
    std::strcpy(account->AccountID, "A001");
    std::strcpy(account->AccountName, u8"张三");
    account->AccountType = AccountTypeType::Primary;
    account->AccountStatus = AccountStatusType::Normal;
    mdb->account->Insert(account);

    // 5. 收尾：停止写库线程，解除订阅
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

### 示例 2：主键 & 二级索引查询

```cpp
// 主键查询：按唯一主键定位单条记录
ExchangeIDType exchangeID("CFFEX");
Exchange* exchange = mdb->exchange->primaryKey->Select(exchangeID);
WriteLog(LogLevel::Info, "%s", exchange->GetDebugString());

// 主键全量遍历：返回 [begin, end) 迭代器对
auto all = mdb->exchange->primaryKey->SelectAll();
for (auto it = all.first; it != all.second; ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// 二级索引等值区间查询（如按报盘代码 OfferID 查主账户）
OfferIDType offerID = 10001;
auto range = mdb->primaryAccount->offerIDIndex->EqualRange(offerID);
for (auto it = range.first; it != range.second; ++it)
{
    WriteLog(LogLevel::Info, "%s", (*it)->GetDebugString());
}

// 按二级索引批量删除（如清空某交易日的资金记录）
mdb->capital->EraseByTradingDayIndex(tradingDay->PreTradingDay);
```

### 示例 3：更新 / 删除 / 清表

```cpp
// 更新：内存同步更新，并通过 MdbSubscriber 广播到 AsyncDBWriter 异步落库
AccountIDType accountID;
std::strcpy(accountID, "A001");
Account* oldAccount = mdb->account->primaryKey->Select(accountID);
Account* newAccount = new Account();
*newAccount = *oldAccount;                              // 拷贝后修改
std::strcpy(newAccount->AccountName, u8"李四");
mdb->account->Update(oldAccount, newAccount);

// 删除单条记录（同步删除内存并广播落库删除）
mdb->account->Erase(oldAccount);

// 清空单表
mdb->account->TruncateTable();

// 清空全部表（广播 TruncateTables + 各表本地清空）
mdb->TruncateTables();
```

### 示例 4：从数据库 / CSV 载入与 Dump 导出

```cpp
#include "InitMdbFromDB.h"
#include "InitMdbFromCsv.h"

// 从持久化数据库批量载入内存（建表后全表加载）
InitMdbFromDB::LoadTables(mdb, db, fullTableList);

// 从 CSV 目录批量载入内存
InitMdbFromCsv::LoadTables(mdb, "./dump", fullTableList);

// 内存全量导出到指定目录（Dump 各表数据）
mdb->Dump("./dump");
```

## 八、Python 脚本说明

数据表模型由 `Model/*.xml` 定义，根目录 Python 脚本用于代码生成与自动化处理：

| 脚本 | 说明 |
| ---- | ---- |
| pump.py / pumpall.py / pumptemp.py | 模板代码生成引擎（由 pumplist.xml 驱动），从 `Model/*.xml` 生成 Mdb 表结构代码 |
| pumplist.xml | 代码生成清单：模板 → 目标文件的映射 |
| ParseTableModel.py | 数据表模型解析 |
| ParsePackageModel.py | 网络数据包模型解析 |
| ParseShortField.py / ParseShortItem.py | 短字段 / 短条目解析 |
| geninc.py | 头文件引用（include）自动生成 |
| ConvertToUtf8Bom.py | 文件编码统一转换为 UTF-8 BOM |
| parseall.py | 批量解析入口 |
| clearall.py | 清理临时文件、缓存 |

## 九、测试程序

`test/TestMdb/TestDB.cpp` 集成测试覆盖以下内容：

| 测试项 | 说明 |
| ---- | ---- |
| `TestSqlite` | SQLite 后端：内存库 + 异步写库全流程 |
| `TestDuckdb` | DuckDB 后端：内存库 + 异步写库全流程 |
| `TestMysql` | MySQL 后端全流程（需本机 MySQL X Plugin，默认注释关闭） |
| `TestMariadb` | MariaDB 后端全流程（需本机 MariaDB，默认注释关闭） |
| 主键 / 索引 | `primaryKey->Select` / `SelectAll`、`offerIDIndex->EqualRange` 等检索验证 |
| 内存操作 | Insert / Update / Erase / TruncateTable 与广播落库验证 |

## 十、许可证 & 声明

- **开源协议**：本项目基于 [MIT 许可证](LICENSE) 开源，可自由使用、修改、二次分发
- **适用场景**：作为内存数据库**实现示例**与学习教程，可参考其架构搭建自己的内存数据层；内置金融数据表可替换为任意业务表
- **风险提示**：本项目为个人开源项目，建议在测试环境充分压测、功能验证后，再评估是否接入生产环境；高频核心交易场景请自行做性能调优与容灾改造

## 十一、补充说明

- **项目定位**：本项目是内存数据库的**实现示例**，重点在"如何实现与使用"；内置金融数据表仅作演示，新业务可基于 `Model/*.xml` + 代码生成快速定义自己的表结构
- **依赖关系**：Mdb 构建在 [Spark](https://gitee.com/xunmeng2002/Spark.git)（基础能力）与 [DBAdapters](https://gitee.com/xunmeng2002/DBAdapters.git)（统一数据库访问层）之上，二者均为预编译依赖
- **包含路径**：Mdb 头文件位于 `src/Mdb/`，以 `#include "Mdb.h"` 风格引用
- **命名空间**：全部接口位于 `mdb` 命名空间；数据库接口复用 `dbadapters` 命名空间
- **并发访问**：内存表为多线程安全设计，读操作建议先 `LockShared()` 再访问主键 / 索引容器，避免与写线程竞争
- **代码生成**：修改 `Model/*.xml` 后运行 `python pumpall.py` 可重新生成表结构代码，无需手写
