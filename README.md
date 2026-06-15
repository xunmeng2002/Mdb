# Mdb
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C++20+-orange.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake3.15+-green.svg)]()

**Mdb** 是面向**金融交易系统**设计的 C++ 内存数据库框架，核心实现**内存数据优先读写 + 日志异步持久化**架构，内置统一接口适配多款主流数据库，专门用于存储和管理交易、持仓、资金、订单等金融核心业务数据，兼顾高性能与数据可靠性。

## 一、项目简介
在金融高频交易场景中，传统磁盘数据库读写延迟高，纯内存数据库又存在数据丢失风险。Mdb 针对性解决该痛点：
1.  业务读写优先操作内存，保障微秒/毫秒级响应速度，适配金融低时延要求；
2.  自动生成数据库操作日志，**异步同步至物理数据库**，保证数据落地不丢失；
3.  屏蔽不同数据库的语法与接口差异，提供一套统一的 C++ 调用接口，可无缝切换 MySQL、MariaDB、SQLite、DuckDB 后端。

本项目基于 C++20 开发，采用 CMake 跨平台构建，搭配 Python 脚本实现数据模型自动化解析与代码生成，内置全套建表脚本与测试用例，适用于交易系统、风控系统、行情系统等金融后端场景。

## 二、核心特性
- ✅ **多数据库兼容**：原生支持 MySQL、MariaDB、SQLite、DuckDB 四大数据库后端
- ✅ **统一访问接口**：一套代码对接所有数据库，切换存储引擎无需修改业务逻辑
- ✅ **内存+磁盘双写架构**：内存高速读写，异步日志落地磁盘，性能与安全兼顾
- ✅ **金融专属数据表**：内置完整的交易业务数据表模型，覆盖交易日、合约、账户、订单、持仓、成交等核心场景
- ✅ **索引能力**：支持主键、普通索引创建与索引查询，加速海量金融数据检索
- ✅ **异步写入**：数据库落地操作异步执行，避免阻塞核心交易链路
- ✅ **自动化工具链**：配套 Python 脚本，实现表模型解析、代码自动生成，提升开发效率

## 三、内置金融数据表模型
框架预置金融交易全链路数据表，开箱即用：

| 表名 | 中文说明 | 业务用途 |
| ---- | ---- | ---- |
| TradingDay | 交易日表 | 记录市场正常交易日期 |
| Exchange | 交易所表 | 存储上交所、深交所、期货交易所等机构信息 |
| Product | 产品表 | 金融品类分类（股票、期货、期权等） |
| Instrument | 合约/证券表 | 具体标的代码、名称、交易参数等 |
| PrimaryAccount | 主账户表 | 顶层资金账户信息 |
| Account | 子账户表 | 交易子账户、租户账户管理 |
| Capital | 资金表 | 账户可用资金、冻结资金、盈亏等数据 |
| Position | 持仓表 | 标的总持仓数量、均价等汇总数据 |
| PositionDetail | 持仓明细表 | 逐笔持仓明细记录 |
| Order | 委托订单表 | 客户委托单全量信息 |
| Trade | 成交记录表 | 撮合完成的成交明细 |

## 四、项目目录结构
```
Mdb/
├── include/Mdb/              # 对外头文件目录
│   ├── Mdb/                  # 框架核心基类与通用接口
│   ├── MysqlWrapper/         # MySQL 数据库封装层
│   ├── MariadbWrapper/       # MariaDB 数据库封装层
│   ├── SqliteWrapper/        # SQLite 数据库封装层
│   └── DuckdbWrapper/        # DuckDB 数据库封装层
├── src/Mdb/                  # 源码实现目录
│   ├── Mdb/                  # 核心逻辑实现
│   ├── MysqlWrapper/         # MySQL 功能实现
│   ├── MariadbWrapper/       # MariaDB 功能实现
│   ├── SqliteWrapper/        # SQLite 功能实现
│   └── DuckdbWrapper/        # DuckDB 功能实现
├── Sql/                      # 各数据库专属 SQL 建表脚本
│   ├── Mysql/
│   ├── Mariadb/
│   ├── Sqlite/
│   └── Duckdb/
├── Model/                    # 金融数据表模型定义文件
├── Test/                     # 单元测试 & 功能测试代码
├── submodules/               # 依赖子模块 CMakeCommon
├── *.py                      # Python 自动化工具脚本
├── vcpkg.json                # vcpkg 依赖管理配置
├── CMakeLists.txt            # CMake 主构建配置
├── CMakeSettings.json        # Visual Studio CMake 配置
├── UpdateSubmodule.bat/sh    # 子模块更新脚本（Windows/Linux）
├── .gitmodules               # Git 子模块配置
├── .gitignore                # Git 忽略规则
└── LICENSE                   # MIT 开源许可证
```

## 五、编译环境与依赖
### 1. 基础环境要求
| 依赖项 | 版本要求 | 备注 |
| ---- | ---- | ---- |
| C++ 编译器 | C++20 及以上 | GCC、Clang、MSVC 均可 |
| CMake | 3.15 及以上 | 跨平台编译核心工具 |
| Python | 3.6+ | 仅用于自动化脚本，非运行依赖 |

### 2. 数据库依赖库
根据使用的后端数据库，安装对应开发库：
- MySQL / MariaDB：MySQL Connector/C++ 、MariaDB Connector/C++
- SQLite：SQLite3 开发库
- DuckDB：DuckDB 官方开发库
- 推荐使用 `vcpkg` 统一管理第三方依赖（项目已提供 `vcpkg.json`）

### 3. 子模块依赖
项目依赖 `CMakeCommon` 子模块，克隆代码后必须同步拉取子模块。

## 六、编译 & 部署步骤
### 1. 克隆代码（推荐递归克隆，自动拉取子模块）
```bash
git clone --recursive https://gitee.com/xunmeng200/Mdb.git
cd Mdb
```

### 2. 手动更新子模块（非递归克隆时执行）
```bash
# Linux / macOS 系统
sh UpdateSubmodule.sh

# Windows 系统（CMD/PowerShell）
UpdateSubmodule.bat
```

### 3. CMake 标准编译流程
```bash
# 1. 创建编译目录
mkdir build && cd build

# 2. 配置 CMake 项目
cmake ..

# 3. 编译 Release 正式版本
cmake --build . --config Release
```
编译完成后，库文件、测试程序会输出至 build 目录下对应子文件夹。

## 七、完整使用示例
### 示例 1：MySQL 数据库连接 + 初始化数据表
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"

int main()
{
    // 实例化 MySQL 封装对象，传入数据库地址
    mdb::MysqlWrapper db("127.0.0.1");

    // 建立数据库连接
    if (!db.Connect())
    {
        return -1;
    }

    // 自动执行建表语句，初始化所有金融业务表
    db.CreateTables();

    return 0;
}
```

### 示例 2：插入交易日数据（单条数据写入）
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"
#include <iostream>

int main()
{
    mdb::MysqlWrapper db("127.0.0.1");
    db.Connect();
    db.CreateTables();

    // 构建数据模型对象
    mdb::TradingDay* day = new mdb::TradingDay();
    day->TradingDay = "20260615"; // 交易日字符串

    // 插入数据（内存即时写入，磁盘异步落地）
    db->InsertTradingDay(day);

    delete day;
    return 0;
}
```

### 示例 3：批量查询数据表数据
```cpp
#include "Mdb/Mdb.h"
#include "MysqlWrapper/MysqlWrapper.h"
#include <iostream>
#include <list>

int main()
{
    mdb::MysqlWrapper db("127.0.0.1");
    db.Connect();

    // 定义容器接收查询结果
    std::list<mdb::TradingDay*> resultList;

    // 查询所有交易日数据
    db.SelectTradingDay(resultList);

    // 遍历打印结果
    for (auto* item : resultList)
    {
        std::cout << "交易日：" << item->TradingDay << std::endl;
    }

    return 0;
}
```

### 示例 4：切换数据库（以 SQLite 为例，接口完全一致）
仅需修改封装类，业务代码无需改动：
```cpp
#include "Mdb/Mdb.h"
#include "SqliteWrapper/SqliteWrapper.h"

int main()
{
    // 切换为 SQLite 数据库
    mdb::SqliteWrapper db("./finance.db");
    db.Connect();
    db.CreateTables();
    
    // 后续增删改查代码与 MySQL 完全一致
    return 0;
}
```

## 八、Python 自动化脚本说明
项目根目录下 Python 脚本用于数据模型解析、代码自动生成，减少重复编码工作：

- `ParsePackageModel.py`：解析网络数据包模型，生成对应数据结构代码
- `ParseTableModel.py`：解析数据表模型，自动生成 C++ 实体类与数据库操作代码
- `ParseShortField.py`：解析短字段配置，批量生成字段映射逻辑
- `geninc.py`：增量代码生成脚本，适配迭代开发
- `copyheader.py` / `copymodel.py`：头文件、数据模型批量复制
- `clearall.py`：清理编译临时文件、缓存文件
- `pump.py` / `pumpall.py`：批量数据导入、同步脚本

## 九、测试程序说明
测试代码统一存放在 `Test/` 目录，可直接运行验证功能：

- **数据库连通性测试**：验证四大数据库连接、建表能力
- **CRUD 增删改查测试**：验证单条 / 批量数据读写
- **异步写入测试**：验证内存写入、磁盘异步同步逻辑
- **模型适配测试**：校验金融数据表字段映射正确性

## 十、许可证与声明
- **开源协议**：本项目基于 [MIT 许可证](LICENSE) 开源，可自由使用、修改、二次分发
- **适用场景**：主要面向金融交易、风险管理、行情分析等系统开发，也可作为 C++ 数据库中间件学习案例
- **风险提示**：本项目为个人开源项目，建议在测试环境充分压测、功能验证后，再评估是否接入生产环境；高频核心交易场景请自行做性能调优与容灾改造

## 十一、贡献指南
欢迎提交 Issue、Pull Request 参与项目迭代：

- 提交 Bug 反馈或功能建议，请在 Issue 中详细描述复现步骤、使用场景
- 提交代码 PR 前，请遵循现有代码风格，并完成单元测试
- 详细 PR 提交规范参考目录 `.gitee/PULL_REQUEST_TEMPLATE_zh-CN.md`
