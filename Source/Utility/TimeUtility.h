#pragma once

#include <chrono>
#include <string>


time_t GetTime();
tm* GetUtcTm();
tm* GetLocalTm();
std::string GetUtcDateTime();
std::string GetUtcDate();
std::string GetUtcTime();
std::string GetUtcDateTimeWithMilliSecond();
std::string GetLocalDateTime();
std::string GetLocalDate();
std::string GetLocalTime();
std::string GetLocalDateTimeWithMilliSecond();

std::string GetLocalDateFromUnixTimeStamp(long long timeStamp);
std::string GetLocalTimeFromUnixTimeStamp(long long timeStamp);

time_t GetTimeFromString(std::string dateTime, std::string format = "%04d%02d%02d-%02d:%02d:%02d");
std::string ToUtcDateTime(time_t* time);
std::string ToUtcDate(time_t* time);
std::string ToUtcTime(time_t* time);
std::string ToLocalDateTime(time_t* time);
std::string ToLocalDate(time_t* time);
std::string ToLocalTime(time_t* time);


template<typename T, typename T2>
long long GetDuration(T2& start, T2& end)
{
	return std::chrono::duration_cast<T>(end - start).count();
}
