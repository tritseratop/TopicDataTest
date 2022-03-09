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

void DataObserver::handleDdsData(std::deque<DDSData> data)
{
	std::deque<DataDto> dtos;
	for (auto d : data)
	{
		dtos.push_back(mapper_.mapDdsData(std::move(d)));
	}
	for (const auto& d : dtos)
	{
		if (server_ != nullptr)
		{
			server_->sendData(d);
		}
	}
}

void DataObserver::handleDdsDataEx(std::deque<DDSDataEx> data)
{

}

void DataObserver::handleDdsAlarm(std::deque<DDSAlarm> data)
{

}

void DataObserver::handleDdsExAlarm(std::deque<DDSExAlarm> data)
{

}