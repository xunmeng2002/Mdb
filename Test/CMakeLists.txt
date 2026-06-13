
add_application(TestDB         ${CMAKE_CURRENT_SOURCE_DIR}/TestDB ${CMAKE_CURRENT_SOURCE_DIR}/TestDB)
target_link_libraries(TestDB PRIVATE Mdb Mysql Mariadb Duckdb Sqlite)

find_package(ZLIB REQUIRED)
message(STATUS "ZLIB::ZLIB ZLIB_INCLUDE_DIRS:${ZLIB_INCLUDE_DIRS}, ZLIB_LIBRARIES:${ZLIB_LIBRARIES}, ZLIB_VERSION:${ZLIB_VERSION}, ZLIB_FOUND:${ZLIB_FOUND}")

#copy_dll_file(TestDB)
#copy_dll_file_with_path(TestDB ${LZ4_DLL_DEBUG} ${LZ4_DLL_RELEASE})
#copy_zlib_dll_file(TestDB ${ZLIB_INCLUDE_DIRS})