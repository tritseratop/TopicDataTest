#include <utility>

#include "DataHandler.h"

DataDto DataMapper::mapDdsData(DDSData data)
{
	DataDto	result{ data.time_service() };
	return result;
}

DataDto DataMapper::mapDdsDataEx(DDSDataEx data)
{
	DataDto	result{ data.time_service() };
	return result;
}
AlarmDto DataMapper::mapDdsAlarm(DDSAlarm data)
{
	AlarmDto result{ data.time_service() };
	return result;
}
AlarmDto DataMapper::mapDdsAlarmEx(DDSAlarmEx data)
{
	AlarmDto result{ data.time_service() };
	return result;
}

DataHandler::DataHandler(IServer* server)
	: server_(server)
	, stop_sending_data_(false)
{
}

void DataHandler::sendingDdsData(uint32_t sleep)
{
	while (!stop_sending_data_)
	{
		if (data_cache_.size() != 0)
		{
			server_->sendData(data_cache_.pop().value());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

void DataHandler::sendingDdsAlarm(uint32_t sleep)
{
	while (!stop_sending_alarm_)
	{
		if (data_cache_.size() != 0)
		{
			server_->sendAlarm(alarm_cache_.pop().value());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

void DataHandler::stopSendingDdsData()
{
	stop_sending_data_ = true;
}
void DataHandler::stopSendingDdsAlarm()
{
	stop_sending_alarm_ = true;
}

void DataHandler::cacheDdsData(DDSData data)
{
	data_cache_.push(mapper_.mapDdsData(std::move(data)));
}

void DataHandler::cacheDdsDataEx(std::deque<DDSDataEx> data)
{

}

void DataHandler::cacheDdsAlarm(std::deque<DDSAlarm> data)
{

}

void DataHandler::cacheDdsAlarmEx(std::deque<DDSAlarmEx> data)
{

}