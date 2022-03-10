#ifndef COMMON_CLASSES_H_
#define COMMON_CLASSES_H_

#include <vector>
#include <string>
#include <algorithm>

template <class T>
struct DataCollection
{
	std::vector<int64_t> time_source;
	std::vector<T> value;
	std::vector<char> quality;
	size_t size() const
	{
		return time_source.size();
	}
	void reserve(size_t capasity)
	{
		time_source.reserve(capasity);
		value.reserve(capasity);
		quality.reserve(capasity);
	}
	void resize(size_t size)
	{
		time_source.resize(size);
		value.resize(size);
		quality.resize(size);
	}

	friend bool operator==(const DataCollection& lhs, const DataCollection& rhs)
	{
		return std::equal(lhs.time_source.begin(), lhs.time_source.end(), rhs.time_source.begin(), rhs.time_source.end())
			&& std::equal(lhs.value.begin(), lhs.value.end(), rhs.value.begin(), rhs.value.end())
			&& std::equal(lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
	}
};

template<class T>
bool operator==(const DataCollection<std::vector<T>>& lhs, const DataCollection<std::vector<T>>& rhs)
{
	bool res = false;
	if (lhs.value.size() == rhs.value.size())
	{
		for (size_t i = 0; i < lhs.value.size(); ++i)
		{
			res = std::equal(lhs.value[i].begin(), lhs.value[i].end(), rhs.value[i].begin(), rhs.value[i].end());
			if (res = false) break;
		}
	}
	return std::equal(lhs.time_source.begin(), lhs.time_source.end(), rhs.time_source.begin(), rhs.time_source.end())
		&& res
		&& std::equal(lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
}

struct DataDto
{
	int64_t time_service;

	DataCollection<int32_t> data_int;
	DataCollection<float> data_float;
	DataCollection<double> data_double;
	DataCollection<std::vector<char>> data_char;

	friend bool operator==(const DataDto& lhs, const DataDto& rhs)
	{
		return lhs.time_service == rhs.time_service
			&& lhs.data_int == rhs.data_int
			&& lhs.data_double == rhs.data_double
			&& lhs.data_char == rhs.data_char;
	}
};

struct AlarmDto
{
	int64_t time_service;
	std::vector<int64_t> time_source;
	std::vector<uint32_t> value;
	std::vector<uint32_t> quality;

	size_t size() const
	{
		return time_source.size();
	}
	void reserve(size_t capasity)
	{
		time_source.reserve(capasity);
		value.reserve(capasity);
		quality.reserve(capasity);
	}
	void resize(size_t size)
	{
		time_source.resize(size);
		value.resize(size);
		quality.resize(size);
	}

	friend bool operator==(const AlarmDto& lhs, const AlarmDto& rhs)
	{
		return std::equal(lhs.time_source.begin(), lhs.time_source.end(), rhs.time_source.begin(), rhs.time_source.end())
			&& std::equal(lhs.value.begin(), lhs.value.end(), rhs.value.begin(), rhs.value.end())
			&& std::equal(lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
	}
};

class IServer
{
public:
	virtual bool sendData(DataDto data) = 0;
	virtual bool sendAlarm(AlarmDto data) = 0;
};

class Configure {
public:
	Configure(int N = 3, int M = 5
		, const std::string& wshost = "localhost", int wsport = 8081)
		: MAX_CLIENT_COUNT(N)
		, MAX_MESSAGE_BUF_COUNT(M)
		, WS_HOST(wshost.c_str())
		, WS_PORT(wsport)
	{}

	int getMaxClientCount() const {
		return MAX_CLIENT_COUNT;
	}
	int getMaxMessageBufCount() const {
		return MAX_MESSAGE_BUF_COUNT;
	}
	int getPort() const {
		return WS_PORT;
	}
	std::string getIp() const {
		return WS_HOST;
	}

public:
	int MAX_CLIENT_COUNT;
	int MAX_MESSAGE_BUF_COUNT;;
	std::string WS_HOST;
	int WS_PORT;
};

#endif //!COMMON_CLASSES_H_