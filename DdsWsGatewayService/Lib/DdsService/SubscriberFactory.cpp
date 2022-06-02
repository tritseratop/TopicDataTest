#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "Lib/DdsService/SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id && lhs.vector_size == rhs.subscriber_id
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType, DataCacher>::setDataSize()
{ }

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType, DataCacher>::runDataSending()
{ }

AbstractDdsSubscriber*
SubscriberFactory::createSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
									const SubscriberConfig& config,
									std::shared_ptr<void> cacher) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcreteSubscriber<DDSData, DDSDataPubSubType, DataCacher>(
			participant, config, std::static_pointer_cast<DataCacher>(cacher));
	case TopicType::DDS_DATA_EX:
		return new ConcreteSubscriber<DDSDataEx, DDSDataExPubSubType, DataCacher>(
			participant, config, std::static_pointer_cast<DataCacher>(cacher));
	case TopicType::DDS_ALARM:
		return new ConcreteSubscriber<DDSAlarm, DDSAlarmPubSubType, AlarmCacher>(
			participant, config, std::static_pointer_cast<AlarmCacher>(cacher));
	case TopicType::DDS_EX_ALARM:
		return new ConcreteSubscriber<DDSAlarmEx, DDSAlarmExPubSubType, AlarmCacher>(
			participant, config, std::static_pointer_cast<AlarmCacher>(cacher));
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}