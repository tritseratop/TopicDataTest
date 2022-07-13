#ifndef DDS_CONFIG_H_
#define DDS_CONFIG_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

namespace scada_ate::dds
{
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

enum DatasetType
{
	DATA_INT,
	DATA_FLOAT,
	DATA_DOUBLE,
	DATA_CHAR,
	ALARM_UINT32
};

using Tags = std::vector<uint32_t>;
using TagToIndex = std::unordered_map<uint32_t, uint32_t>;

struct MappingInfo
{
	std::unordered_map<DatasetType, Tags> tags;
	std::unordered_map<DatasetType, TagToIndex> tag_to_index;
	std::string topic_name;
	std::string info;
};

TopicType StringToTopicType(std::string type_name);

std::string TopicTypeToString(TopicType type);

struct AdditionalPackageInfo
{
	int64_t dds_dispatch_time = 0;
	int64_t dds_arrived_time = 0;
	int64_t mapping_time = 0;
	std::string info;
};

template<class T>
struct ParticipantConfig
{
	uint32_t domain_id = 0;
	// participant params
	std::string participant_name = "participant_0";

	// transport params
	Transport transport = Transport::TCP;
	std::string ip = "127.0.0.1";
	uint16_t port = 4042;
	std::vector<std::string> whitelist;

	int64_t lease_duration_ms = 20000;
	int64_t lease_duration_announcement_period_ms = 5000;

	// subs/pubs params
	std::vector<T> configs;

	// vector_size
	size_t MaxSizeSizeDataChar = 100;
	size_t MaxSizeDataCollectionInt = 100;
	size_t MaxSizeDataCollectionFloat = 100;
	size_t MaxSizeDataCollectionDouble = 100;
	size_t MaxSizeDataCollectionChar = 100;

	size_t MaxSizeSizeDataExVectorChar = 100;
	size_t MaxSizeDDSDataExVectorInt = 100;
	size_t MaxSizeDDSDataExVectorFloat = 100;
	size_t MaxSizeDDSDataExVectorDouble = 100;
	size_t MaxSizeDDSDataExVectorChar = 100;

	size_t MaxSizeDDSAlarmVectorAlarm = 100;
	size_t MaxSizeDDSExVectorAlarms = 100;

	std::string logfile_name = "log.txt";

	friend bool operator==(const ParticipantConfig& lhs, const ParticipantConfig& rhs)
	{
		return lhs.domain_id == rhs.domain_id && lhs.participant_name == rhs.participant_name
			   && lhs.transport == rhs.transport && lhs.ip == rhs.ip && lhs.port == rhs.port
			   && lhs.whitelist == rhs.whitelist && lhs.lease_duration_ms == rhs.lease_duration_ms
			   && lhs.lease_duration_announcement_period_ms
					  == rhs.lease_duration_announcement_period_ms
			   && lhs.configs == rhs.configs && lhs.MaxSizeSizeDataChar == rhs.MaxSizeSizeDataChar
			   && lhs.MaxSizeDataCollectionInt == rhs.MaxSizeDataCollectionInt
			   && lhs.MaxSizeDataCollectionFloat == rhs.MaxSizeDataCollectionFloat
			   && lhs.MaxSizeDataCollectionDouble == rhs.MaxSizeDataCollectionDouble
			   && lhs.MaxSizeDataCollectionChar == rhs.MaxSizeDataCollectionChar
			   && lhs.MaxSizeSizeDataExVectorChar == rhs.MaxSizeSizeDataExVectorChar
			   && lhs.MaxSizeDDSDataExVectorInt == rhs.MaxSizeDDSDataExVectorInt
			   && lhs.MaxSizeDDSDataExVectorFloat == rhs.MaxSizeDDSDataExVectorFloat
			   && lhs.MaxSizeDDSDataExVectorDouble == rhs.MaxSizeDDSDataExVectorDouble
			   && lhs.MaxSizeDDSDataExVectorChar == rhs.MaxSizeDDSDataExVectorChar
			   && lhs.MaxSizeDDSAlarmVectorAlarm == rhs.MaxSizeDDSAlarmVectorAlarm
			   && lhs.MaxSizeDDSExVectorAlarms == rhs.MaxSizeDDSExVectorAlarms
			   && lhs.logfile_name == rhs.logfile_name;
	}
};
} // namespace scada_ate::dds

#endif //!DDS_CONFIG_H_