#include "Utility.h"
#include <iostream>
#include <cstring>


void ParseProcessName(const char* fullProcessName, char* processName, int len)
{
#if WINDOWS
	const char* temp = strrchr(fullProcessName, '\\');
	temp = temp == nullptr ? fullProcessName : ++temp;
	temp = strtok((char*)temp, ".");
#endif
#if LINUX
	const char* temp = strrchr(fullProcessName, '/');
	temp = temp == nullptr ? fullProcessName : ++temp;
#endif
	strncpy(processName, temp, len);
}

std::string ItoA(int value)
{
	static char buff[32];
	sprintf(buff, "%d", value);
	return std::string(buff);
}

std::string FtoA(double value)
{
	static char buff[32];
	sprintf(buff, "%f", value);
	return std::string(buff);
}

