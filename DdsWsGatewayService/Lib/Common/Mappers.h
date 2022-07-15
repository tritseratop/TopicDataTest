#ifndef MAPPERS_H_
#define MAPPERS_H_

#include "Utilities/Common/MediateDto.h"
#include "Utilities/Common/nlohmann/json.hpp"
#include "Utilities/Dds/Config.h"
#include "Utilities/Dds/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

#include "oatpp/core/Types.hpp"

namespace scada_ate
{
template<typename T>
using MappingPredicate = std::function<bool(T, T)>;

template<class T>
void pushBackContainerWithChars(std::back_insert_iterator<T> result_it,
								const std::vector<char>& chars);

std::string convertCharVectorToString(const std::vector<char>& chars);

template<class T, class DdsDataSequence>
void overlapDdsDataEx(DataSequence<T>& sequence_to_change,
					  const std::vector<DdsDataSequence>& current_sequence,
					  const dds::TagToIndex& tag_to_index);

template<class T, class DdsDataSequence>
void selectOnlyDifferent(DataSequence<T> sequence_to_change,
						 const std::vector<DdsDataSequence>& current_sequence,
						 const dds::TagToIndex& tag_to_index);

template<class T, class DdsDataSequence>
bool isEqualValues(const DataSequence<T>& sequence, const DdsDataSequence& sample, uint32_t index);

class DdsDataMapper
{
public:
	DdsDataMapper() { }

	MediateDataDto toMediateDataDto(DDSData dataset, const dds::MappingInfo& info);

	MediateDataDto toMediateDataDtoOnPrevBase(DDSData current_dataset,
											  const MediateDataDto& prev_dto,
											  const dds::MappingInfo& info);
	void overlapMediateDataDto(DDSData current_dataset,
							   MediateDataDto& prev_dto,
							   const dds::MappingInfo& info);

	MediateDataDto toMediateDataDtoOnlyDifferent(const DDSData& current_dataset,
												 const MediateDataDto& prev_dto,
												 const dds::MappingInfo& info);
};

class DdsDataExMapper
{
public:
	DdsDataExMapper() { }

	MediateDataDto toMediateDataDto(const DDSDataEx& current_dataset,
									const dds::MappingInfo& info,
									const MediateDataDto& prev_dto = MediateDataDto());

	MediateDataDto toMediateDataDtoOnlyDifferent(const DDSDataEx& current_dataset,
												 const dds::MappingInfo& info,
												 MediateDataDto prev_dto = MediateDataDto());
};

class DdsAlarmMapper
{
public:
	DdsAlarmMapper() { }

	MediateAlarmDto toMediateAlarmDto(DDSAlarm dataset, const dds::MappingInfo& info);
};

class DdsAlarmExMapper
{
public:
	DdsAlarmExMapper() { }

	MediateAlarmDto toMediateAlarmDto(MediateAlarmDto prev_dto,
									  const DDSAlarmEx& current_dataset,
									  const dds::MappingInfo& info);
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