SET(PROJECT_NAME "TestMariaDB")
project(${PROJECT_NAME})
message("PROJECT_NAME:" ${PROJECT_NAME})


add_package(Src ./Src)
add_package(DBOperate ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/DBOperate)

add_package(MariaDB ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/MariaDB)
add_package(InitMdb ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/InitMdb)
add_package(Mdb ${CMAKE_SOURCE_DIR}/Source/SimExchangeDB/Mdb)

include_package(duckdb ${CMAKE_SOURCE_DIR}/../Libs/MariaDB/1.1.5/include/mariadb)
link_directories(${CMAKE_SOURCE_DIR}/../Libs/MariaDB/1.1.5/win64)

add_executable(${PROJECT_NAME} ${all_file})
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	#target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/MT> $<$<CONFIG:Debug>:/MTd>)
	target_link_libraries(${PROJECT_NAME} PersonalLib.lib mariadbcpp.lib)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${PROJECT_NAME} PersonalLib pthread dl mariadbcpp)
endif()

