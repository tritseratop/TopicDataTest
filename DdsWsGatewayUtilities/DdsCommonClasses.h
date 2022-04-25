#ifndef DDS_COMMON_CLASSES_H_
#define DDS_COMMON_CLASSES_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

enum Transport
{
	UDP, 
	TCP, 
	SHARED_MEMORY
};

enum TopicType
{
	DDS_DATA,
	DDS_DATA_EX,
	DDS_ALARM,
	DDS_EX_ALARM,
	UNKNOWN
};

enum DataCollectiionType
{
	DATA_INT,
	DATA_FLOAT,
	DATA_DOUBLE,
	DATA_CHAR,
	ALARM_UINT32
};

TopicType string2TopicType(std::string type_name);

std::string TopicType2string(TopicType type);

struct AdditionalPackageInfo
{
	int64_t dds_dispatch_time = 0;
	int64_t dds_arrived_time = 0;
	int64_t mapping_time = 0;
	std::string info;
};

using Tags = std::vector<uint32_t>;
using TagToIndex = std::unordered_map<uint32_t, uint32_t>;

struct AdditionalTopicInfo
{
	std::unordered_map<DataCollectiionType, Tags> tags;
	std::unordered_map<DataCollectiionType, TagToIndex> tag_to_index;
	std::string topic_name;
	std::string info;
};

template<class T>
struct ServiceConfig
{
	// participant params
	std::string participant_name;

	// transport params
	Transport transport = Transport::TCP;
	std::string ip;
	uint16_t port = 4042;
	std::vector<std::string> whitelist;

	// subscribers params
	std::vector<T> configs;

	// vector_size
	size_t MaxSizeSizeDataChar = 0;
	size_t MaxSizeDataCollectionInt = 0;
	size_t MaxSizeDataCollectionFloat = 0;
	size_t MaxSizeDataCollectionDouble = 0;
	size_t MaxSizeDataCollectionChar = 0;

	size_t MaxSizeSizeDataExVectorChar = 0;
	size_t MaxSizeDDSDataExVectorInt = 0;
	size_t MaxSizeDDSDataExVectorFloat = 0;
	size_t MaxSizeDDSDataExVectorDouble = 0;
	size_t MaxSizeDDSDataExVectorChar = 0;

	size_t MaxSizeDDSAlarmVectorAlarm = 0;
	size_t MaxSizeDDSExVectorAlarms = 0;

	uint32_t ws_data_sleep = 100;
	uint32_t ws_alarm_sleep = 100;

	std::string log_file_name = "log.txt";

	friend bool operator==(const ServiceConfig& lhs, const ServiceConfig& rhs)
	{
		return lhs.participant_name == rhs.participant_name
			&& lhs.ip == rhs.ip
			&& lhs.port == rhs.port
			&& lhs.whitelist == rhs.whitelist
			&& lhs.configs == rhs.configs;
	}
};

struct PublisherConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	bool isSync = false;

	friend bool operator==(const PublisherConfig& lhs, const PublisherConfig& rhs);
};


struct SubscriberConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	AdditionalTopicInfo info;

	friend bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs);
};


template <class T>
struct DataCollection
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

	friend bool operator==(const DataCollection& lhs, const DataCollection& rhs)
	{
		return std::equal(lhs.time_source.begin(), lhs.time_source.end(), rhs.time_source.begin(), rhs.time_source.end())
			&& std::equal(lhs.id_tag.begin(), lhs.id_tag.end(), rhs.id_tag.begin(), rhs.id_tag.end())
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
		&& std::equal(lhs.id_tag.begin(), lhs.id_tag.end(), rhs.id_tag.begin(), rhs.id_tag.end())
		&& res
		&& std::equal(lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
}

struct MediateDataDto
{
	int64_t time_service;

	DataCollection<int32_t> data_int;
	DataCollection<float> data_float;
	DataCollection<double> data_double;
	DataCollection<std::vector<char>> data_char;

	int64_t dispatch_time;
	std::string topic_name = "";

	friend bool operator==(const MediateDataDto& lhs, const MediateDataDto& rhs)
	{
		return lhs.time_service == rhs.time_service
			&& lhs.data_int == rhs.data_int
			&& lhs.data_double == rhs.data_double
			&& lhs.data_char == rhs.data_char
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
		return std::equal(lhs.time_source.begin(), lhs.time_source.end(), rhs.time_source.begin(), rhs.time_source.end())
			&& std::equal(lhs.id_tag.begin(), lhs.id_tag.end(), rhs.id_tag.begin(), rhs.id_tag.end())
			&& std::equal(lhs.value.begin(), lhs.value.end(), rhs.value.begin(), rhs.value.end())
			&& std::equal(lhs.quality.begin(), lhs.quality.end(), rhs.quality.begin(), rhs.quality.end());
	}
};

#endif //!DDS_COMMON_CLASSES_H_