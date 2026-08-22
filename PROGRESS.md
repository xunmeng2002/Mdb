# Mdb 进度记录

> 按项目治理规则维护：每次会话结束更新本文件，下次会话开始先读取。

## ✅ 已完成

- **中英文 README 重写**：原 `README.md` / `README.en.md` 已过期（目录结构错误、API 示例错误、CMake 3.15、缺 DBAdapters 依赖），已按实际代码重写为与 Spark / DBAdapters 一致的风格，涵盖项目简介、核心特性、11 张金融数据表模型、目录结构、环境依赖（Spark + DBAdapters + duckdb 预编译、vcpkg 三驱动、CMakeCommon 子模块）、Presets 快速构建、使用示例（完整接线 / 主键索引查询 / 更新删除清表 / InitMdbFromDB·Csv·Dump）、Python 脚本、测试程序、许可证、补充说明。
- **项目定位修正（用户澄清）**：Mdb 是"如何实现并使用内存数据库"的**实现示例（Demo）**，内置的数据库表与业务字段**不是重点**。已据此调整两份 README 的开篇、项目简介、第三节（改为"示例数据表模型"）、适用场景与补充说明，强调架构与实现方法，弱化金融业务语义。
- **环境准备文档修正**：将 Mdb 下 `docs/environment-setup.md` / `environment-setup.en.md` 中残留的 Spark 内容替换为 Mdb 专属：依赖概览（三个预编译依赖）、1.6 验证（驱动 + Spark/DBAdapters/duckdb + TestDB.exe）、2.6 WSL 路径（`cd /mnt/d/Gitee/Mdb`，去掉无 install 规则的 install prefix）、3.2 FAQ（Could not find Spark/DBAdapters/duckdb）、TOC 锚点同步修正。

## 🔄 进行中

- 无

## ❓ 待讨论 / 待决策

- README 落款沿用 Spark 的 "Created by [Fireseeker]"（Mdb LICENSE 同为 xunmeng2002）——如需调整署名请告知。
- Mdb 构建依赖 Spark、DBAdapters、duckdb 三个**预编译库**（`../Libs/<name>/<triplet>`），`vcpkg.json` 未声明三者；若考虑可复现构建，可讨论是否将 duckdb 纳入 vcpkg 管理。
- Mdb 的 `CMakeLists.txt` **没有 install() 规则**，但 `CMakePresets.json` 中 `CMAKE_INSTALL_PREFIX` 指向 `../Libs/DBAdapters/<triplet>`（从 DBAdapters 复制粘贴的遗留）。文档 2.6 已去掉 install prefix 以匹配现状；是否补全 install 规则、修正 preset 待定。
- `test/TestDB` 中 MySQL / MariaDB 测试默认注释关闭（需本地服务），如要常开可配置 CI。
