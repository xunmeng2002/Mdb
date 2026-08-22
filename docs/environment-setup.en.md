# Environment Setup Guide

> This document covers both **Windows (MSVC)** and **WSL (GCC)** platforms, detailing every step required to set up the build environment from scratch.

> **Dependency overview (Mdb)**: besides the generic toolchain below, three prebuilt dependencies are required — the **Spark** foundational library, the **DBAdapters** database access layer, and **DuckDB** (installed under the project's parent directory at `../Libs/Spark/<triplet>`, `../Libs/DBAdapters/<triplet>`, and `../Libs/duckdb/<triplet>`), plus the `sqlite3`, `mysql-connector-cpp`, and `mariadb-connector-cpp` drivers provided by vcpkg. See the "Environment & Dependencies" section of the [README](../README.en.md) for details.

---

## Table of Contents

- [Environment Setup Guide](#environment-setup-guide)
  - [Table of Contents](#table-of-contents)
  - [1. Windows (MSVC)](#1-windows-msvc)
    - [1.1 System Requirements](#11-system-requirements)
    - [1.2 Visual Studio 2022](#12-visual-studio-2022)
    - [1.3 Git](#13-git)
    - [1.4 Proxy Configuration (Optional but Recommended)](#14-proxy-configuration-optional-but-recommended)
    - [1.5 vcpkg](#15-vcpkg)
    - [1.6 Verification](#16-verification)
  - [2. WSL (GCC)](#2-wsl-gcc)
    - [2.1 System Requirements](#21-system-requirements)
    - [2.2 WSL Setup](#22-wsl-setup)
    - [2.3 Build Toolchain](#23-build-toolchain)
    - [2.4 vcpkg](#24-vcpkg)
    - [2.5 Proxy Configuration (Optional)](#25-proxy-configuration-optional)
    - [2.6 Verification](#26-verification)
  - [3. Troubleshooting](#3-troubleshooting)
    - [3.1 vcpkg Timeout / Cannot Connect to GitHub](#31-vcpkg-timeout--cannot-connect-to-github)
    - [3.2 VS Says "Could not find Spark" / "Could not find DBAdapters" / "Could not find duckdb"](#32-vs-says-could-not-find-spark--could-not-find-dbadapter--could-not-find-duckdb)
    - [3.3 cmake cannot find Ninja in WSL](#33-cmake-cannot-find-ninja-in-wsl)
    - [3.4 Git errors after turning proxy off](#34-git-errors-after-turning-proxy-off)

---

## 1. Windows (MSVC)

### 1.1 System Requirements

- **OS**: Windows 10 21H2+ / Windows 11
- **Disk**: At least 20 GB free space

### 1.2 Visual Studio 2022

Install **Visual Studio 2022** (Enterprise or Community). Under **Workloads**, check:

- **Desktop development with C++**
  - MSVC v143 build tools (x64/x86)
  - Windows 10/11 SDK
  - C++ CMake tools for Windows
  - C++ ATL for latest v143 build tools (optional)

Reference installation path:

```
D:/Program Files/Microsoft Visual Studio/2022/Enterprise/
```

> If vcpkg is installed elsewhere, update the `VCPKG_ROOT` system environment variable to point to the actual installation directory.

### 1.3 Git

```bash
git --version  # Must be ≥ 2.30
```

Git can be installed via the Visual Studio installer or from [https://git-scm.com](https://git-scm.com).

### 1.4 Proxy Configuration (Optional but Recommended)

In mainland China, GitHub connectivity is often unreliable. Using a proxy tool (e.g., Clash Verge) is recommended.

**1.4.1 Configure Git Proxy**

```bash
# Set (adjust port to match your proxy tool)
git config --global http.proxy http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897

# Unset
git config --global --unset http.proxy
git config --global --unset https.proxy
```

**1.4.2 Set Environment Variables (Persistent)**

```bash
setx HTTP_PROXY "http://127.0.0.1:7897"
setx HTTPS_PROXY "http://127.0.0.1:7897"
setx http_proxy "http://127.0.0.1:7897"
setx https_proxy "http://127.0.0.1:7897"
```

Restart your terminal or Visual Studio after setting.

### 1.5 vcpkg

**Option A: System-wide installation (recommended)**

```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```

**Set the `VCPKG_ROOT` system environment variable (required):**

```bash
setx VCPKG_ROOT "D:/path/to/vcpkg"
```

Add `%VCPKG_ROOT%` to `PATH`, then **restart your terminal / Visual Studio** for the change to take effect.

> **Note**: The project's CMake locates vcpkg via `$ENV{VCPKG_ROOT}` (`set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")` in `CMakeLists.txt`). On Windows, `restore_vcpkg_root()` reads the **system environment variable** (User-level first, then Machine-level). So `VCPKG_ROOT` must be set as a system environment variable — **there is no longer a fixed fallback path**.

### 1.6 Verification

Open **Visual Studio**, select the `x64-Debug` configuration, and run CMake configuration (generate cache):

- The output should show vcpkg-related logs (downloading and building `sqlite3`, `mysql-connector-cpp`, and `mariadb-connector-cpp` with their transitive dependencies)
- After configuration, **Spark**, **DBAdapters**, **duckdb**, and all database drivers should be found successfully (no `Could not find ...` errors)
- Build should complete without errors, and `bin/Debug/TestDB.exe` should run

---

## 2. WSL (GCC)

### 2.1 System Requirements

- **OS**: Windows 10 21H2+ / Windows 11 (WSL2 enabled)
- **WSL version**: 2.0.0+
- **Linux distro**: Ubuntu 22.04 LTS (recommended)

### 2.2 WSL Setup

**2.2.1 Install WSL**

```bash
# Run PowerShell as Administrator
wsl --install -d Ubuntu

# Verify
wsl -l -v
#   NAME      STATE           VERSION
# * Ubuntu    Running         2
```

**2.2.2 Mirrored Networking Mode (Optional but Recommended)**

Makes WSL share the Windows network stack — localhost is unified and proxy settings are inherited automatically.

Create `%USERPROFILE%\.wslconfig` with:

```ini
[wsl2]
networkingMode=mirrored
dnsTunneling=true
autoProxy=true
```

Restart WSL:

```bash
wsl --shutdown
# Then restart wsl
```

### 2.3 Build Toolchain

Run these commands **inside WSL**:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build curl zip unzip tar pkg-config -y
```

| Package | Purpose |
|---------|---------|
| `build-essential` | g++, gcc, make, and other compilation tools |
| `cmake` | CMake build system |
| `ninja-build` | Ninja build system (the project's generator) |
| `curl`, `zip`, `unzip`, `tar` | vcpkg dependency download and extraction |
| `pkg-config` | Package configuration helper |

### 2.4 vcpkg

**Install vcpkg:**

```bash
cd ~
git clone https://github.com/microsoft/vcpkg ~/Github/vcpkg
./vcpkg/bootstrap-vcpkg.sh
```

**Set environment variables (persistent):**

```bash
echo 'export VCPKG_ROOT=~/Github/vcpkg' >> ~/.bashrc
echo 'export PATH=$VCPKG_ROOT:$PATH' >> ~/.bashrc
source ~/.bashrc
```

(Replace `.bashrc` with `.zshrc` if using zsh.)

**Verify:**

```bash
vcpkg --version
echo $VCPKG_ROOT  # Should output /home/xxx/Github/vcpkg
```

> **Note**: For WSL / Linux builds, CMake reads the `VCPKG_ROOT` shell environment variable directly (`restore_vcpkg_root()` only runs on Windows). Make sure the `export` above is in `~/.bashrc` and has been sourced.

### 2.5 Proxy Configuration (Optional)

If mirrored networking mode is not enabled, configure the proxy manually inside WSL:

```bash
# Git proxy
git config --global http.proxy http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897

# Environment variables
echo 'export http_proxy=http://127.0.0.1:7897' >> ~/.bashrc
echo 'export https_proxy=http://127.0.0.1:7897' >> ~/.bashrc
source ~/.bashrc
```

### 2.6 Verification

Run CMake configuration inside WSL:

```bash
cd /mnt/d/Gitee/Mdb
cmake -S . -B out/build/WSL-GCC-Debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -G Ninja
```

Alternatively, use **Visual Studio**'s CMake integration — select the `WSL-GCC-Debug` configuration and generate the cache.

---

## 3. Troubleshooting

### 3.1 vcpkg Timeout / Cannot Connect to GitHub

- Make sure your proxy tool is running
- Check `git config --global http.proxy` is set correctly
- The vcpkg downloader only reads the `HTTP_PROXY` / `HTTPS_PROXY` environment variables; the project's CMake calls `reuse_git_proxy_for_vcpkg()` to automatically reuse your git proxy config when those variables are unset, so no extra action is usually needed
- If it still fails, set `http_proxy` / `https_proxy` manually

### 3.2 VS Says "Could not find Spark" / "Could not find DBAdapters" / "Could not find duckdb"

- Verify **Spark**, **DBAdapters**, and **duckdb** are installed under the project's parent `../Libs/` directory for the target platform (Windows: `../Libs/Spark/x64-windows`, `../Libs/DBAdapters/x64-windows`, `../Libs/duckdb/x64-windows`; Linux / WSL: `x64-linux`)
- Verify the install directory contains the corresponding `*-config.cmake` (generated by running `cmake --install` in the respective repository)
- Verify vcpkg is installed and `VCPKG_ROOT` is set as a **system environment variable** (`restore_vcpkg_root()` reads the User/Machine level variable)
- In VS, select **Project → Delete Cache and Regenerate**, or delete the `out/` directory and reconfigure

### 3.3 cmake cannot find Ninja in WSL

```bash
sudo apt install ninja-build
```

### 3.4 Git errors after turning proxy off

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```
