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


add_static_md_module(Mdb ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb)
target_link_libraries(MdbStatic PUBLIC Spark::Core DbAdapters::DbInterface)
target_link_libraries(MdbStatic PRIVATE Spark::TemplateLib)

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
