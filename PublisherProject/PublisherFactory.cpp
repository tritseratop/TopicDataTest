#include "PublisherFactory.h"

using namespace eprosima::fastdds::dds;

bool operator==(const PublisherConfig& lhs, const PublisherConfig& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id
		&& lhs.vector_size == rhs.subscriber_id
		&& lhs.topic_name == rhs.topic_name
		&& lhs.topic_type_name == rhs.topic_type_name
		&& lhs.topic_type == rhs.topic_type;
}

AbstractDdsPublisher* PublisherFactory::createPublisher(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const PublisherConfig& config) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcretePublisher<DDSData, DDSDataPubSubType>(participant, config);
	case TopicType::DDS_DATA_EX:
		return new ConcretePublisher<DDSDataEx, DDSDataExPubSubType>(participant, config);
	case TopicType::DDS_ALARM:
		return new ConcretePublisher<DDSAlarm, DDSAlarmPubSubType>(participant, config);
	case TopicType::DDS_EX_ALARM:
		return new ConcretePublisher<DDSExAlarm, DDSExAlarmPubSubType>(participant, config);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}


TopicType string2TopicType(std::string type_name)
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
	else if (type_name == "DDSExAlarm")
	{
		return TopicType::DDS_EX_ALARM;
	}
	else
	{
		return TopicType::UNKNOWN;
	}
}

std::string TopicType2string(TopicType type)
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
		return "DDSExAlarm";
	default:
		return "";
	}
}