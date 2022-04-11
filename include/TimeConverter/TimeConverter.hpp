#pragma once
#include<chrono>

struct TimePoint
{
	unsigned int year;
	unsigned int mounth;
	unsigned int day;
	unsigned int hour;
	unsigned int minutes;
	unsigned int seconds;
	unsigned int ms;
	unsigned int mcs;
};

class TimeConverter
{
public:

	static std::chrono::time_point<std::chrono::system_clock> GetTime_SysClock();
	static long long GetTime_LLmcs();
	static TimePoint GetTime_TimePoint();

	static std::chrono::time_point<std::chrono::system_clock> LLmcsToSysClock(long long tp);
	static long long SysClockToLLmcs(std::chrono::time_point<std::chrono::system_clock> tp);
	static TimePoint LLmcsToTimePoint(long long tp);
	static TimePoint SysClockToTimePoint(std::chrono::time_point<std::chrono::system_clock> tp);
};

