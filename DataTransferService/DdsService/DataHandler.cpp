#include <utility>

#include "DataHandler.h"

DataDto DataMapper::mapDdsData(DDSData data)
{
	std::vector<std::vector<char>> data_char;
	data_char.reserve(data.data_char().value().size());

	for (int i = 0; i < data.data_char().value().size(); ++i)
	{
		data_char.push_back(std::move(data.data_char().value()[i].value()));
	}

	DataDto	result{
		data.time_service(),
		{ 
			std::vector(data.data_int().value().size(), data.time_source()),
			std::move(data.data_int().value()),
			std::move(data.data_int().quality()) 
		},
		{ 
			std::vector(data.data_float().value().size(), data.time_source()),
			std::move(data.data_float().value()),
			std::move(data.data_float().quality()) 
		},
		{
			std::vector(data.data_double().value().size(), data.time_source()),
			std::move(data.data_double().value()),
			std::move(data.data_double().quality()) 
		},
		{
			std::vector(data.data_char().value().size(), data.time_source()),
			std::move(data_char),
			std::move(data.data_char().quality())
		}
	};
	return result;
}

DataDto DataMapper::mapDdsDataEx(const DDSDataEx& data_ex, DataDto data)
{
	data.time_service = data_ex.time_service();
	fillChanged(data.data_int, data_ex.data_int());
	fillChanged(data.data_float, data_ex.data_float());
	fillChanged(data.data_double, data_ex.data_double());
	fillChanged(data.data_char, data_ex.data_char());

	return data;
}

AlarmDto DataMapper::mapDdsAlarm(DDSAlarm data)
{
	AlarmDto result{
		data.time_service(),
		std::vector(data.alarms().size(), data.time_source()),
		std::move(data.alarms()),
		std::move(data.quality())
	};
	return result;
}

AlarmDto DataMapper::mapDdsAlarmEx(const DDSAlarmEx& data_ex, AlarmDto dto)
{
	dto.time_service = data_ex.time_service();
	fillChanged(dto, data_ex.alarms());
	return dto;
}

template<class DtoDataCollection, class DdsData>
void DataMapper::fillChanged(DtoDataCollection& next, const std::vector<DdsData>& prev)
{
	size_t n = prev.size();
	for (const auto& c : prev)
	{
		if (c.id_tag() >= next.size())
		{
			next.resize(c.id_tag() + 1);
		}
		next.time_source[c.id_tag()] = c.time_source();
		next.value[c.id_tag()] = c.value();
		next.quality[c.id_tag()] = c.quality();
	}
}

DataHandler::DataHandler(IServer* server)
	: server_(server)
	, stop_sending_data_(false)
	, stop_sending_alarm_(false)
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