cmake_minimum_required(VERSION 3.20)
include(submodules/CMakeCommon/CMakeCommon.cmake)
restore_vcpkg_root()
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
message(STATUS "ENV{VCPKG_ROOT} = $ENV{VCPKG_ROOT}")
message(STATUS "ENV{VCPKG_DOWNLOADS} = $ENV{VCPKG_DOWNLOADS}")
message(STATUS "ENV{VCPKG_DEFAULT_BINARY_CACHE} = $ENV{VCPKG_DEFAULT_BINARY_CACHE}")
message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
SET(PROJECT_NAME "Mdb")
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)
include(GenerateExportHeader)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_VERBOSE_MAKEFILE ON)
# 平台/配置相关编译选项：MSVC 的 /EHsc 及各配置优化级别、Linux 的 -g/-O 系、
# NDEBUG/_DEBUG 均为 CMake+编译器默认行为，不再显式重复；仅保留净增量项
if(WIN32)
    add_compile_definitions(UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
    add_compile_options(/utf-8 /bigobj)
endif()
message("CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message("CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
message("CMAKE_CL_64:" ${CMAKE_CL_64})
message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_CXX_FLAGS_RELEASE:" ${CMAKE_CXX_FLAGS_RELEASE})
message("CMAKE_CXX_FLAGS_DEBUG:" ${CMAKE_CXX_FLAGS_DEBUG})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/$<CONFIG>)

set(VCPKG_APPLOCAL_DEPS ON)

message(STATUS "VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")

if(WIN32)
    find_package(Spark REQUIRED PATHS "../Libs/Spark/x64-windows")
    find_package(DbAdapters CONFIG REQUIRED PATHS "../Libs/DbAdapters/x64-windows")
    find_package(duckdb CONFIG REQUIRED PATHS "../Libs/duckdb/x64-windows")
else()
    find_package(Spark REQUIRED PATHS "../Libs/Spark/x64-linux")
    find_package(DbAdapters CONFIG REQUIRED PATHS "../Libs/DbAdapters/x64-linux")
    find_package(duckdb CONFIG REQUIRED PATHS "../Libs/duckdb/x64-linux")
endif()


find_package(Threads REQUIRED)

# 三种表实现拆成独立库，共用数据契约（MdbStructs / MdbTableBase / MdbReadLease / 比较器）。
# 允许同进程共存：读写锁版在 Mdb，另两个在 Mdb::Rcu::PointerSwap / Mdb::Rcu::StableRow。
# 一个库 = 一个文件夹：目录内的 .cpp 全归该库，新增文件无需改 CMake
function(add_mdb_static_library target_name source_dir)
    file(GLOB source_files CONFIGURE_DEPENDS "${source_dir}/*.cpp")
    add_library(${target_name} STATIC ${source_files})
    set_target_properties(${target_name} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        POSITION_INDEPENDENT_CODE ON)
    target_include_directories(${target_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb>)
endfunction()

set(MdbSourceDir ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb)

add_mdb_static_library(MdbModelStatic ${MdbSourceDir}/Model)
target_link_libraries(MdbModelStatic PUBLIC DbAdapters::DbInterface Spark::Core)
target_link_libraries(MdbModelStatic PRIVATE Spark::TemplateLib)

add_mdb_static_library(MdbLockBasedStatic ${MdbSourceDir}/LockBased)
target_link_libraries(MdbLockBasedStatic PUBLIC MdbModelStatic)
target_link_libraries(MdbLockBasedStatic PRIVATE Spark::Serialization)

add_mdb_static_library(MdbRcuCoreStatic ${MdbSourceDir}/Rcu)
target_link_libraries(MdbRcuCoreStatic PUBLIC MdbModelStatic Threads::Threads)

add_mdb_static_library(MdbRcuPointerSwapStatic ${MdbSourceDir}/Rcu/PointerSwap)
target_link_libraries(MdbRcuPointerSwapStatic PUBLIC MdbRcuCoreStatic)
target_link_libraries(MdbRcuPointerSwapStatic PRIVATE Spark::Serialization)

add_mdb_static_library(MdbRcuStableRowStatic ${MdbSourceDir}/Rcu/StableRow)
target_link_libraries(MdbRcuStableRowStatic PUBLIC MdbRcuCoreStatic)
target_link_libraries(MdbRcuStableRowStatic PRIVATE Spark::Serialization)

# 旧名字保留为聚合目标，现有消费方（TestMdb）不用改
add_library(MdbStatic INTERFACE)
target_link_libraries(MdbStatic INTERFACE MdbModelStatic MdbLockBasedStatic)

# 测试可执行文件
add_application(TestMdb  ${CMAKE_CURRENT_SOURCE_DIR}/test/TestMdb ${CMAKE_CURRENT_SOURCE_DIR}/test/TestMdb)
target_link_libraries(TestMdb PRIVATE
    MdbStatic
    DbAdapters::DbInterface
    DbAdapters::AsyncDbWriter
    DbAdapters::SqliteWrapper
    DbAdapters::DuckdbWrapper
    DbAdapters::MysqlWrapper
    DbAdapters::MariadbWrapper
    Spark::Core
    Spark::Serialization)
copy_dll_file(TestMdb)

# RCU 形态 demo：自包含，只依赖标准库
# DemoRcu 为 B2（行地址稳定 + seqlock，含二级索引）；DemoRcuB 为 B（换指针 + 代数回收，仅主键）
add_application(DemoRcu ${CMAKE_CURRENT_SOURCE_DIR}/test/DemoRcu ${CMAKE_CURRENT_SOURCE_DIR}/test/DemoRcu)
target_link_libraries(DemoRcu PRIVATE Threads::Threads)
add_application(DemoRcuB ${CMAKE_CURRENT_SOURCE_DIR}/test/DemoRcuB ${CMAKE_CURRENT_SOURCE_DIR}/test/DemoRcuB)
target_link_libraries(DemoRcuB PRIVATE Threads::Threads)

# RCU 形态接入 Mdb 接口的对照测试：同一套场景分别跑 PointerSwap（B）与 StableRow（B2）
add_application(TestRcuTables ${CMAKE_CURRENT_SOURCE_DIR}/test/TestRcuTables ${CMAKE_CURRENT_SOURCE_DIR}/test/TestRcuTables)
target_link_libraries(TestRcuTables PRIVATE MdbModelStatic MdbRcuPointerSwapStatic MdbRcuStableRowStatic Threads::Threads)
copy_dll_file(TestRcuTables)
