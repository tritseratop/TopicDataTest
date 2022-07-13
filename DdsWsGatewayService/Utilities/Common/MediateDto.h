#ifndef MEDIATE_DTO_H_
#define MEDIATE_DTO_H_

#include <string>
#include <tuple>
#include <vector>

namespace scada_ate
{
template<class T>
bool operator==(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
struct DataSequence
{
	std::vector<int64_t> time_source;
	std::vector<uint32_t> id_tag;
	std::vector<T> value;
	std::vector<char> quality;

	size_t size() const
	{
		return time_source.size();
	}
	void reserve(size_t capasity)
	{
		time_source.reserve(capasity);
		id_tag.reserve(capasity);
		value.reserve(capasity);
		quality.reserve(capasity);
	}
	void resize(size_t size)
	{
		time_source.resize(size);
		id_tag.resize(size);
		value.resize(size);
		quality.resize(size);
	}

	friend bool operator==(const DataSequence& lhs, const DataSequence& rhs)
	{
		return lhs.time_source == rhs.time_source && lhs.id_tag == rhs.id_tag
			   && lhs.value == rhs.value && lhs.quality == rhs.quality;
	}
};

struct MediateDataDto
{
	int64_t time_service;

	DataSequence<int32_t> data_int;
	DataSequence<float> data_float;
	DataSequence<double> data_double;
	DataSequence<std::vector<char>> data_char;

	int64_t dispatch_time;
	std::string topic_name = "";

	friend bool operator==(const MediateDataDto& lhs, const MediateDataDto& rhs)
	{
		return lhs.time_service == rhs.time_service && lhs.data_int == rhs.data_int
			   && lhs.data_double == rhs.data_double && lhs.data_char == rhs.data_char
			   && lhs.topic_name == rhs.topic_name;
	}

	friend bool operator<(const MediateDataDto& lhs, const MediateDataDto& rhs)
	{
		return std::tie(lhs.topic_name, lhs.time_service)
			   < std::tie(rhs.topic_name, rhs.time_service);
	}
};

struct MediateAlarmDto
{
	int64_t time_service;
	std::vector<int64_t> time_source;
	std::vector<uint32_t> id_tag;
	std::vector<uint32_t> value;
	std::vector<uint32_t> quality;

	std::string topic_name = "";

	size_t size() const
	{
		return time_source.size();
	}
	void reserve(size_t capasity)
	{
		time_source.reserve(capasity);
		id_tag.reserve(capasity);
		value.reserve(capasity);
		quality.reserve(capasity);
	}
	void resize(size_t size)
	{
		time_source.resize(size);
		id_tag.resize(size);
		value.resize(size);
		quality.resize(size);
	}

	friend bool operator==(const MediateAlarmDto& lhs, const MediateAlarmDto& rhs)
	{
		return std::equal(lhs.time_source.begin(),
						  lhs.time_source.end(),
						  rhs.time_source.begin(),
						  rhs.time_source.end())
			   && std::equal(
				   lhs.id_tag.begin(), lhs.id_tag.end(), rhs.id_tag.begin(), rhs.id_tag.end())
			   && std::equal(lhs.value.begin(), lhs.value.end(), rhs.value.begin(), rhs.value.end())
			   && std::equal(
				   lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
	}
};
} // namespace scada_ate

#endif //!MEDIATE_DTO_H_