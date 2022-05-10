#ifndef DATA_CACHER_H_
#define DATA_CACHER_H_

#include "../../Utilities/DdsCommonClasses.h"
#include "../../Utilities/PackageAnalyser.h"
#include "../../Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "../../Utilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../../Utilities/WsDto.h"

#include <deque>
#include <unordered_map>

class DdsTopicToMediateDtoMapper
{
public:
	MediateDataDto mapDdsData(DDSData data, const AdditionalTopicInfo& info);
	MediateDataDto mapDdsDataEx(MediateDataDto prev_dto,
								const DDSDataEx& cur_data_ex,
								const AdditionalTopicInfo& info);

	MediateAlarmDto mapDdsAlarm(DDSAlarm data, const AdditionalTopicInfo& info);
	MediateAlarmDto mapDdsAlarmEx(MediateAlarmDto prev_dto,
								  const DDSAlarmEx& cur_data_ex,
								  const AdditionalTopicInfo& info);

	template <class DtoDataCollection, class DdsSample>
	void fillChanged(DtoDataCollection& prev_dto_collection,
					 const std::vector<DdsSample>& cur_samples,
					 const TagToIndex& tag_to_index);
};

class DirectDtoMapper
{
public:
	WsDataDto mapDataDto(MediateDataDto data);
};

class MediateDtoToWsDtoMapper
{
public:
	WsDataDto::Wrapper mapDataDto(const MediateDataDto& data);

private:
	template <class OatppT, class T>
	void fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v);
	void fillVector(oatpp::Vector<oatpp::String>& oatpp_v, const std::vector<std::vector<char>>& v);
	void fillVector(oatpp::String& oatpp_v, const std::vector<char>& v);
};

class DataCacher
{
public:
	DataCacher(size_t depth);

	void cache(DDSData data, const AdditionalTopicInfo& info);
	void cache(DDSData data,
			   const AdditionalTopicInfo& info,
			   const AdditionalPackageInfo& package_info);
	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSDataEx& data,
			   const AdditionalTopicInfo& info,
			   const AdditionalPackageInfo& package_info);
	std::optional<MediateDataDto> popDdsDto();
	std::deque<MediateDataDto> getDataCacheCopy() const;

	void cache(DDSAlarm data, const AdditionalTopicInfo& info);
	void cache(DDSAlarm data,
			   const AdditionalTopicInfo& info,
			   const AdditionalPackageInfo& package_info);
	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSAlarmEx& data,
			   const AdditionalTopicInfo& info,
			   const AdditionalPackageInfo& package_info);
	//std::optional<WsDataDto::Wrapper> popDdsAlarm();
	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	size_t depth_;

	ThreadSafeDeque<MediateDataDto> data_cache_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsTopicToMediateDtoMapper mapper_;

	PackageAnalyser* analyser_;
};

#endif //!DATA_CACHER_H_