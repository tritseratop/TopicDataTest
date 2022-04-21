#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include <deque>
#include <unordered_map>

#include "../DataTransferService/ThreadSafeQueue/ThreadSafeQueue.h"
#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "DdsCommonClasses.h"
#include "WsCommonClasses.h"
#include "WsDto.h"

class DdsTopicToMediateDtoMapper
{
public:
	DataDto mapDdsData(DDSData data, const AdditionalTopicInfo& info);
	DataDto mapDdsDataEx(DataDto prev_dto, const DDSDataEx& cur_data_ex, const AdditionalTopicInfo& info);
	AlarmDto mapDdsAlarm(DDSAlarm data, const AdditionalTopicInfo& info);
	AlarmDto mapDdsAlarmEx(AlarmDto prev_dto, const DDSAlarmEx& cur_data_ex, const AdditionalTopicInfo& info);

	template<class DtoDataCollection, class DdsSample>
	void fillChanged(DtoDataCollection& prev_dto_collection, const std::vector<DdsSample>& cur_samples, const TagToIndex& tag_to_index);
};

class DirectDtoMapper
{
public:
	WsDataDto mapDataDto(DataDto data);

};

class MediateDtoToWsDtoMapper
{
public:
	WsDataDto::Wrapper mapDataDto(const DataDto& data);

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
	void update(DDSData data, const AdditionalTopicInfo& info);
	void update(DDSData data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void update(const DDSDataEx& data, const AdditionalTopicInfo& info);
	void update(const DDSDataEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<DataDto> getDataCacheCopy() const;

	bool sendDdsAlarm();
	void update(DDSAlarm data, const AdditionalTopicInfo& info);
	void update(DDSAlarm data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void update(const DDSAlarmEx& data, const AdditionalTopicInfo& info);
	void update(const DDSAlarmEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<AlarmDto> getAlarmCacheCopy() const;

private:
	bool stop_sending_data_;
	ThreadSafeDeque<DataDto> data_cache_;

	bool stop_sending_alarm_;
	ThreadSafeDeque<AlarmDto> alarm_cache_;

	DdsTopicToMediateDtoMapper mapper_;
	MediateDtoToWsDtoMapper ws_mapper_;
	IServer* server_;
};

#endif//!DATA_OBSERVER_H_