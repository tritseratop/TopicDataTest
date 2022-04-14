#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include <deque>
#include <unordered_map>

#include "../DataTransferService/ThreadSafeQueue/ThreadSafeQueue.h"
#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "PackageAnalysis.h"
#include "DdsCommonClasses.h"
#include "WsCommonClasses.h"
#include "WsDto.h"

class DataMapper
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

class WsDtoMapper
{
public:
	WsDataDto::Wrapper mapDataDto(const DataDto& data);

private:
	template<class OatppT, class T>
	void fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v);
	void fillVector(oatpp::Vector<oatpp::String>& oatpp_v, const std::vector<std::vector<char>>& v);
	void fillVector(oatpp::String& oatpp_v, const std::vector<char>& v);
};

class DataHandler
{
public:
	DataHandler(IServer* server);

	bool sendDdsData();
	void cache(DDSData data, const AdditionalTopicInfo& info);
	void cache(DDSData data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSDataEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<DataDto> getDataCacheCopy() const;

	bool sendDdsAlarm();
	void cache(DDSAlarm data, const AdditionalTopicInfo& info);
	void cache(DDSAlarm data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info);
	void cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info, const AdditionalPackageInfo& package_info);
	std::deque<AlarmDto> getAlarmCacheCopy() const;

private:
	bool stop_sending_data_;
	ThreadSafeDeque<DataDto> data_cache_;

	bool stop_sending_alarm_;
	ThreadSafeDeque<AlarmDto> alarm_cache_;

	DataMapper mapper_;
	WsDtoMapper ws_mapper_;
	IServer* server_;

	PackageAnalyser* analyser_;
};

#endif//!DATA_OBSERVER_H_