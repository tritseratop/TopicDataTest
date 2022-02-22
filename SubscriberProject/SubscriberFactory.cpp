#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

AbstractDdsSubscriber* SubscriberFactory::createSubscriber(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const SubscriberConfig& config) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcreteSubscriber<DDSData>(participant, config);
	case TopicType::DDS_DATA_EX:
		return new ConcreteSubscriber<DDSDataEx>(participant, config);
	case TopicType::DDS_ALARM:
		return new ConcreteSubscriber<DDSAlarm>(participant, config);
	case TopicType::DDS_EX_ALARM:
		return new ConcreteSubscriber<DDSExAlarm>(participant, config);
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