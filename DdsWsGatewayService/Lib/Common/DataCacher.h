#ifndef DATA_CACHER_H_
#define DATA_CACHER_H_

#include "Lib/Common/Mappers.h"
#include "Utilities/Common/ThreadSafeQueue/ThreadSafeQueue.h"

#include <deque>
#include <unordered_map>

namespace scada_ate
{
class Cacher
{
public:
	virtual std::optional<std::string> pop() = 0;
	virtual std::optional<std::string> getLast() = 0;
};

class DataCacher : public Cacher
{
public:
	DataCacher(size_t depth, dds::MappingInfo mapping_info);

	void cache(DDSData data);
	void cache(const DDSDataEx& data);

	std::optional<std::string> pop() override;
	std::optional<std::string> getLast() override;

	std::deque<MediateDataDto> getDataCacheCopy() const;

private:
	size_t depth_;
	ThreadSafeDeque<MediateDataDto> data_cache_;

	mutable std::mutex cache_change_;

	MediateDtoMapper dto_mapper_;
	DdsDataExMapper ddsdata_ex_mapper_;
	DdsDataMapper ddsdata_mapper_;

	const dds::MappingInfo mapping_info_;
};

class AlarmCacher : public Cacher
{
public:
	AlarmCacher(size_t depth, dds::MappingInfo mapping_info_);

	void cache(DDSAlarm data);
	void cache(const DDSAlarmEx& data);

	std::optional<std::string> pop() override;
	std::optional<std::string> getLast() override;

	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	size_t depth_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsAlarmMapper dds_alarm_mapper_;
	DdsAlarmExMapper dds_alarm_ex_mapper_;

	const dds::MappingInfo mapping_info_;
};
} // namespace scada_ate

#endif //!DATA_CACHER_H_