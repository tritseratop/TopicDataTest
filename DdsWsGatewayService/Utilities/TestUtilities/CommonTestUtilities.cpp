#include "Utilities/TestUtilities/CommonTestUtilities.h"

VectorsForData::VectorsForData(size_t size, size_t char_size)
	: time_service(rand())
	, time_values(generateVector<int64_t>(size))
	, int_values(generateVector<int32_t>(size))
	, int_qualities(generateVector<char>(size))
	, float_values(generateVector<float>(size))
	, float_qualities(generateVector<char>(size))
	, double_values(generateVector<double>(size))
	, double_qualities(generateVector<char>(size))
	, char_value(generateVector<char>(char_size))
	, char_qualities(generateVector<char>(size))
{ }

template<>
std::vector<char> generateVector(size_t size)
{
	static const char alphanum[] = "0123456789"
								   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
								   "abcdefghijklmnopqrstuvwxyz";
	std::vector<char> result;
	result.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		result.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
	}
	return result;
}