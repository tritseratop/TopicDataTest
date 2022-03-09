#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include <deque>

#include "../../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../../include/CommonClasses.h"

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
	DataObserver(IServer* server);
	void handleDdsData(std::deque<DDSData> data);
	void handleDdsDataEx(std::deque<DDSDataEx> data);
	void handleDdsAlarm(std::deque<DDSAlarm> data);
	void handleDdsExAlarm(std::deque<DDSExAlarm> data);

private:
	DataMapper mapper_;
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_