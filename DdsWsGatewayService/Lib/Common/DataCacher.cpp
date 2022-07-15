#include "Lib/Common/DataCacher.h"

#include <utility>

namespace scada_ate
{
using namespace dds;

DataCacher::DataCacher(size_t depth)
	: depth_(depth)
{ }

void DataCacher::cache(DDSData data, const dds::MappingInfo& mapping)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	unsafe_push(std::move(ddsdata_mapper_.toMediateDataDto(std::move(data), mapping)));
}

void DataCacher::cache(const DDSDataEx& data, const dds::MappingInfo& mapping)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	MediateDataDto dto = ddsdata_ex_mapper_.toMediateDataDto(
		std::move(data), mapping, data_cache_.empty() ? MediateDataDto() : data_cache_.back());
	unsafe_push(std::move(dto));
}

void DataCacher::update(DDSData data, const dds::MappingInfo& mapping)
{
	std::unique_lock<std::shared_mutex> w_lock(cache_mutex_);
	if (!data_cache_.empty() && data.time_service() < data_cache_.front().time_service)
	{
		return;
	}
	auto dto = ddsdata_mapper_.toMediateDataDtoOnPrevBase(
		std::move(data), data_cache_.empty() ? MediateDataDto() : data_cache_.back(), mapping);
	unsafe_push(std::move(dto));
}

void DataCacher::update(const DDSDataEx& data, const dds::MappingInfo& mapping)
{
	cache(data, mapping);
}

void DataCacher::updateOnlyDifferent(DDSData data) { }

void DataCacher::updateOnlyDifferent(const DDSDataEx& data) { }

void DataCacher::unsafe_push(MediateDataDto&& dto)
{
	data_cache_.push_back(std::move(dto));
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
{ }

void AlarmCacher::cache(DDSAlarm data, const dds::MappingInfo& mapping) { }

void AlarmCacher::cache(const DDSAlarmEx& data, const dds::MappingInfo& mapping) { }

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