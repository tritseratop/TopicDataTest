#include "Utilities/Dds/Configure.h"

namespace scada_ate::dds
{
TopicType StringToTopicType(std::string type_name)
{
	if (type_name == "DDSData")
	{
		return TopicType::DDS_DATA;
	}
	else if (type_name == "DDSDataEx")
	{
		return TopicType::DDS_DATA_EX;
	}
	else if (type_name == "DDSAlarm")
	{
		return TopicType::DDS_ALARM;
	}
	else if (type_name == "DDSAlarmEx")
	{
		return TopicType::DDS_EX_ALARM;
	}
	else
	{
		return TopicType::UNKNOWN;
	}
}

std::string TopicTypeToString(TopicType type)
{
	switch (type)
	{
	case TopicType::DDS_DATA:
		return "DDSData";
	case TopicType::DDS_DATA_EX:
		return "DDSDataEx";
	case TopicType::DDS_ALARM:
		return "DDSAlarm";
	case TopicType::DDS_EX_ALARM:
		return "DDSAlarmEx";
	default:
		return "";
	}
}

//template<class T>
//bool operator==(const ParticipantConfigure<T>& lhs, const ParticipantConfigure<T>& rhs)
//{
//	return lhs.domain_id == rhs.domain_id && lhs.participant_name == rhs.participant_name
//		   && lhs.transport == rhs.transport && lhs.ip == rhs.ip && lhs.port == rhs.port
//		   && lhs.whitelist == rhs.whitelist && lhs.lease_duration == rhs.lease_duration
//		   && lhs.lease_duration_announcement_period == rhs.lease_duration_announcement_period
//		   && lhs.configs == rhs.configs && lhs.MaxSizeSizeDataChar == rhs.MaxSizeSizeDataChar
//		   && lhs.MaxSizeDataCollectionInt == rhs.MaxSizeDataCollectionInt
//		   && lhs.MaxSizeDataCollectionFloat == rhs.MaxSizeDataCollectionFloat
//		   && lhs.MaxSizeDataCollectionDouble == rhs.MaxSizeDataCollectionDouble
//		   && lhs.MaxSizeDataCollectionChar == rhs.MaxSizeDataCollectionChar
//		   && lhs.MaxSizeSizeDataExVectorChar == rhs.MaxSizeSizeDataExVectorChar
//		   && lhs.MaxSizeDDSDataExVectorInt == rhs.MaxSizeDDSDataExVectorInt
//		   && lhs.MaxSizeDDSDataExVectorFloat == rhs.MaxSizeDDSDataExVectorFloat
//		   && lhs.MaxSizeDDSDataExVectorDouble == rhs.MaxSizeDDSDataExVectorDouble
//		   && lhs.MaxSizeDDSDataExVectorChar == rhs.MaxSizeDDSDataExVectorChar
//		   && lhs.MaxSizeDDSAlarmVectorAlarm == rhs.MaxSizeDDSAlarmVectorAlarm
//		   && lhs.MaxSizeDDSExVectorAlarms == rhs.MaxSizeDDSExVectorAlarms
//		   && lhs.logfile_name == rhs.logfile_name;
//}
} // namespace scada_ate::dds