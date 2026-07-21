cmake_minimum_required(VERSION 3.20)
set(VcpkgPth "D:/Github/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VcpkgPth}/scripts/buildsystems/vcpkg.cmake")
SET(PROJECT_NAME "Mdb")
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(submodules/CMakeCommon/CMakeCommon.cmake)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions(-DUNICODE -D_UNICODE)


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj /utf-8")
    add_definitions(-DWINDOWS -D_CRT_SECURE_NO_WARNINGS -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
	add_compile_options(
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/Zi>
        $<$<CONFIG:MinSizeRel>:/O1>)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
	add_definitions("-DLINUX")
    add_compile_options(
        $<$<CONFIG:Debug>:-O0 -g -ggdb>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:MinSizeRel>:-Os>)
    add_compile_definitions(
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<CONFIG:RelWithDebInfo>:NDEBUG>
        $<$<CONFIG:MinSizeRel>:NDEBUG>)
ENDIF()
message("CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message("CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
message("CMAKE_CL_64:" ${CMAKE_CL_64})
message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_CXX_FLAGS_RELEASE:" ${CMAKE_CXX_FLAGS_RELEASE})
message("CMAKE_CXX_FLAGS_DEBUG:" ${CMAKE_CXX_FLAGS_DEBUG})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/$<CONFIG>)
 
message(STATUS "VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
add_template_library(DBInterface		${CMAKE_CURRENT_SOURCE_DIR}/include)
add_shared_module(Mdb				    ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/Mdb)
add_shared_module(AsyncDBWriter			${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/AsyncDBWriter)
add_shared_module(MysqlWrapper			${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/MysqlWrapper)
add_shared_module(MariadbWrapper		${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/MariadbWrapper)
add_shared_module(DuckdbWrapper			${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/DuckdbWrapper)
add_shared_module(SqliteWrapper			${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Mdb/SqliteWrapper)

find_package(PersonalLib REQUIRED PATHS "../Libs/PersonalLib")
find_package(duckdb REQUIRED PATHS "../Libs/duckdb")
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
find_package(unofficial-mariadb-connector-cpp CONFIG REQUIRED)



target_link_libraries(AsyncDBWriter PRIVATE PersonalLib::Core)
target_link_libraries(Mdb PUBLIC AsyncDBWriter DBInterface PersonalLib::Core PersonalLib::Serialization)
target_link_libraries(DuckdbWrapper PUBLIC DBInterface duckdb)
target_link_libraries(SqliteWrapper PUBLIC DBInterface unofficial::sqlite3::sqlite3)
target_link_libraries(MysqlWrapper PUBLIC DBInterface unofficial::mysql-connector-cpp::connector)
target_link_libraries(MariadbWrapper PUBLIC DBInterface unofficial::mariadb-connector-cpp::mariadbcpp)

# libmariadb 运行时动态加载认证插件（非 PE 依赖，无法通过 vcpkg applocal 自动部署）
# 编译时注入插件路径，Connect() 内设环境变量 MARIADB_PLUGIN_DIR 告知 libmariadb
target_compile_definitions(MariadbWrapper PRIVATE
  MARIADB_PLUGIN_DIR="${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/$<IF:$<CONFIG:Debug>,debug/,>plugins/libmariadb"
)

add_subdirectory(test)
