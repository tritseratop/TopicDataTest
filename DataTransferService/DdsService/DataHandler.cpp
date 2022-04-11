#include <utility>

#include "DataHandler.h"

DataDto DataMapper::mapDdsData(DDSData data, const AdditionalTopicInfo& info)
{
	std::vector<std::vector<char>> data_char;
	data_char.reserve(data.data_char().value().size());
	
	auto tags = info.tags;
	for (auto& [type, tag] : tags)
	{
		tag.resize(data.data_char().value().size());
	}

	for (int i = 0; i < data.data_char().value().size(); ++i)
	{
		data_char.push_back(std::move(data.data_char().value()[i].value()));
	}

	DataDto	result{
		data.time_service(),
		{ 
			std::vector(data.data_int().value().size(), data.time_source()),
			std::move(tags.at(DataCollectiionType::DATA_INT)),
			std::move(data.data_int().value()),
			std::move(data.data_int().quality()) 
		},
		{ 
			std::vector(data.data_float().value().size(), data.time_source()),
			std::move(tags.at(DataCollectiionType::DATA_FLOAT)),
			std::move(data.data_float().value()),
			std::move(data.data_float().quality()) 
		},
		{
			std::vector(data.data_double().value().size(), data.time_source()),
			std::move(tags.at(DataCollectiionType::DATA_DOUBLE)),
			std::move(data.data_double().value()),
			std::move(data.data_double().quality()) 
		},
		{
			std::vector(data.data_char().value().size(), data.time_source()),
			std::move(tags.at(DataCollectiionType::DATA_CHAR)),
			std::move(data_char),
			std::move(data.data_char().quality())
		}
	};
	return result;
}

DataDto DataMapper::mapDdsDataEx(DataDto prev_dto, const DDSDataEx& cur_data_ex, const AdditionalTopicInfo& info)
{
	prev_dto.time_service = cur_data_ex.time_service();
	fillChanged(prev_dto.data_int, cur_data_ex.data_int(), info.tag_to_index.at(DataCollectiionType::DATA_INT));
	fillChanged(prev_dto.data_float, cur_data_ex.data_float(), info.tag_to_index.at(DataCollectiionType::DATA_FLOAT));
	fillChanged(prev_dto.data_double, cur_data_ex.data_double(), info.tag_to_index.at(DataCollectiionType::DATA_DOUBLE));
	fillChanged(prev_dto.data_char, cur_data_ex.data_char(), info.tag_to_index.at(DataCollectiionType::DATA_DOUBLE));

	return prev_dto;
}

AlarmDto DataMapper::mapDdsAlarm(DDSAlarm data,const AdditionalTopicInfo& info)
{
	auto tags = info.tags.at(DataCollectiionType::ALARM_UINT32);
	tags.resize(data.alarms().size());

	AlarmDto result{
		data.time_service(),
		std::vector(data.alarms().size(), data.time_source()),
		std::move(tags),
		std::move(data.alarms()),
		std::move(data.quality())
	};
	return result;
}

AlarmDto DataMapper::mapDdsAlarmEx(AlarmDto prev_dto, const DDSAlarmEx& cur_data_ex, const AdditionalTopicInfo& info)
{
	prev_dto.time_service = cur_data_ex.time_service();
	fillChanged(prev_dto, cur_data_ex.alarms(), info.tag_to_index.at(DataCollectiionType::ALARM_UINT32));
	return prev_dto;
}

template<class DtoDataCollection, class DdsSample>
void DataMapper::fillChanged(DtoDataCollection& prev_dto_collection, const std::vector<DdsSample>& cur_samples, const TagToIndex& tag_to_index)
{
	size_t n = cur_samples.size();
	for (const auto& sample : cur_samples)
	{
		if (tag_to_index.at(sample.id_tag()) >= prev_dto_collection.size())
		{
			prev_dto_collection.resize(tag_to_index.at(sample.id_tag()) + 1);
		}
		prev_dto_collection.time_source[tag_to_index.at(sample.id_tag())] = sample.time_source();
		prev_dto_collection.id_tag[tag_to_index.at(sample.id_tag())] = sample.id_tag();
		prev_dto_collection.value[tag_to_index.at(sample.id_tag())] = sample.value();
		prev_dto_collection.quality[tag_to_index.at(sample.id_tag())] = sample.quality();
	}
}

DataHandler::DataHandler(IServer* server)
	: server_(server)
	, stop_sending_data_(false)
	, stop_sending_alarm_(false)
{
}

bool DataHandler::sendDdsData()
{
	if (server_ != nullptr)
	{
		if (data_cache_.size() != 0)
		{
			server_->sendData(data_cache_.pop_front().value());
		}
		return true;
	}
	return false;
}

bool DataHandler::sendDdsAlarm()
{
	if (server_ != nullptr)
	{
		if (data_cache_.size() != 0)
		{
			server_->sendAlarm(alarm_cache_.pop_front().value());
		}
		return true;
	}
	return false;
}

void DataHandler::cache(DDSData data, const AdditionalTopicInfo& info)
{
	data_cache_.push_back(mapper_.mapDdsData(std::move(data), info));
}

void DataHandler::cache(const DDSDataEx& data, const AdditionalTopicInfo& info)
{
	if (!data_cache_.empty())
	{
		data_cache_.push_back(mapper_.mapDdsDataEx(data_cache_.back().value(), data, info));
	}
	else
	{
		//TODO datadto default constructor check
		data_cache_.push_back(mapper_.mapDdsDataEx(DataDto(), data, info));
	}
}

void DataHandler::cache(DDSAlarm data, const AdditionalTopicInfo& info)
{

}

void DataHandler::cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info)
{

}

std::deque<DataDto> DataHandler::getDataCacheCopy() const
{
	return data_cache_.getDequeCopy();
}