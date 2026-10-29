# 可执行文件用 Bench 前缀，与 Test* 一起在 bin/<Config> 排在一处。
# 计时器复用 Spark 的公开头 <Spark/TemplateLib/Benchmark/BenchmarkRunner.h>，CSV 口径与兄弟仓一致。
add_application(BenchTableOps ${CMAKE_CURRENT_SOURCE_DIR}/BenchTableOps ${CMAKE_CURRENT_SOURCE_DIR}/BenchTableOps)
target_link_libraries(BenchTableOps PRIVATE MdbLockBasedStatic MdbRcuPointerSwapStatic MdbRcuStableRowStatic Spark::Core Threads::Threads)
target_include_directories(BenchTableOps PRIVATE ${CMAKE_SOURCE_DIR}/test)
copy_dll_file(BenchTableOps)
