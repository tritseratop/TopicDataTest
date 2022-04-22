#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include "ThreadSafeQueue/ThreadSafeQueue.h"
#include "TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "DdsCommonClasses.h"
#include "WsCommonClasses.h"
#include "WsDto.h"

#include <deque>
#include <unordered_map>

class DdsTopicToMediateDtoMapper
{
public:
	MediateDataDto mapDdsData(DDSData data, const AdditionalTopicInfo& info);
	MediateDataDto mapDdsDataEx(MediateDataDto prev_dto, const DDSDataEx& cur_data_ex, const AdditionalTopicInfo& info);
	MediateAlarmDto mapDdsAlarm(DDSAlarm data, const AdditionalTopicInfo& info);
	MediateAlarmDto mapDdsAlarmEx(MediateAlarmDto prev_dto, const DDSAlarmEx& cur_data_ex, const AdditionalTopicInfo& info);

	template<class DtoDataCollection, class DdsSample>
	void fillChanged(DtoDataCollection& prev_dto_collection, const std::vector<DdsSample>& cur_samples, const TagToIndex& tag_to_index);
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
	template<class OatppT, class T>
	void fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v);
	void fillVector(oatpp::Vector<oatpp::String>& oatpp_v, const std::vector<std::vector<char>>& v);
	void fillVector(oatpp::String& oatpp_v, const std::vector<char>& v);
};

class DataObserver
{
public:
	DataObserver(IServer* server);

	bool sendDdsData();
	void cache(DDSData data, const AdditionalTopicInfo& info);
	void cache(DDSData data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<MediateDataDto> getDataCacheCopy() const;

	bool sendDdsAlarm();
	void cache(DDSAlarm data, const AdditionalTopicInfo& info);
	void cache(DDSAlarm data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<MediateAlarmDto> getAlarmCacheCopy() const;

private:
	bool stop_sending_data_;
	ThreadSafeDeque<MediateDataDto> data_cache_;

	bool stop_sending_alarm_;
	ThreadSafeDeque<MediateAlarmDto> alarm_cache_;

	DdsTopicToMediateDtoMapper mapper_;
	MediateDtoToWsDtoMapper ws_mapper_;
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_