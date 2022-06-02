#include "Lib/Notifier/DataCacher.h"

#include <utility>

DataCacher::DataCacher(size_t depth, AdditionalTopicInfo mapping_info)
	: depth_(depth)
	, mapping_info_(std::move(mapping_info))
{
	analyser_ = PackageAnalyser::getInstance();
	analyser_->addDataToAnalyse("DDSData to Dto");
	analyser_->addDataToAnalyse("DDSDataEx to Dto");
}

void DataCacher::cache(DDSData data)
{
	auto start = TimeConverter::GetTime_LLmcs();

	data_cache_.push_back(ddsdata_mapper_.toMediateDataDto(std::move(data), mapping_info_));

	analyser_->pushDataTimestamp("DDSData to Dto", TimeConverter::GetTime_LLmcs() - start);

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::cache(const DDSDataEx& data)
{
	auto start = TimeConverter::GetTime_LLmcs();

	if (!data_cache_.empty())
	{
		data_cache_.push_back(
			ddsdata_ex_mapper_.toMediateDataDto(data, mapping_info_, data_cache_.back().value()));
	}
	else
	{
		//TODO MediateDataDto default constructor check
		data_cache_.push_back(
			ddsdata_ex_mapper_.toMediateDataDto(data, mapping_info_, MediateDataDto()));
	}

	analyser_->pushDataTimestamp("DDSDataEx to Dto", TimeConverter::GetTime_LLmcs() - start);

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

std::optional<MediateDataDto> DataCacher::popDdsDto()
{
	return data_cache_.pop_front();
}

AlarmCacher::AlarmCacher(size_t depth, AdditionalTopicInfo mapping_info)
	: depth_(depth)
	, mapping_info_(std::move(mapping_info))
{ }

void AlarmCacher::cache(DDSAlarm data) { }

void AlarmCacher::cache(const DDSAlarmEx& data) { }

std::deque<MediateDataDto> DataCacher::getDataCacheCopy() const
{
	return data_cache_.getDequeCopy();
}