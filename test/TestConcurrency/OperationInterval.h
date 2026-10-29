#pragma once

#include <chrono>

// 读写两端的操作间隔：模拟真实业务里两次操作之间的计算与 IO，读者不再把共享锁占满。
// 命令行第一个参数给微秒数，缺省 10 µs；给 0 即回到「满载自旋」的饱和最坏情况。
std::chrono::microseconds& ConfiguredOperationInterval();
