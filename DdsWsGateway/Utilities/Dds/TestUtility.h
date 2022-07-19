#ifndef TEST_UTILITY_H_
#define TEST_UTILITY_H_

#include "Utilities/Common/Generators.h"
#include "Utilities/Common/MediateDto.h"
#include "Utilities/Common/nlohmann/json.hpp"
#include "Utilities/Dds/Config.h"
#include "Utilities/Dds/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

namespace scada_ate::dds
{
MappingInfo getMappingInfo(size_t size = 4);

struct DataUnion
{
	DDSData data;
	MediateDataDto dto;
	MappingInfo tags_info;
};

DataUnion getDataUnion(size_t size, size_t char_size = 1);
DataUnion getDataUnion(const VectorsForData& vectors, const MappingInfo& tag_info);

MediateDataDto getMediateDataDto(size_t size, size_t char_size = 1);
MediateDataDto getMediateDataDto(const VectorsForData& vectors, const MappingInfo& tag_info);

DDSData getDdsData(size_t size, size_t char_size = 1);
DDSData getDdsData(const VectorsForData& vectors);

DDSDataEx getDdsDataEx(size_t size, size_t char_size = 1);
DDSDataEx getDdsDataEx(const VectorsForData& vectors, const MappingInfo& tag_info);

using DdsDataSets = std::vector<DDSData>;
DdsDataSets getDdsDataSets(size_t vector_size, size_t sequences_size, size_t char_size = 1);

std::vector<DdsDataSets> getVectorOfDdsDataSets(size_t vector_size,
												size_t sets_size,
												size_t sequences_size,
												size_t char_size = 1);

using DdsDataExSets = std::vector<DDSDataEx>;
DdsDataExSets getDdsDataExSets(size_t vector_size,
							   size_t sequences_size,
							   size_t char_size,
							   const MappingInfo& info);

std::vector<DdsDataExSets> getVectorOfDdsDataExSets(size_t vector_size,
													size_t sets_size,
													size_t sequences_size,
													size_t char_size,
													const MappingInfo& info);

struct DataExUnion
{
	DDSData data;
	DDSDataEx data_ex;
	MediateDataDto dto_to_change;
	MediateDataDto result_dto;
	MappingInfo tags_info;
};

DataExUnion getDataExUnion(size_t data_ex_size, size_t data_size, size_t char_size = 1);

std::pair<VectorsForData, VectorsForData> divideVectorsForDataToTwo(const VectorsForData& vectors,
																	size_t offset);
std::pair<MappingInfo, MappingInfo> divideMappingInfoToTwo(const MappingInfo& info, size_t offset);

template<class T>
void sortByTimeService(std::vector<T>& vector_of_datasets)
{
	std::sort(vector_of_datasets.begin(), vector_of_datasets.end(), [](const T& a, const T& b) {
		return a.time_service() < b.time_service();
	});
}

template<>
void sortByTimeService(std::vector<MediateDataDto>& vector_of_datasets);

template<>
void sortByTimeService(std::vector<MediateAlarmDto>& vector_of_datasets);

template<class T>
std::vector<T> mergeAndSortByTimeService(std::vector<std::vector<T>> vector_of_datasets)
{
	if (vector_of_datasets.empty())
	{
		return {};
	}
	std::vector<T> result;
	for (auto datasets : std::move(vector_of_datasets))
	{
		std::move(datasets.begin(), datasets.end(), std::back_inserter(result));
	}
	sortByTimeService(result);
	return result;
};

struct OneTestConditions
{
	size_t all_vectors_sizes = 0;
	size_t char_vector_sizes = 0;
	int64_t publication_interval = 100;

	bool operator==(const OneTestConditions& rhs) const;
};

struct GlobalTestConditions
{
	std::vector<OneTestConditions> conditions;
	std::string ip = "127.0.0.1";
	bool isWsServerRun = false;
	bool isDdsServerRun = false;
	bool isSync = false;
	size_t samples_number = 0;

	friend bool operator==(const GlobalTestConditions& lhs, const GlobalTestConditions& rhs);
};

GlobalTestConditions parseJsonToGlobalTestConditions(const nlohmann::json& json);

std::string formMappingTestName(std::string description, const OneTestConditions& cond);
} // namespace scada_ate::dds

#endif //!TEST_UTILITY_H_