#ifndef TEST_UTILITY_H_
#define TEST_UTILITY_H_

#include "TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "DdsCommonClasses.h"

template<class T>
std::vector<T> getDefaultVector(size_t size = 0, T offset = 0);

std::vector<char> getDefaultVector(size_t size = 0, char offset = 0);

template<class T>
std::vector<T> getFilledVector(size_t size, T filler);

AdditionalTopicInfo getAdditionalTopicInfo(size_t size = 4);

std::pair<DDSData, MediateDataDto> getEqualDdsData(size_t size = 4);

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

#endif //!TEST_UTILITY_H_