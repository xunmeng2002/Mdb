# 环境准备指南

> 本文档面向 **Windows（MSVC）** 和 **WSL（GCC）** 两个平台，说明从零搭建项目编译环境所需的全部步骤。

> **项目依赖概览（Mdb）**：除下述通用工具链外，还需准备 **Spark** 基础库、**DBAdapters** 数据库访问层与 **DuckDB** 三个预编译依赖（分别安装到项目父目录 `../Libs/Spark/<triplet>`、`../Libs/DBAdapters/<triplet>`、`../Libs/duckdb/<triplet>`），并由 vcpkg 提供 `sqlite3`、`mysql-connector-cpp`、`mariadb-connector-cpp` 三个数据库驱动。完整说明见仓库根目录 [README](../README.md) 的"环境依赖"章节。

---

## 目录

- [环境准备指南](#环境准备指南)
  - [目录](#目录)
  - [一、Windows（MSVC）](#一windowsmsvc)
    - [1.1 系统要求](#11-系统要求)
    - [1.2 Visual Studio 2022](#12-visual-studio-2022)
    - [1.3 Git](#13-git)
    - [1.4 代理配置（可选但推荐）](#14-代理配置可选但推荐)
    - [1.5 vcpkg](#15-vcpkg)
    - [1.6 验证](#16-验证)
  - [二、WSL（GCC）](#二wslgcc)
    - [2.1 系统要求](#21-系统要求)
    - [2.2 WSL 安装与配置](#22-wsl-安装与配置)
    - [2.3 构建工具链](#23-构建工具链)
    - [2.4 vcpkg](#24-vcpkg)
    - [2.5 代理配置（可选）](#25-代理配置可选)
    - [2.6 验证](#26-验证)
  - [三、常见问题](#三常见问题)
    - [3.1 vcpkg 拉取超时 / 无法连接 GitHub](#31-vcpkg-拉取超时--无法连接-github)
    - [3.2 VS 提示 "Could not find Spark" / "Could not find DBAdapters" / "Could not find duckdb"](#32-vs-提示-could-not-find-spark--could-not-find-dbadapter--could-not-find-duckdb)
    - [3.3 WSL 中 cmake 找不到 Ninja](#33-wsl-中-cmake-找不到-ninja)
    - [3.4 代理关闭后 Git 报错](#34-代理关闭后-git-报错)

---

## 一、Windows（MSVC）

### 1.1 系统要求

- **操作系统**：Windows 10 21H2+ / Windows 11
- **磁盘空间**：至少 20 GB 可用空间

### 1.2 Visual Studio 2022

安装 **Visual Studio 2022**（Enterprise 或 Community 均可），工作负载勾选：

- **使用 C++ 的桌面开发**
  - MSVC v143 生成工具（x64/x86）
  - Windows 10/11 SDK
  - C++ CMake 工具（适用于 Windows）
  - 适用于最新 v143 生成工具的 C++ ATL（可选）

安装路径（参考）：

```
D:/Program Files/Microsoft Visual Studio/2022/Enterprise/
```

> 如果 vcpkg 安装在其他位置，请同步更新 `VCPKG_ROOT` 系统环境变量，使其指向实际安装目录。

### 1.3 Git

```bash
git --version  # 需 ≥ 2.30
```

Git 随 Visual Studio 安装时可选，也可从 [https://git-scm.com](https://git-scm.com) 单独安装。

### 1.4 代理配置（可选但推荐）

在中国大陆网络环境下，GitHub 访问不稳定，推荐使用代理工具（如 Clash Verge）。

**1.4.1 配置 Git 代理**

```bash
# 设置（根据你的代理端口修改）
git config --global http.proxy http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897

# 取消
git config --global --unset http.proxy
git config --global --unset https.proxy
```

**1.4.2 配置环境变量（持久化）**

```bash
setx HTTP_PROXY "http://127.0.0.1:7897"
setx HTTPS_PROXY "http://127.0.0.1:7897"
setx http_proxy "http://127.0.0.1:7897"
setx https_proxy "http://127.0.0.1:7897"
```

设置后需重启终端或 Visual Studio。

### 1.5 vcpkg

**方案一：系统级安装（推荐）**

```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```

**设置 `VCPKG_ROOT` 系统环境变量（必需）：**

```bash
setx VCPKG_ROOT "D:/path/to/vcpkg"
```

将 `%VCPKG_ROOT%` 添加到 `PATH`，并**重启终端 / Visual Studio** 使环境变量生效。

> **注意**：项目 CMake 通过 `$ENV{VCPKG_ROOT}` 定位 vcpkg（`CMakeLists.txt` 中 `set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")`），Windows 下由 `restore_vcpkg_root()` 自动读取**系统环境变量**（优先 User 级，其次 Machine 级）。因此 `VCPKG_ROOT` 必须以系统环境变量形式设置，**不再支持固定的回退路径**。

### 1.6 验证

打开 **Visual Studio**，选择 `x64-Debug` 配置，执行 CMake 配置（生成 cache）：

- 控制台应输出 vcpkg 相关日志（自动安装 `sqlite3`、`mysql-connector-cpp`、`mariadb-connector-cpp` 及其传递依赖）
- 配置完成后能正确找到 **Spark**、**DBAdapters**、**duckdb** 与各数据库驱动（无 `Could not find ...` 报错）
- 构建无报错，`bin/Debug/TestDB.exe` 可正常运行

---

## 二、WSL（GCC）

### 2.1 系统要求

- **操作系统**：Windows 10 21H2+ / Windows 11（已启用 WSL2）
- **WSL 版本**：2.0.0+
- **Linux 发行版**：Ubuntu 22.04 LTS（推荐）

### 2.2 WSL 安装与配置

**2.2.1 安装 WSL**

```bash
# 以管理员身份运行 PowerShell
wsl --install -d Ubuntu

# 确认版本
wsl -l -v
#   NAME      STATE           VERSION
# * Ubuntu    Running         2
```

**2.2.2 镜像网络模式（可选但推荐）**

使 WSL 共享 Windows 的网络栈，localhost 互通，自动继承代理设置。

在 `%USERPROFILE%\.wslconfig` 中写入：

```ini
[wsl2]
networkingMode=mirrored
dnsTunneling=true
autoProxy=true
```

重启 WSL：

```bash
wsl --shutdown
# 然后重新运行 wsl
```

### 2.3 构建工具链

在 WSL 终端中执行：

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build curl zip unzip tar pkg-config -y
```

| 包名 | 作用 |
|------|------|
| `build-essential` | g++、gcc、make 等编译工具链 |
| `cmake` | CMake 构建系统 |
| `ninja-build` | Ninja 构建系统（项目使用的生成器） |
| `curl`、`zip`、`unzip`、`tar` | vcpkg 下载和解压依赖 |
| `pkg-config` | 包配置工具 |

### 2.4 vcpkg

**安装 vcpkg：**

```bash
cd ~
git clone https://github.com/microsoft/vcpkg ~/Github/vcpkg
./vcpkg/bootstrap-vcpkg.sh
```

**设置环境变量（持久化）：**

```bash
echo 'export VCPKG_ROOT=~/Github/vcpkg' >> ~/.bashrc
echo 'export PATH=$VCPKG_ROOT:$PATH' >> ~/.bashrc
source ~/.bashrc
```

（如果使用 zsh，将 `.bashrc` 替换为 `.zshrc`）

**验证：**

```bash
vcpkg --version
echo $VCPKG_ROOT  # 应输出 /home/xxx/Github/vcpkg
```

> **注意**：WSL / Linux 构建时，CMake 直接读取 shell 环境变量 `VCPKG_ROOT`（`restore_vcpkg_root()` 仅在 Windows 生效），请确保上述 `export` 已写入 `~/.bashrc` 并 `source` 生效。

### 2.5 代理配置（可选）

如果未启用镜像网络模式，需在 WSL 中手动配置代理：

```bash
# Git 代理
git config --global http.proxy http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897

# 环境变量
echo 'export http_proxy=http://127.0.0.1:7897' >> ~/.bashrc
echo 'export https_proxy=http://127.0.0.1:7897' >> ~/.bashrc
source ~/.bashrc
```

### 2.6 验证

在 WSL 终端直接运行 CMake 配置：

```bash
cd /mnt/d/Gitee/Mdb
cmake -S . -B out/build/WSL-GCC-Debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -G Ninja
```

或使用 **Visual Studio** 的 CMake 集成，选择 `WSL-GCC-Debug` 配置后生成 cache。

---

## 三、常见问题

### 3.1 vcpkg 拉取超时 / 无法连接 GitHub

- 确认代理工具已开启
- 检查 `git config --global http.proxy` 是否设置正确
- vcpkg 下载器只读取 `HTTP_PROXY` / `HTTPS_PROXY` 环境变量；项目 CMake 已通过 `reuse_git_proxy_for_vcpkg()` 在未设置环境变量时自动复用 git 代理配置，通常无需额外处理
- 若仍未生效，请手动设置 `http_proxy` / `https_proxy` 环境变量

### 3.2 VS 提示 "Could not find Spark" / "Could not find DBAdapters" / "Could not find duckdb"

- 确认 **Spark**、**DBAdapters** 与 **duckdb** 已安装到项目父目录 `../Libs/` 下对应的平台路径（Windows：`../Libs/Spark/x64-windows`、`../Libs/DBAdapters/x64-windows`、`../Libs/duckdb/x64-windows`；Linux / WSL：`x64-linux`）
- 确认安装目录内存在对应的 `*-config.cmake`（由相应仓库执行 `cmake --install` 生成）
- 确认 vcpkg 已安装，且 `VCPKG_ROOT` 已设置为**系统环境变量**（`restore_vcpkg_root()` 会从 User / Machine 级环境变量读取）
- 在 VS 中选择 **项目 → 清除缓存并重新生成**，或删除 `out/` 目录后重新配置

### 3.3 WSL 中 cmake 找不到 Ninja

```bash
sudo apt install ninja-build
```

### 3.4 代理关闭后 Git 报错

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```
