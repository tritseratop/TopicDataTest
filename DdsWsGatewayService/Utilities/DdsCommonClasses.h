#ifndef DDS_COMMON_CLASSES_H_
#define DDS_COMMON_CLASSES_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

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
struct ServiceConfigForTest
{
	// participant params
	std::string participant_name;

	// transport params
	Transport transport = Transport::TCP;
	std::string ip;
	uint16_t port = 4042;
	std::vector<std::string> whitelist;

	// subs/pubs params
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

	friend bool operator==(const ServiceConfigForTest& lhs, const ServiceConfigForTest& rhs)
	{
		return lhs.participant_name == rhs.participant_name && lhs.ip == rhs.ip
			   && lhs.port == rhs.port && lhs.whitelist == rhs.whitelist
			   && lhs.configs == rhs.configs;
	}
};

struct PublisherConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	uint16_t char_vector_size = 0;
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

	bool isCache = false;

	friend bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs);
};

#endif //!DDS_COMMON_CLASSES_H_