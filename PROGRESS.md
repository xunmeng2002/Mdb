# Mdb 进度记录

> 按项目治理规则维护：每次会话结束更新本文件，下次会话开始先读取。

## ✅ 已完成

- **项目名统一为 `DbAdapters`（2026-10-04，DBAdapters 仓发起）**：用户指令「DBAdapters 这个命名都要改成 DbAdapters」，DBAdapters 仓扫全盘后一并改了四个仓。本仓共 51 处：`CMakePresets.json` 安装前缀 2、`src/Mdb/{InitMdbFromDB,MdbStructs,MdbTableBase,MdbTableRegistry}.h` 各 1、`test/TestMdb/TestMdb.cpp` 7、`README.md`/`README.en.md` 各 12、`docs/environment-setup.md`/`.en.md` 各 7。**其中最要紧的是 `src/` 与 `test/` 那 11 处 `#include <DBAdapters/...>`**：NTFS 大小写不敏感，Windows/MSVC 一直编译得过，直到源码被 VS `copySources` 复制到 ext4 才报"找不到头文件"——与 DBAdapters 仓 2026-09-27「WSL 构建失败修复」条**同源**，本批提前消掉。**验证**：本仓 `git grep -In "DBAdapters"`（排除本文件）归零；改后 14 条 `<DbAdapters/...>` include 逐条与 DBAdapters 仓 `git ls-files include/DbAdapters/**` 做**大小写精确**比对，12 条命中，2 条未命中（见 ❓）。改动量逐文件核对为「增删相等且等于该文件出现次数」，无行尾改写。**未提交**。**风险（§7）**：纯大小写改名，无逻辑、无 API、无依赖变更；`CMakePresets.json` 那两行本就无效（本仓无 `install()` 规则）。**补记（同日，第二批）**：用户指令「DBInterface 这个也要改」，把上面「2 条未命中」收口——`test/TestMdb/TestMdb.cpp:5,6` 两处 include 与中英 README 依赖表各 1 处，共 4 处 `DBInterface` → `DbInterface`。改后本仓 14 条 `<DbAdapters/...>` include 与 DBAdapters 仓 `git ls-files include/DbAdapters/**` **全部按大小写命中**。同轮 QuantTrading 侧也改了 2 处（见该仓 `PROGRESS.md`）。**未提交**。

- **中英文 README 重写**：原 `README.md` / `README.en.md` 已过期（目录结构错误、API 示例错误、CMake 3.15、缺 DBAdapters 依赖），已按实际代码重写为与 Spark / DBAdapters 一致的风格，涵盖项目简介、核心特性、11 张金融数据表模型、目录结构、环境依赖（Spark + DBAdapters + duckdb 预编译、vcpkg 三驱动、CMakeCommon 子模块）、Presets 快速构建、使用示例（完整接线 / 主键索引查询 / 更新删除清表 / InitMdbFromDB·Csv·Dump）、Python 脚本、测试程序、许可证、补充说明。
- **项目定位修正（用户澄清）**：Mdb 是"如何实现并使用内存数据库"的**实现示例（Demo）**，内置的数据库表与业务字段**不是重点**。已据此调整两份 README 的开篇、项目简介、第三节（改为"示例数据表模型"）、适用场景与补充说明，强调架构与实现方法，弱化金融业务语义。
- **环境准备文档修正**：将 Mdb 下 `docs/environment-setup.md` / `environment-setup.en.md` 中残留的 Spark 内容替换为 Mdb 专属：依赖概览（三个预编译依赖）、1.6 验证（驱动 + Spark/DBAdapters/duckdb + TestDB.exe）、2.6 WSL 路径（`cd /mnt/d/Gitee/Mdb`，去掉无 install 规则的 install prefix）、3.2 FAQ（Could not find Spark/DBAdapters/duckdb）、TOC 锚点同步修正。

## 🔄 进行中

- 无

## ❓ 待讨论 / 待决策

- README 落款沿用 Spark 的 "Created by [Fireseeker]"（Mdb LICENSE 同为 xunmeng2002）——如需调整署名请告知。
- Mdb 构建依赖 Spark、DBAdapters、duckdb 三个**预编译库**（`../Libs/<name>/<triplet>`），`vcpkg.json` 未声明三者；若考虑可复现构建，可讨论是否将 duckdb 纳入 vcpkg 管理。
- Mdb 的 `CMakeLists.txt` **没有 install() 规则**，但 `CMakePresets.json` 中 `CMAKE_INSTALL_PREFIX` 指向 `../Libs/DbAdapters/<triplet>`（从 DBAdapters 复制粘贴的遗留）。文档 2.6 已去掉 install prefix 以匹配现状；是否补全 install 规则、修正 preset 待定。**改名补记（2026-10-04）**：本批只把 `DBAdapters` 的大小写统一为 `DbAdapters`，该行**仍然无效**（无 `install()` 规则这一条未变），"补全 install 规则 or 删掉该行"的决策**仍待定**。
- **中英 README 里的 `AsyncDBWriter` 拼写是错的（2026-10-04 发现，未改）**：本库的类名与模块目录都是 `AsyncDbWriter`（`CMakeLists.txt:17` 的 `class ASYNCDBWRITER_EXPORTS AsyncDbWriter`），而本仓 `README.md` / `README.en.md` 共 16 处写成 `AsyncDBWriter`。**其中 `README.md:193` / `README.en.md:193` 是实打实的坏示例**：`#include <DbAdapters/AsyncDBWriter/AsyncDBWriter.h>`——目录名与头文件名**两处都错**，照抄编译不过（与本仓 2026-09-27 那批修过的 DbAdapters 仓 README 示例同源）。**未改**：用户本轮只点了 `DBAdapters` 与 `DBInterface` 两个 token，`AsyncDBWriter` 是第三个，待确认后一并改（另有 QuantTrading 8 处、DbAdapters 仓无）。
- `test/TestDB` 中 MySQL / MariaDB 测试默认注释关闭（需本地服务），如要常开可配置 CI。
