SET(PROJECT_NAME "TestDB")
project(${PROJECT_NAME})
message("PROJECT_NAME:" ${PROJECT_NAME})


add_package(Src ./Src)
add_package(DBOperate ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/DBOperate)

add_package(MysqlDB ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/MysqlDB)
add_package(SqliteDB ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/SqliteDB)
add_package(DuckDB ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/DuckDB)
add_package(InitMdb ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/InitMdb)
add_package(Mdb ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/Mdb)

include_package(MysqlConnector ${CMAKE_SOURCE_DIR}/../Libs/MysqlConnector/9.0.0/include)
include_package(duckdb ${CMAKE_SOURCE_DIR}/../Libs/duckdb/include)

link_directories(${CMAKE_SOURCE_DIR}/../Libs/MysqlConnector/9.0.0)
link_directories(${CMAKE_SOURCE_DIR}/../Libs/MysqlConnector/9.0.0/Debug)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	link_directories(${CMAKE_SOURCE_DIR}/../Libs/duckdb/win64/Debug)
else(CMAKE_BUILD_TYPE MATCHES "Debug")
	link_directories(${CMAKE_SOURCE_DIR}/../Libs/duckdb/win64/Release)
endif()

add_executable(${PROJECT_NAME} ${all_file})
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(${PROJECT_NAME} PersonalLib.lib mysqlcppconn.lib mysqlcppconnx.lib libcrypto.lib libssl.lib duckdb.lib)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${PROJECT_NAME} PersonalLib pthread dl mysqlcppconnx duckdb.lib)
endif()

