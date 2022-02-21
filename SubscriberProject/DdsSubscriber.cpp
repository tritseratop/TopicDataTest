#include <mutex>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include "DdsSubscriber.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

DdsSubscriber::DdsSubscriber()
	: participant_(nullptr)
	, config_subscriber_(nullptr)
	, config_reader_(nullptr)
	, config_topic_(nullptr)
	, config_type_(new ConfigTopicPubSubType())
	, ddsDataSubscriberCreator(new DDSDataSubscriberCreator())
	, ddsDataExSubscriberCreator(new DDSDataExSubscriberCreator())
	, config_listener_(this)
{
}

DdsSubscriber::~DdsSubscriber()
{
	// TODO: вынести в другую функцию удаление подписчиков
	for (auto& sub : subscribers_)
	{
		delete sub;
	}
	subscribers_.clear();

	// TODO: надо ли проверять на nullptr??
	participant_->delete_topic(config_topic_);
	if (config_subscriber_ != nullptr)
	{
		config_subscriber_->delete_datareader(config_reader_);
	}
	participant_->delete_subscriber(config_subscriber_);

	DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool DdsSubscriber::initConfigSubscriber()
{
	if (participant_ == nullptr)
	{
		DomainParticipantQos participantQos;
		participantQos.name("Participant_subscriber");
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
	}
	if (participant_ == nullptr)
	{
		return false;
	}

	config_type_.register_type(participant_);

	config_topic_ = participant_->create_topic("ConfigTopic", "ConfigTopic", TOPIC_QOS_DEFAULT);
	if (config_topic_ == nullptr)
	{
		return false;
	}

	config_subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (config_subscriber_ == nullptr)
	{
		return false;
	}

	config_reader_ = config_subscriber_->create_datareader(
		config_topic_, 
		DATAREADER_QOS_DEFAULT, 
		&config_listener_);
	if (config_reader_ == nullptr)
	{
		return false;
	}

	return true;
}

void DdsSubscriber::runConfigSubscriber(uint32_t samples)
{
	while (config_listener_.samples_ < samples)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

DdsSubscriber::ConfigSubscriberListener::ConfigSubscriberListener(
	DdsSubscriber* subscriber)
	: subscriber_(subscriber)
	, samples_(0)
	, matched_(0)
{
}
DdsSubscriber::ConfigSubscriberListener::~ConfigSubscriberListener()
{
}

void DdsSubscriber::ConfigSubscriberListener::on_data_available(
	eprosima::fastdds::dds::DataReader* reader)
{
	SampleInfo info;
	if (reader->take_next_sample(&(subscriber_->config_), &info) == ReturnCode_t::RETCODE_OK)
	{
		if (info.valid_data)
		{
			samples_++;
			std::cout << "Subscriber id: " << subscriber_->config_.subscriber_id() 
				<< " with vector size: " << subscriber_->config_.vector_size()
				<< " with topic type name: " << subscriber_->config_.topictype_name()
				<< " RECEIVED." << std::endl;
		}
	}
	// TODO: Создавать подписчиков по принятой инфе

}

bool DdsSubscriber::createParticipant()
{
	if (participant_ == nullptr)
	{
		DomainParticipantQos participantQos;
		participantQos.name("Participant_subscriber");
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
	}
	if (participant_ == nullptr)
	{
		return false;
	}
	return true;
}

bool DdsSubscriber::initSubscribers(const std::vector<SubscriberConfiguration>& configs)
{
	createParticipant();
	if (!configs.empty())
	{
		for (const auto& config : configs)
		{
			createNewSubscriber(config);
		}
	}
	else
	{
		std::cout << "Configuration for subscribers is not found" << std::endl;
		return false;
	}
	for (auto& sub : subscribers_)
	{
		sub->init();
	}
	return true;
}

bool DdsSubscriber::createNewSubscriber(const SubscriberConfiguration& config)
{
	// TODO: узнать че менять в SUBSCRIBER_QOS_DEFAULT
	AbstractDdsSubscriber* sub = factory_.createSubscriber(participant_, config);
	if (sub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	subscribers_.push_back(sub);
	return true;
}

void DdsSubscriber::runSubscribers()
{
	for (auto& sub : subscribers_)
	{
		sub->run(1);
	}
}

void DdsSubscriber::ConfigSubscriberListener::on_subscription_matched(
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
			<< " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
	}
}

// TODO: сделать макрос?
TypeSupport initTypeSupportByTopic(std::string topic_name, uint16_t vector_size)
{
	if (topic_name == "DDSData")
	{
		scada_ate::typetopics::SetMaxSizeDataCollectionInt(vector_size);
		scada_ate::typetopics::SetMaxSizeDataCollectionFloat(vector_size);
		scada_ate::typetopics::SetMaxSizeDataCollectionDouble(vector_size);
		scada_ate::typetopics::SetMaxSizeDataCollectionChar(vector_size);
		return TypeSupport(new DDSDataPubSubType());
	}
	else if (topic_name == "DDSDataEx")
	{
		scada_ate::typetopics::SetMaxSizeDDSDataExVectorInt(vector_size);
		scada_ate::typetopics::SetMaxSizeDDSDataExVectorFloat(vector_size);
		scada_ate::typetopics::SetMaxSizeDDSDataExVectorDouble(vector_size);
		scada_ate::typetopics::SetMaxSizeDDSDataExVectorChar(vector_size);
		return TypeSupport(new DDSDataExPubSubType());
	}
	else if (topic_name == "DDSAlarm")
	{
		//scada_ate::typetopics::SetMaxSizeDDSAlarmAlarms(vector_size);
		return TypeSupport(new DDSAlarmPubSubType());
	}
	else if (topic_name == "DDSExAlarm")
	{
		scada_ate::typetopics::SetMaxSizeDDSExVectorAlarms(vector_size);
		return TypeSupport(new DDSExAlarmPubSubType());
	}
	else
	{
		std::cout << "Topic name " << topic_name << " is not found" << std::endl;
		return TypeSupport(nullptr);
	}
}