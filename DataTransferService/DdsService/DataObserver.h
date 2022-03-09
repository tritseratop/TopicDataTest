#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

//#include <deque>

#include "../ThreadSafeQueue/ThreadSafeQueue.h"
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
	void handleDdsData(ThreadSafeQueue<DDSData> data);
	void handleDdsDataEx(ThreadSafeQueue<DDSDataEx> data);
	void handleDdsAlarm(ThreadSafeQueue<DDSAlarm> data);
	void handleDdsExAlarm(ThreadSafeQueue<DDSExAlarm> data);

private:
	DataMapper mapper_;   
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_