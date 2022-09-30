#include "TimeUtility.h"
#include <iostream>
#include <ctime>
#include <time.h>
#include <sys/timeb.h>



time_t GetTime()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return std::chrono::system_clock::to_time_t(now);
}
tm* GetUtcTm()
{
	time_t t = GetTime();
	return gmtime(&t);
}
tm* GetLocalTm()
{
	time_t t = GetTime();
	return localtime(&t);
}
std::string GetUtcDateTime()
{
	auto t = GetTime();
	auto localTm = gmtime(&t);
	char buff[32];
	strftime(buff, 32, "%Y%m%d-%H:%M:%S", localTm);
	return std::string(buff);
}
std::string GetUtcDate()
{
	auto t = GetTime();
	auto localTm = gmtime(&t);
	char buff[32];
	strftime(buff, 32, "%Y%m%d", localTm);
	return std::string(buff);
}
std::string GetUtcTime()
{
	auto t = GetTime();
	auto localTm = gmtime(&t);
	char buff[32];
	strftime(buff, 32, "%H:%M:%S", localTm);
	return std::string(buff);
}
std::string GetUtcDateTimeWithMilliSecond()
{
	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	time_t t = std::chrono::system_clock::to_time_t(now);
	int milliSecond = now.time_since_epoch().count() % 1000;
	auto localTm = gmtime(&t);
	char buff[32];
	auto len = strftime(buff, 32, "%Y%m%d-%H:%M:%S", localTm);
	sprintf(buff + len, ".%03u", milliSecond);
	return std::string(buff);
}
std::string GetLocalDateTime()
{
	auto t = GetTime();
	auto localTm = localtime(&t);
	char buff[32];
	strftime(buff, 32, "%Y%m%d-%H:%M:%S", localTm);
	return std::string(buff);
}
std::string GetLocalDate()
{
	auto t = GetTime();
	auto localTm = localtime(&t);
	char buff[32];
	strftime(buff, 32, "%Y%m%d", localTm);
	return std::string(buff);
}
std::string GetLocalTime()
{
	auto t = GetTime();
	auto localTm = localtime(&t);
	char buff[32];
	strftime(buff, 32, "%H:%M:%S", localTm);
	return std::string(buff);
}
std::string GetLocalDateTimeWithMilliSecond()
{
	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	time_t t = std::chrono::system_clock::to_time_t(now);
	int milliSecond = now.time_since_epoch().count() % 1000;
	auto localTm = localtime(&t);
	char buff[32];
	auto len = strftime(buff, 32, "%Y%m%d-%H:%M:%S", localTm);
	sprintf(buff + len, ".%03u", milliSecond);
	return std::string(buff);
}

std::string GetLocalDateFromUnixTimeStamp(long long timeStamp)
{
	time_t time = timeStamp / 1000000000LL;
	static char buff[16];
	auto len = strftime(buff, 16, "%Y%m%d", localtime(&time));
	return std::string(buff);
}
std::string GetLocalTimeFromUnixTimeStamp(long long timeStamp)
{
	time_t time = timeStamp / 1000000000LL;
	static char buff[16];
	auto len = strftime(buff, 16, "%H:%M:%S", localtime(&time));
	return std::string(buff);
}

time_t GetTimeFromString(std::string dateTime, std::string format)
{
	tm t;
	int len = sscanf(dateTime.c_str(), format.c_str(), &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
	t.tm_year -= 1900;
	t.tm_mon -= 1;

	return mktime(&t);
}
std::string ToUtcDateTime(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%Y%m%d-%H:%M:%S", gmtime(time));
	return std::string(buff);
}
std::string ToUtcDate(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%Y%m%d", gmtime(time));
	return std::string(buff);
}
std::string ToUtcTime(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%H:%M:%S", gmtime(time));
	return std::string(buff);
}
std::string ToLocalDateTime(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%Y%m%d-%H:%M:%S", localtime(time));
	return std::string(buff);
}
std::string ToLocalDate(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%Y%m%d", localtime(time));
	return std::string(buff);
}
std::string ToLocalTime(time_t* time)
{
	char buff[32];
	strftime(buff, 32, "%H:%M:%S", localtime(time));
	return std::string(buff);
}
