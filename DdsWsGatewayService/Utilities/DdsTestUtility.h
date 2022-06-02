#ifndef TEST_UTILITY_H_
#define TEST_UTILITY_H_

#include "DdsCommonClasses.h"
#include "MediateDto.h"
#include "TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "nlohmann/json.hpp"

#include "oatpp/core/Types.hpp"

template<class T>
std::vector<T> getDefaultVector(size_t size = 0, T offset = 0);

std::vector<char> getDefaultVector(size_t size = 0, char offset = 0);

template<class T>
std::vector<T> getFilledVector(size_t size, T filler);

AdditionalTopicInfo getAdditionalTopicInfo(size_t size = 4);

std::pair<DDSData, MediateDataDto> getEqualDdsData(size_t size, size_t char_size = 1);

MediateDataDto getMediateDataDto(size_t size, size_t char_size = 1);

DDSData getDdsData(size_t size = 4, size_t char_size = 100);

DDSDataEx getDdsDataEx(size_t size = 4, size_t char_size = 100);

struct DataExUnion
{
	DDSData data_;
	DDSDataEx data_ex;
	MediateDataDto dto_to_change;
	MediateDataDto dto;
};

DataExUnion getEqualDdsDataEx(size_t size_ex = 2, size_t size_data = 4);

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

std::vector<ServiceConfigForTest<SubscriberConfig>>
createDdsServiceConfigs(const GlobalTestConditions& conditions);

std::string formMappingTestName(std::string description, const OneTestConditions& cond);

#endif //!TEST_UTILITY_H_