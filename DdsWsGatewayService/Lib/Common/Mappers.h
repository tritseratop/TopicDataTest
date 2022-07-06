#ifndef MAPPERS_H_
#define MAPPERS_H_

#include "Utilities/Common/MediateDto.h"
#include "Utilities/Common/nlohmann/json.hpp"
#include "Utilities/Dds/Configure.h"
#include "Utilities/Dds/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

#include "oatpp/core/Types.hpp"

namespace scada_ate
{
template<class T>
void pushBackContainerWithChars(std::back_insert_iterator<T> result_it,
								const std::vector<char>& chars);

std::string convertCharVectorToString(const std::vector<char>& chars);

template<class T, class DdsSample>
void fillChanged(DataSampleSequence<T>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const dds::TagToIndex& tag_to_index);

class DdsDataMapper
{
public:
	DdsDataMapper() { }
	DdsDataMapper(const dds::AdditionalTopicInfo& info)
		: info_(info)
	{ }
	MediateDataDto toMediateDataDto(DDSData data, const dds::AdditionalTopicInfo& info);

private:
	dds::AdditionalTopicInfo info_;
};

class DdsDataExMapper
{
public:
	DdsDataExMapper() { }
	DdsDataExMapper(const dds::AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateDataDto toMediateDataDto(DDSDataEx cur_data_ex,
									const dds::AdditionalTopicInfo& info,
									MediateDataDto prev_dto = MediateDataDto());

private:
	dds::AdditionalTopicInfo info_;
};

class DdsAlarmMapper
{
public:
	DdsAlarmMapper() { }
	DdsAlarmMapper(const dds::AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateAlarmDto toMediateAlarmDto(DDSAlarm data);
	MediateAlarmDto toMediateAlarmDto(DDSAlarm data, const dds::AdditionalTopicInfo& info);

private:
	dds::AdditionalTopicInfo info_;
};

class DdsAlarmExMapper
{
public:
	DdsAlarmExMapper() { }
	DdsAlarmExMapper(const dds::AdditionalTopicInfo& info)
		: info_(info)
	{ }

	MediateAlarmDto toMediateAlarmDto(MediateAlarmDto prev_dto,
									  const DDSAlarmEx& cur_data_ex,
									  const dds::AdditionalTopicInfo& info);

private:
	dds::AdditionalTopicInfo info_;
};

class MediateDtoMapper
{
public:
	std::string toString(const MediateDataDto& dto);

	std::string toStringWithCharVectors(const MediateDataDto& dto);

private:
	template<class OatppT, class T>
	void fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v);
	void fillVector(oatpp::Vector<oatpp::String>& oatpp_v, const std::vector<std::vector<char>>& v);
	void fillVector(oatpp::String& oatpp_v, const std::vector<char>& v);
};
} // namespace scada_ate

#endif //!MAPPERS_H_