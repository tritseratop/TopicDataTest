#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include <deque>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"

struct DataDto
{
	uint64_t time_service;
};

struct AlarmDto
{
	uint64_t time_service;
};

class DataMapper
{
public:
	DataDto mapDdsData(DDSData data);
	DataDto mapDdsDataEx(DDSDataEx data);
	AlarmDto mapDdsAlarm(DDSAlarm data);
	AlarmDto mapDdsExAlarm(DDSExAlarm data);
};

class DataObserver
{
public:
	void handleDdsData(std::deque<DDSData> data);
	void handleDdsDataEx(std::deque<DDSDataEx> data);
	void handleDdsAlarm(std::deque<DDSAlarm> data);
	void handleDdsExAlarm(std::deque<DDSExAlarm> data);

private:
	DataMapper mapper_;
};

#endif//!DATA_OBSERVER_H_