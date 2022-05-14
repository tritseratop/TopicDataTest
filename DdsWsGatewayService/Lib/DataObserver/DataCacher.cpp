#include "DataCacher.h"

#include <utility>

DataCacher::DataCacher(size_t depth)
	: depth_(depth)
{
	analyser_ = PackageAnalyser::getInstance();
	analyser_->addDataToAnalyse("DDSData to Dto");
	analyser_->addDataToAnalyse("DDSDataEx to Dto");
}

void DataCacher::cache(DDSData data, const AdditionalTopicInfo& info)
{
	auto start = TimeConverter::GetTime_LLmcs();

	data_cache_.push_back(ddsdata_mapper_.toMediateDataDto(std::move(data), info));

	analyser_->pushDataTimestamp("DDSData to Dto", TimeConverter::GetTime_LLmcs() - start);

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::cache(const DDSDataEx& data, const AdditionalTopicInfo& info)
{
	auto start = TimeConverter::GetTime_LLmcs();

	if (!data_cache_.empty())
	{
		data_cache_.push_back(
			ddsdata_ex_mapper_.toMediateDataDto(data, info, data_cache_.back().value()));
	}
	else
	{
		//TODO MediateDataDto default constructor check
		data_cache_.push_back(ddsdata_ex_mapper_.toMediateDataDto(data, info, MediateDataDto()));
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

void DataCacher::cache(DDSAlarm data, const AdditionalTopicInfo& info) { }

void DataCacher::cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info) { }

std::deque<MediateDataDto> DataCacher::getDataCacheCopy() const
{
	return data_cache_.getDequeCopy();
}