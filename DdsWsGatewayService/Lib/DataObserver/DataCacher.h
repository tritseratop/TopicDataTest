#ifndef DATA_CACHER_H_
#define DATA_CACHER_H_

#include "../../Utilities/PackageAnalyser.h"
#include "../../Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Mappers.h"

#include <deque>
#include <unordered_map>

class DataCacher
{
public:
	DataCacher(size_t depth);

	void cache(DDSData data, const AdditionalTopicInfo& info);

	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info);

	std::optional<MediateDataDto> popDdsDto();
	std::deque<MediateDataDto> getDataCacheCopy() const;

	void cache(DDSAlarm data, const AdditionalTopicInfo& info);

	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info);

	//std::optional<WsDataDto::Wrapper> popDdsAlarm();
	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	size_t depth_;

	ThreadSafeDeque<MediateDataDto> data_cache_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsDataExMapper ddsdata_ex_mapper_;
	DdsDataMapper ddsdata_mapper_;

	PackageAnalyser* analyser_;
};

#endif //!DATA_CACHER_H_