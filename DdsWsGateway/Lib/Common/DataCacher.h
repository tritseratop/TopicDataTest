#ifndef DATA_CACHER_H_
#define DATA_CACHER_H_

#include "Lib/Common/Mappers.h"
#include "Utilities/Common/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/Dds/Config.h"

#include <deque>
#include <unordered_map>

namespace scada_ate
{
class Cacher
{
public:
	virtual std::optional<std::string> pop() = 0;
	virtual std::optional<std::string> getLast() const = 0;
};

class DataCacher : public Cacher
{
public:
	DataCacher(size_t depth);

	void cache(DDSData data, const dds::MappingInfo& mapping);
	void cache(const DDSDataEx& data, const dds::MappingInfo& mapping);

	void update(DDSData data, const dds::MappingInfo& mapping_);
	void update(const DDSDataEx& data, const dds::MappingInfo& mapping);

	//
	void updateOnlyDifferent(DDSData data);
	void updateOnlyDifferent(const DDSDataEx& data);

	void unsafe_push(MediateDataDto&& dto);

	std::optional<std::string> pop() override;
	std::optional<std::string> getLast() const override;

	std::deque<MediateDataDto> getDataCacheCopy() const;

private:
	size_t depth_;
	std::deque<MediateDataDto> data_cache_;

	mutable std::shared_mutex cache_mutex_;

	MediateDtoMapper dto_mapper_;
	DdsDataExMapper ddsdata_ex_mapper_;
	DdsDataMapper ddsdata_mapper_;
};

class AlarmCacher : public Cacher
{
public:
	AlarmCacher(size_t depth, dds::MappingInfo mapping_info_);

	void cache(DDSAlarm data, const dds::MappingInfo& mapping);
	void cache(const DDSAlarmEx& data, const dds::MappingInfo& mapping);

	std::optional<std::string> pop() override;
	std::optional<std::string> getLast() const override;

	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	size_t depth_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsAlarmMapper dds_alarm_mapper_;
	DdsAlarmExMapper dds_alarm_ex_mapper_;
};
} // namespace scada_ate

#endif //!DATA_CACHER_H_