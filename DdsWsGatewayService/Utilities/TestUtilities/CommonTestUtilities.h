#ifndef COMMON_TEST_UTILITIES_H_
#define COMMON_TEST_UTILITIES_H_

#include <cstdint>
#include <cstdlib>
#include <vector>

struct VectorsForData
{
	int64_t time_service;

	std::vector<int64_t> time_values;

	std::vector<int32_t> int_values;
	std::vector<char> int_qualities;

	std::vector<float> float_values;
	std::vector<char> float_qualities;

	std::vector<double> double_values;
	std::vector<char> double_qualities;

	std::vector<char> char_value;
	std::vector<char> char_qualities;

	VectorsForData(size_t size, size_t char_size = 1);
};

template<class T>
std::vector<T> generateVector(size_t size)
{
	std::vector<T> result;
	result.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		result.push_back(rand());
	}
	return result;
}

template<>
std::vector<char> generateVector(size_t size);

template<class T>
std::vector<T> getFilledVector(size_t size, T filler)
{
	std::vector<T> result(size, filler);
	return result;
}

#endif //!COMMON_TEST_UTILITIES_H_