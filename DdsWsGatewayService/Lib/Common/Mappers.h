#ifndef MAPPERS_H_
#define MAPPERS_H_

#include "Utilities/DdsCommonClasses.h"
#include "Utilities/MediateDto.h"
#include "Utilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "Utilities/WsDto.h"
#include "Utilities/nlohmann/json.hpp"

template<class T>
void pushBackContainerWithChars(std::back_insert_iterator<T> result_it,
								const std::vector<char>& chars);

std::string convertCharVectorToString(const std::vector<char>& chars);

template<class T, class DdsSample>
void fillChanged(DataSampleSequence<T>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const TagToIndex& tag_to_index);

template<class T, class DdsSample>
void fillChanged(std::vector<DataSample<T>>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const TagToIndex& tag_to_index);

class DdsDataMapper
{
public:
	DdsDataMapper() { }
	DdsDataMapper(const AdditionalTopicInfo& info)
		: info_(info)
	{ }
	MediateDataDto toMediateDataDto(DDSData data, const AdditionalTopicInfo& info);

	MediateDataDtoWithVectorsOfStruct
	toMediateDataDtoWithVectorsOfStruct(DDSData data, const AdditionalTopicInfo& info);

private:
	AdditionalTopicInfo info_;
};

class DdsDataExMapper
{
public:
	DdsDataExMapper() { }
	DdsDataExMapper(const AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateDataDto toMediateDataDto(DDSDataEx cur_data_ex,
									const AdditionalTopicInfo& info,
									MediateDataDto prev_dto = MediateDataDto());

	MediateDataDtoWithVectorsOfStruct
	toMediateDataDtoWithVectorsOfStruct(DDSDataEx, const AdditionalTopicInfo& info);

	MediateDataDtoWithVectorsOfStruct
	toMediateDataDtoWithVectorsOfStruct(DDSDataEx cur_data_ex,
										MediateDataDtoWithVectorsOfStruct prev_dto,
										const AdditionalTopicInfo& info);

	MediateDataDtoWithVectorsOfStruct toMediateDataDtoWithVectorsOfStruct(
		DDSDataEx cur_data_ex, MediateDataDtoWithVectorsOfStruct prev_dto);

private:
	AdditionalTopicInfo info_;
};

class DdsAlarmMapper
{
public:
	DdsAlarmMapper() { }
	DdsAlarmMapper(const AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateAlarmDto toMediateAlarmDto(DDSAlarm data);
	MediateAlarmDto toMediateAlarmDto(DDSAlarm data, const AdditionalTopicInfo& info);

private:
	AdditionalTopicInfo info_;
};

class DdsAlarmExMapper
{
public:
	DdsAlarmExMapper() { }
	DdsAlarmExMapper(const AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateAlarmDto toMediateAlarmDto(MediateAlarmDto prev_dto,
									  const DDSAlarmEx& cur_data_ex,
									  const AdditionalTopicInfo& info);

private:
	AdditionalTopicInfo info_;
};

class MediateDtoMapper
{
public:
	std::string toString(const MediateDataDto& dto);

	std::string toStringWithCharVectors(const MediateDataDto& dto);

	WsDataDto::Wrapper toWsDataDto(const MediateDataDto& data);

private:
	template<class OatppT, class T>
	void fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v);
	void fillVector(oatpp::Vector<oatpp::String>& oatpp_v, const std::vector<std::vector<char>>& v);
	void fillVector(oatpp::String& oatpp_v, const std::vector<char>& v);
};

#endif //!MAPPERS_H_