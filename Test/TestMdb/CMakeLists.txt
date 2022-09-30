SET(PROJECT_NAME "TestMdb")
project(${PROJECT_NAME})
message("PROJECT_NAME:" ${PROJECT_NAME})

add_package(Src ./Src)
add_package(Utility ${CMAKE_SOURCE_DIR}/Source/Utility)
add_package(MemCache ${CMAKE_SOURCE_DIR}/Source/MemCache)
add_package(DataStruct ${CMAKE_SOURCE_DIR}/Source/DataStruct)
add_package(Index ${CMAKE_SOURCE_DIR}/Source/Index)
add_package(Mdb ${CMAKE_SOURCE_DIR}/Source/Mdb)


add_executable(${PROJECT_NAME} ${all_file})
