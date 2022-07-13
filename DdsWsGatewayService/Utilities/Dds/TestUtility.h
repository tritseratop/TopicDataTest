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

struct DdsDataUnion
{
	DDSData data;
	MediateDataDto dto;
	MappingInfo tags_info;
};

DdsDataUnion getEqualDdsData(size_t size, size_t char_size = 1);
DdsDataUnion getEqualDdsData(const VectorsForData& vectors, const MappingInfo& tag_info);

MediateDataDto getMediateDataDto(size_t size, size_t char_size = 1);
MediateDataDto getMediateDataDto(const VectorsForData& vectors, const MappingInfo& tag_info);

DDSData getDdsData(size_t size, size_t char_size = 1);
DDSData getDdsData(const VectorsForData& vectors);

DDSDataEx getDdsDataEx(size_t size, size_t char_size = 1);
DDSDataEx getDdsDataEx(const VectorsForData& vectors, const MappingInfo& tag_info);

struct DdsDataExUnion
{
	DDSData data;
	DDSDataEx data_ex;
	MediateDataDto dto_to_change;
	MediateDataDto result_dto;
	MappingInfo tags_info;
};

DdsDataExUnion getEqualDdsDataEx(size_t data_ex_size, size_t data_size, size_t char_size = 1);

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