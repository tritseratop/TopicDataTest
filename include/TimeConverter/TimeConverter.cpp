#include "TimeConverter.hpp"

std::chrono::time_point<std::chrono::system_clock> TimeConverter::GetTime_SysClock()
{
	return std::chrono::system_clock::now();
}

long long TimeConverter::GetTime_LLmcs()
{
	return  std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

TimePoint TimeConverter::GetTime_TimePoint()
{
	TimePoint tp;
	tm tp_tm;
	time_t tp_tt;
	auto tp_sys = std::chrono::system_clock::now();
	tp_tt = std::chrono::system_clock::to_time_t(tp_sys);
	localtime_s(&tp_tm, &tp_tt);

	/*std::chrono::seconds now_sec = std::chrono::duration_cast<std::chrono::seconds>(tp_sys.time_since_epoch());
	std::chrono::milliseconds now_msec = std::chrono::duration_cast<std::chrono::milliseconds>(tp_sys.time_since_epoch());
	std::chrono::microseconds now_mcsec = std::chrono::duration_cast<std::chrono::microseconds>(tp_sys.time_since_epoch());*/

	tp.year = tp_tm.tm_year + 1900;
	tp.mounth = tp_tm.tm_mon+1;
	tp.day = tp_tm.tm_mday;
	tp.hour = tp_tm.tm_hour;
	tp.minutes = tp_tm.tm_min;
	tp.seconds = tp_tm.tm_sec;

	tp.ms = (tp_sys.time_since_epoch().count() % 10000000) / 10000;
	tp.mcs = (tp_sys.time_since_epoch().count() % 10000) / 10;
	
	/*tp.mcs = (now_mcsec - std::chrono::duration_cast<std::chrono::microseconds>(now_msec)).count();
	tp.ms = (now_msec - std::chrono::duration_cast<std::chrono::milliseconds>(now_sec)).count();*/

	return  tp;
}

std::chrono::time_point<std::chrono::system_clock> TimeConverter::LLmcsToSysClock(long long tp)
{
	
	std::chrono::time_point<std::chrono::system_clock> now_time_convert{ std::chrono::microseconds(tp) };
	return now_time_convert;
}

long long TimeConverter::SysClockToLLmcs(std::chrono::time_point<std::chrono::system_clock> tp)
{
	auto now_mcs = std::chrono::time_point_cast<std::chrono::microseconds> (tp);
	return now_mcs.time_since_epoch().count();
}

TimePoint TimeConverter::LLmcsToTimePoint(long long tp)
{
	TimePoint t;
	tm tp_tm;
	time_t tp_tt = tp/10000000;
	localtime_s(&tp_tm, &tp_tt);

	t.year = tp_tm.tm_year + 1900;
	t.mounth = tp_tm.tm_mon + 1;
	t.day = tp_tm.tm_mday;
	t.hour = tp_tm.tm_hour;
	t.minutes = tp_tm.tm_min;
	t.seconds = tp_tm.tm_sec;

	t.ms = (tp % 1000000) / 1000;
	t.mcs = (tp % 1000);

	return  t;
}

TimePoint TimeConverter::SysClockToTimePoint(std::chrono::time_point<std::chrono::system_clock> tp)
{
	TimePoint t;
	tm tp_tm;
	time_t tp_tt;
	tp_tt = std::chrono::system_clock::to_time_t(tp);
	localtime_s(&tp_tm, &tp_tt);

	/*std::chrono::seconds now_sec = std::chrono::duration_cast<std::chrono::seconds>(tp_sys.time_since_epoch());
	std::chrono::milliseconds now_msec = std::chrono::duration_cast<std::chrono::milliseconds>(tp_sys.time_since_epoch());
	std::chrono::microseconds now_mcsec = std::chrono::duration_cast<std::chrono::microseconds>(tp_sys.time_since_epoch());*/

	t.year = tp_tm.tm_year + 1900;
	t.mounth = tp_tm.tm_mon + 1;
	t.day = tp_tm.tm_mday;
	t.hour = tp_tm.tm_hour;
	t.minutes = tp_tm.tm_min;
	t.seconds = tp_tm.tm_sec;

	t.ms = (tp.time_since_epoch().count() % 10000000) / 10000;
	t.mcs = (tp.time_since_epoch().count() % 10000) / 10;

	/*tp.mcs = (now_mcsec - std::chrono::duration_cast<std::chrono::microseconds>(now_msec)).count();
	tp.ms = (now_msec - std::chrono::duration_cast<std::chrono::milliseconds>(now_sec)).count();*/

	return  t;
}