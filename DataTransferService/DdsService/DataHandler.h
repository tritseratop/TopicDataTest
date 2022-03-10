#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include <deque>

#include "../ThreadSafeQueue/ThreadSafeQueue.h"
#include "../../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../../include/CommonClasses.h"

class DataMapper
{
public:
	DataDto mapDdsData(DDSData data);
	DataDto mapDdsDataEx(DDSDataEx data);
	AlarmDto mapDdsAlarm(DDSAlarm data);
	AlarmDto mapDdsAlarmEx(DDSAlarmEx data);
};

class DataHandler
{
public:
	DataHandler(IServer* server);

	void sendingDdsData(uint32_t sleep);
	void sendingDdsAlarm(uint32_t sleep);

	void stopSendingDdsData();
	void stopSendingDdsAlarm();

	void cacheDdsData(DDSData data);
	void cacheDdsDataEx(std::deque<DDSDataEx> data);
	void cacheDdsAlarm(std::deque<DDSAlarm> data);
	void cacheDdsAlarmEx(std::deque<DDSAlarmEx> data);

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;
	ThreadSafeQueue<DataDto> data_cache_;
	ThreadSafeQueue<AlarmDto> alarm_cache_;
	DataMapper mapper_;
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_