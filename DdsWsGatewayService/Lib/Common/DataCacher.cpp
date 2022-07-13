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
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	unsafe_push(data);
}

void DataCacher::cache(const DDSDataEx& data)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	unsafe_push(data);
}

void DataCacher::update(DDSData data)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	if (!data_cache_.empty() && data.time_service() < data_cache_.front().time_service)
	{
		return;
	}
	cache(std::move(data));
}

void DataCacher::update(const DDSDataEx& data)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	if (!data_cache_.empty() && data.time_service() < data_cache_.front().time_service)
	{
		return;
	}
	unsafe_push(data);
}

void DataCacher::updateOnlyDifferent(DDSData data) { }

void DataCacher::updateOnlyDifferent(const DDSDataEx& data) { }

void DataCacher::unsafe_push(DDSData data)
{
	data_cache_.push_back(ddsdata_mapper_.toMediateDataDto(std::move(data), mapping_info_));
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::unsafe_push(const DDSDataEx& data)
{
	if (!data_cache_.empty())
	{
		data_cache_.push_back(
			ddsdata_ex_mapper_.toMediateDataDto(data, mapping_info_, data_cache_.back()));
	}
	else
	{
		//TODO MediateDataDto default constructor check
		data_cache_.push_back(
			ddsdata_ex_mapper_.toMediateDataDto(data, mapping_info_, MediateDataDto()));
	}
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

std::optional<std::string> DataCacher::pop()
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	if (!data_cache_.empty())
	{
		auto tmp = data_cache_.front();
		data_cache_.pop_front();
		return std::optional<std::string>(dto_mapper_.toString(tmp));
	}
	else
	{
		return std::optional<std::string>();
	}
}

std::optional<std::string> DataCacher::getLast() const
{
	std::shared_lock<std::shared_mutex> r_lock(cache_mutex_);
	return {};
}

std::deque<MediateDataDto> DataCacher::getDataCacheCopy() const
{
	std::shared_lock<std::shared_mutex> r_lock(cache_mutex_);
	return data_cache_;
}

AlarmCacher::AlarmCacher(size_t depth, MappingInfo mapping_info)
	: depth_(depth)
	, mapping_info_(std::move(mapping_info))
{ }

void AlarmCacher::cache(DDSAlarm data) { }

void AlarmCacher::cache(const DDSAlarmEx& data) { }

std::optional<std::string> AlarmCacher::pop()
{
	auto tmp = alarm_cache_.pop_front_and_return();
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

std::optional<std::string> AlarmCacher::getLast() const
{
	return {};
}

} // namespace scada_ate