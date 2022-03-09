#include <utility>

#include "DataObserver.h"

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
AlarmDto DataMapper::mapDdsExAlarm(DDSExAlarm data)
{
	AlarmDto result{ data.time_service() };
	return result;
}

DataObserver::DataObserver(IServer* server)
	: server_(server)
{
}

void DataObserver::handleDdsData(ThreadSafeQueue<DDSData> data)
{
	ThreadSafeQueue<DataDto> dtos;
	/*for (auto d : data)
	{
		dtos.push(mapper_.mapDdsData(std::move(d)));
	}
	for (const auto& d : dtos)
	{
		if (server_ != nullptr)
		{
			server_->sendData(d);
		}
	}*/
}

void DataObserver::handleDdsDataEx(ThreadSafeQueue<DDSDataEx> data)
{

}

void DataObserver::handleDdsAlarm(ThreadSafeQueue<DDSAlarm> data)
{

}

void DataObserver::handleDdsExAlarm(ThreadSafeQueue<DDSExAlarm> data)
{

}