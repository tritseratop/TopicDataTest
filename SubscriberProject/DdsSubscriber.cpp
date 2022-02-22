#include <mutex>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/Domain.h>
#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

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
		participant_->delete_subscriber(config_subscriber_);
	}

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
	if (reader->take_next_sample(&(subscriber_->config_topic_data_), &info) == ReturnCode_t::RETCODE_OK)
	{
		if (info.valid_data)
		{
			samples_++;
			std::cout << "Subscriber id: " << subscriber_->config_topic_data_.subscriber_id() 
				<< " with vector size: " << subscriber_->config_topic_data_.vector_size()
				<< " with topic type name: " << subscriber_->config_topic_data_.topictype_name()
				<< " RECEIVED." << std::endl;
		}
	}
	// TODO: Создавать подписчиков по принятой инфе

}

bool DdsSubscriber::createParticipant()
{
	if (participant_ == nullptr)
	{
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, getParticipantQos());
	}
	if (participant_ == nullptr)
	{
		return false;
	}
	return true;
}


DomainParticipantQos DdsSubscriber::getParticipantQos()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	DomainParticipantQos qos;

	Locator_t initial_peer_locator;
	initial_peer_locator.kind = LOCATOR_KIND_TCPv4;
	initial_peer_locator.port = config_.port;

	std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

	for (std::string ip : config_.whitelist)
	{
		descriptor->interfaceWhiteList.push_back(ip);
		std::cout << "Whitelisted " << ip << std::endl;
	}
	IPLocator::setIPv4(initial_peer_locator, config_.ip);
	qos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator); // Publisher's meta channel

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(5, 0);
	qos.name("Participant_sub"); // TODO хз откуда брать имя
	

	qos.transport().use_builtin_transports = false;

	qos.transport().user_transports.push_back(descriptor);

	return qos;
}

bool DdsSubscriber::initSubscribers(const ServiceConfig& config)
{
	config_ = config;
	createParticipant();
	if (!config.sub_configs.empty())
	{
		for (const auto& config : config.sub_configs)
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

bool DdsSubscriber::createNewSubscriber(const SubscriberConfig& config)
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