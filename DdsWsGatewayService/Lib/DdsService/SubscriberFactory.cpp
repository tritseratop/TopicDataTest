#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id && lhs.vector_size == rhs.subscriber_id &&
		   lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name &&
		   lhs.topic_type == rhs.topic_type;
}

template <>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::setDataSize()
{
	std::vector<int> v(10, 0);
	DataCollectionInt dataCollectionInt;
	dataCollectionInt.value(v);
	data_sample_.data_int(dataCollectionInt);
}

template <>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::runDataSending()
{ }

AbstractDdsSubscriber*
SubscriberFactory::createSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
									const SubscriberConfig& config,
									DataCacher* observer) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcreteSubscriber<DDSData, DDSDataPubSubType>(participant, config, observer);
	case TopicType::DDS_DATA_EX:
		return new ConcreteSubscriber<DDSDataEx, DDSDataExPubSubType>(
			participant, config, observer);
	case TopicType::DDS_ALARM:
		return new ConcreteSubscriber<DDSAlarm, DDSAlarmPubSubType>(participant, config, observer);
	case TopicType::DDS_EX_ALARM:
		return new ConcreteSubscriber<DDSAlarmEx, DDSAlarmExPubSubType>(
			participant, config, observer);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}