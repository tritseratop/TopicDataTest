#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

DDSDataSubscriber::DDSDataSubscriber(
	DomainParticipant* participant,
	const SubscriberConfiguration& config)
	: participant_(participant)
	, subscriber_(nullptr)
	, reader_(nullptr)
	, topic_(nullptr)
	, support_type_(new DDSDataPubSubType())
	, config_(config)
	, listener_(this)
{
}

// TODO
DDSDataSubscriber::~DDSDataSubscriber()
{}

bool DDSDataSubscriber::init()
{
	if (participant_ == nullptr)
	{
		return false;
	}

	support_type_.register_type(participant_);

	topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (subscriber_ == nullptr)
	{
		return false;
	}

	reader_ = subscriber_->create_datareader(
		topic_,
		DATAREADER_QOS_DEFAULT,
		&listener_);
	if (reader_ == nullptr)
	{
		return false;
	}

	return true;
}

void DDSDataSubscriber::run(uint32_t samples)
{
	while (listener_.samples_ < samples)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

DDSDataSubscriber::DDSDataSubscriberListener::DDSDataSubscriberListener(
	DDSDataSubscriber* subscriber)
	: matched_(0)
	, samples_(0)
	, sub_(subscriber)
{
}

DDSDataSubscriber::DDSDataSubscriberListener::~DDSDataSubscriberListener()
{
}

void DDSDataSubscriber::DDSDataSubscriberListener::on_data_available(
	eprosima::fastdds::dds::DataReader* reader)
{
	SampleInfo info;
	if (reader->take_next_sample(&data_, &info) == ReturnCode_t::RETCODE_OK)
	{
		if (info.valid_data)
		{
			samples_++;
			std::cout << "time_service: " << data_.time_service()
				<< " with time_source: " << data_.time_source()
				<< " RECEIVED." << std::endl;
			sub_->data_.push_back(data_);
		}
	}
}

void DDSDataSubscriber::DDSDataSubscriberListener::on_subscription_matched(
	eprosima::fastdds::dds::DataReader* reader,
	const eprosima::fastdds::dds::SubscriptionMatchedStatus& info)
{
	if (info.current_count_change == 1)
	{
		std::cout << "DDSDataSubscriber matched." << std::endl;
	}
	else if (info.current_count_change == -1)
	{
		std::cout << "DDSDataSubscriber unmatched." << std::endl;
	}
	else
	{
		std::cout << "DDSDataSubscriber: " << info.current_count_change
			<< " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
	}
}

DDSDataExSubscriber::DDSDataExSubscriber(
	DomainParticipant* participant,
	const SubscriberConfiguration& config)
	: participant_(participant)
	, subscriber_(nullptr)
	, reader_(nullptr)
	, topic_(nullptr)
	, support_type_(new DDSDataExPubSubType())
	, config_(config)
	, listener_(this)
{
}

// TODO
DDSDataExSubscriber::~DDSDataExSubscriber()
{
}

bool DDSDataExSubscriber::init()
{
	if (participant_ == nullptr)
	{
		return false;
	}

	support_type_.register_type(participant_);

	topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (subscriber_ == nullptr)
	{
		return false;
	}

	reader_ = subscriber_->create_datareader(
		topic_,
		DATAREADER_QOS_DEFAULT,
		&listener_);
	if (reader_ == nullptr)
	{
		return false;
	}

	return true;
}

void DDSDataExSubscriber::run(uint32_t samples)
{

}

AbstractDdsSubscriber* SubscriberFactory::createSubscriber(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const SubscriberConfiguration& config) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new DDSDataSubscriber(participant, config);
	case TopicType::DDS_DATA_EX:
		return new DDSDataSubscriber(participant, config);
	case TopicType::DDS_ALARM:
		return new DDSDataSubscriber(participant, config);
	case TopicType::DDS_EX_ALARM:
		return new DDSDataSubscriber(participant, config);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}

AbstractDdsSubscriber* DDSDataSubscriberCreator::createSubscriber(
	DomainParticipant* participant,
	const SubscriberConfiguration& config) const
{

	return new DDSDataSubscriber(participant, config);
}

AbstractDdsSubscriber* DDSDataExSubscriberCreator::createSubscriber(
	DomainParticipant* participant,
	const SubscriberConfiguration& config) const
{
	return new DDSDataExSubscriber(participant, config);
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