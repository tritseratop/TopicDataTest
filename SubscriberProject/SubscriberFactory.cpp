#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

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

	topic_ = participant_->create_topic(config_.topic_name, config_.topic_type, TOPIC_QOS_DEFAULT);
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

	topic_ = participant_->create_topic(config_.topic_name, config_.topic_type, TOPIC_QOS_DEFAULT);
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