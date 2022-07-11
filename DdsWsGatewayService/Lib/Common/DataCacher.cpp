#include "Lib/Common/DataCacher.h"

#include <utility>

namespace scada_ate
{
using namespace dds;

DataCacher::DataCacher(size_t depth, MappingInfo mapping_info)
	: depth_(depth)
	, mapping_info_(std::move(mapping_info))
{ }

void DataCacher::cache(DDSData data)
{
	data_cache_.push_back(ddsdata_mapper_.toMediateDataDto(std::move(data), mapping_info_));

	std::lock_guard<std::mutex> guard(cache_change_);
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::cache(const DDSDataEx& data)
{
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
	std::lock_guard<std::mutex> guard(cache_change_);
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

std::optional<std::string> DataCacher::pop()
{
	auto tmp = data_cache_.pop_front();
	if (tmp.has_value())
	{
		return std::optional<std::string>(dto_mapper_.toString(tmp.value()));
	}
	else
	{
		return std::optional<std::string>();
	}
}

std::optional<std::string> DataCacher::getLast()
{
	return {};
}

AlarmCacher::AlarmCacher(size_t depth, MappingInfo mapping_info)
	: depth_(depth)
	, mapping_info_(std::move(mapping_info))
{ }

void AlarmCacher::cache(DDSAlarm data) { }

void AlarmCacher::cache(const DDSAlarmEx& data) { }

std::optional<std::string> AlarmCacher::pop()
{
	auto tmp = alarm_cache_.pop_front();
	if (tmp.has_value())
	{
		//return std::optional<std::string>(dds_alarm_mapper_.toString(tmp.value()));
		return std::optional<std::string>("");
	}
	else
	{
		return std::optional<std::string>();
	}
}

std::optional<std::string> AlarmCacher::getLast()
{
	return {};
}

std::deque<MediateDataDto> DataCacher::getDataCacheCopy() const
{
	return data_cache_.getDequeCopy();
}
} // namespace scada_ate