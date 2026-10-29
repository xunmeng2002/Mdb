#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <Spark/Core/Logger/Logger.h>

int main(int argc, char** argv)
{
	Spark::Core::Logger& logger = Spark::Core::Logger::GetInstance();
	logger.Init(argv[0]);
	logger.SetLogLevel(Spark::Core::LogLevel::Info, Spark::Core::LogLevel::Info);
	logger.Start();

	doctest::Context context(argc, argv);
	const int failedTestCount = context.run();

	logger.Stop();
	logger.Join();
	return failedTestCount;
}
