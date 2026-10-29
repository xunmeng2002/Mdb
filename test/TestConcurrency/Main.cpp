#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "OperationInterval.h"

#include <cstdio>
#include <cstdlib>

std::chrono::microseconds& ConfiguredOperationInterval()
{
	static std::chrono::microseconds interval{ 10 };
	return interval;
}

int main(int argc, char** argv)
{
	std::setvbuf(stdout, nullptr, _IONBF, 0);
	// 裸数字才是操作间隔；以 '-' 开头的一律留给 doctest 解析
	if (argc > 1 && argv[1][0] != '-')
	{
		const long long microseconds = std::strtoll(argv[1], nullptr, 10);
		ConfiguredOperationInterval() = std::chrono::microseconds{ microseconds < 0 ? 0 : microseconds };
	}

	doctest::Context context(argc, argv);
	return context.run();
}
