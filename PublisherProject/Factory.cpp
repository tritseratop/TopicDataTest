#include "PublisherProject/Factory.h"

#include "DdsWsGatewayService/Utilities/Dds/TestUtility.h"

namespace scada_ate::dds::publisher
{
using namespace eprosima::fastdds::dds;

template<>
void ConcretePublisher<DDSData, DDSDataPubSubType>::setData()
{
	data_ = getDdsData(config_.vector_size, config_.char_vector_size);
}

template<>
void ConcretePublisher<DDSDataEx, DDSDataExPubSubType>::setData()
{
	data_ = getDdsDataEx(config_.vector_size, config_.char_vector_size);
}

template<>
void ConcretePublisher<DDSAlarm, DDSAlarmPubSubType>::setData()
{ }

template<>
void ConcretePublisher<DDSAlarmEx, DDSAlarmExPubSubType>::setData()
{ }

AbstractDdsPublisher* Factory::createPublisher(
	eprosima::fastdds::dds::DomainParticipant* participant, const Config& config) const
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
		return new ConcretePublisher<DDSAlarmEx, DDSAlarmExPubSubType>(participant, config);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}
} // namespace scada_ate::dds::publisher