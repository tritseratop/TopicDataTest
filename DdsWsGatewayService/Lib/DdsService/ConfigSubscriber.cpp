#include "ConfigSubscriber.h"

ConfigSubscriber::ConfigSubscriber(eprosima::fastdds::dds::DomainParticipant* participant)
	: participant_(participant)
	, subscriber_(nullptr)
	, reader_(nullptr)
	, topic_(nullptr)
	, type_(new ConfigTopicPubSubType())
	, listener_(this)
{ }

ConfigSubscriber::~ConfigSubscriber()
{
	if (participant_ != nullptr)
	{
		if (topic_ != nullptr)
		{
			participant_->delete_topic(topic_);
		}
		if (subscriber_ != nullptr)
		{
			if (reader_ != nullptr)
			{
				subscriber_->delete_datareader(reader_);
			}
			participant_->delete_subscriber(subscriber_);
		}
	}
}

bool ConfigSubscriber::initSubscriber()
{
	using namespace eprosima::fastdds::dds;

	if (participant_ == nullptr)
	{
		return false;
	}

	type_.register_type(participant_);

	topic_ = participant_->create_topic("ConfigTopic", "ConfigTopic", TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (subscriber_ == nullptr)
	{
		return false;
	}

	reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);
	if (reader_ == nullptr)
	{
		return false;
	}

	return true;
}

void ConfigSubscriber::runSubscriber(uint32_t samples)
{
	while (listener_.samples_ < samples)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

ConfigSubscriber::Listener::Listener(ConfigSubscriber* subscriber)
	: subscriber_(subscriber)
	, samples_(0)
	, matched_(0)
{ }

ConfigSubscriber::Listener::~Listener() { }

void ConfigSubscriber::Listener::on_data_available(eprosima::fastdds::dds::DataReader* reader)
{
	eprosima::fastdds::dds::SampleInfo info;
	if (reader->take_next_sample(&(subscriber_->topic_data_), &info) == ReturnCode_t::RETCODE_OK)
	{
		if (info.valid_data)
		{
			samples_++;
			std::cout << "Subscriber id: " << subscriber_->topic_data_.subscriber_id()
					  << " with vector size: " << subscriber_->topic_data_.vector_size()
					  << " with topic type name: " << subscriber_->topic_data_.topictype_name()
					  << " RECEIVED." << std::endl;
		}
	}
}

void ConfigSubscriber::Listener::on_subscription_matched(
	eprosima::fastdds::dds::DataReader* reader,
	const eprosima::fastdds::dds::SubscriptionMatchedStatus& info)
{
	if (info.current_count_change == 1)
	{
		std::cout << "Subscriber matched." << std::endl;
	}
	else if (info.current_count_change == -1)
	{
		std::cout << "Subscriber unmatched." << std::endl;
	}
	else
	{
		std::cout << info.current_count_change
				  << " is not a valid value for SubscriptionMatchedStatus current count change"
				  << std::endl;
	}
}
