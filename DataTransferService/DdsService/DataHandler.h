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
	DataDto mapDdsDataEx(const DDSDataEx& data_ex, DataDto data);
	AlarmDto mapDdsAlarm(DDSAlarm data);
	AlarmDto mapDdsAlarmEx(const DDSAlarmEx& data_ex, AlarmDto dto);

	template<class DtoDataCollection, class DdsData>
	void fillChanged(DtoDataCollection& next, const std::vector<DdsData>& prev);
};

class DataHandler
{
public:
	DataHandler(IServer* server);

	bool sendDdsData();
	bool sendDdsAlarm();

	void cacheDdsData(DDSData data);
	void cacheDdsDataEx(DDSDataEx data);
	void cacheDdsAlarm(DDSAlarm data);
	void cacheDdsAlarmEx(DDSAlarmEx data);

	std::deque<DataDto> getDataCacheCopy() const;
	std::deque<AlarmDto> getAlarmCacheCopy() const;

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;
	ThreadSafeDeque<DataDto> data_cache_;
	ThreadSafeDeque<AlarmDto> alarm_cache_;
	DataMapper mapper_;
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_