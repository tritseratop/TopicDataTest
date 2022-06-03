#ifndef DATA_CACHER_H_
#define DATA_CACHER_H_

#include "Lib/Common/Mappers.h"
#include "Utilities/PackageAnalyser.h"
#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"

#include <deque>
#include <unordered_map>

//template<class DdsTopic>
//class Cacher
//{
//
//};

class DataCacher
{
public:
	DataCacher(size_t depth, AdditionalTopicInfo mapping_info);

	void cache(DDSData data);
	void cache(const DDSDataEx& data);

	std::optional<MediateDataDto> popDdsDto();
	std::deque<MediateDataDto> getDataCacheCopy() const;

private:
	size_t depth_;
	ThreadSafeDeque<MediateDataDto> data_cache_;

	DdsDataExMapper ddsdata_ex_mapper_;
	DdsDataMapper ddsdata_mapper_;

	const AdditionalTopicInfo mapping_info_;

	PackageAnalyser* analyser_;
};

class AlarmCacher
{
public:
	AlarmCacher(size_t depth, AdditionalTopicInfo mapping_info_);

	void cache(DDSAlarm data);
	void cache(const DDSAlarmEx& data);

	//std::optional<WsDataDto::Wrapper> popDdsAlarm();
	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	size_t depth_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsAlarmMapper dds_alarm_mapper_;
	DdsAlarmExMapper dds_alarm_ex_mapper_;

	const AdditionalTopicInfo mapping_info_;
};

#endif //!DATA_CACHER_H_