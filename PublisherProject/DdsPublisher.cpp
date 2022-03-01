#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>

#include "DdsPublisher.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

bool operator==(const ServiceConfig& lhs, const ServiceConfig& rhs)
{
	return lhs.participant_name == rhs.participant_name
		&& lhs.ip == rhs.ip
		&& lhs.port == rhs.port
		&& lhs.whitelist == rhs.whitelist
		&& lhs.pub_configs == rhs.pub_configs;
}

DdsPublisher::DdsPublisher(const ServiceConfig& config)
	: participant_(nullptr)
	, config_(config)
	, publisher_(nullptr)
	, topic_(nullptr)
	, writer_(nullptr)
	, type_(new ConfigTopicPubSubType())
	, stop(false)
{
}

DdsPublisher::~DdsPublisher()
{
	if (writer_ != nullptr)
	{
		publisher_->delete_datawriter(writer_);
	}
	if (publisher_ != nullptr)
	{
		participant_->delete_publisher(publisher_);
	}
	if (topic_ != nullptr)
	{
		participant_->delete_topic(topic_);
	}
	DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool DdsPublisher::initConfigPub()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	config_topic_data_.subscriber_id(0);
	config_topic_data_.vector_size(10000);
	config_topic_data_.topictype_name("FirstType");

	DomainParticipantQos qos;
	qos.name("Participant_pub");

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(5, 0);

	qos.transport().use_builtin_transports = false;

	std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

	std::vector<std::string> whitelist({ "127.0.0.1" });
	for (std::string ip : whitelist)
	{
		descriptor->interfaceWhiteList.push_back(ip);
		std::cout << "Whitelisted " << ip << std::endl;
	}

	descriptor->sendBufferSize = 0;
	descriptor->receiveBufferSize = 0;

	descriptor->set_WAN_address("127.0.0.1");

	descriptor->add_listener_port(4042);

	qos.transport().user_transports.push_back(descriptor);

	participant_ = DomainParticipantFactory::get_instance()->create_participant(0, qos);
	if (participant_ == nullptr)
	{
		return false;
	}

	// Register the Type
	type_.register_type(participant_);

	TypeSupport type(new DDSDataPubSubType());
	type.register_type(participant_);

	// Create the publications Topic
	topic_ = participant_->create_topic("ConfigTopic", "ConfigTopic", TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	// Create the Publisher
	publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
	if (publisher_ == nullptr)
	{
		return false;
	}

	// Create the DataWriter
	writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
	if (writer_ == nullptr)
	{
		return false;
	}

	return true;
}

void DdsPublisher::runConfigPub(
	uint32_t number,
	uint32_t sleep = 1000)
{
	uint32_t samples_sent = 0;
	while (samples_sent < number)
	{
		if (publish(writer_, &listener_, samples_sent))
		{
			samples_sent++;
			std::cout << "Subscriber id: " << config_topic_data_.subscriber_id()
				<< " with vector size: " << config_topic_data_.vector_size()
				<< " with topic type name: " << config_topic_data_.topictype_name()
				<< " SENDED." << std::endl;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

bool DdsPublisher::initPublishers()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	DomainParticipantQos qos;
	qos.name("Participant_pub");

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(5, 0);

	qos.transport().use_builtin_transports = false;

	std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

	std::vector<std::string> whitelist({ "127.0.0.1" });
	for (std::string ip : whitelist)
	{
		descriptor->interfaceWhiteList.push_back(ip);
		std::cout << "Whitelisted " << ip << std::endl;
	}

	descriptor->sendBufferSize = 0;
	descriptor->receiveBufferSize = 0;

	descriptor->set_WAN_address("127.0.0.1");

	descriptor->add_listener_port(4042);

	qos.transport().user_transports.push_back(descriptor);

	if (participant_ == nullptr)
	{
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, qos);
	}
	if (participant_ == nullptr)
	{
		return false;
	}

	if (!config_.pub_configs.empty())
	{
		for (const auto& config : config_.pub_configs)
		{
			createNewPublisher(config);
		}
	}
	else
	{
		std::cout << "Configuration for subscribers is not found" << std::endl;
		return false;
	}
	for (auto& pub : publishers_)
	{
		pub->init();
	}
	return true;
}
void DdsPublisher::runPublishers(
	uint32_t number,
	uint32_t sleep)
{
	for (auto& pub : publishers_)
	{
		pub->run(10, 1000);
	}
}

void DdsPublisher::changeSubsConfig(const ServiceConfig& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already runConfigPub" << std::endl;
	}
	else
	{
		config_ = config;
		initPublishers();
	}
}

bool DdsPublisher::createNewPublisher(const PublisherConfig& config)
{
	// TODO: ������ �� ������ � SUBSCRIBER_QOS_DEFAULT
	AbstractDdsPublisher* pub = factory_.createPublisher(participant_, config);
	if (pub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	publishers_.push_back(pub);
	return true;
}

bool DdsPublisher::publish(DataWriter* writer, const DdsPublisherListener* listener, uint32_t samples_sent)
{
	//std::lock_guard<std::mutex> guard(std::mutex());
	if (listener > 0 && listener_.first_connected_)
	{
		writer->write(&config_topic_data_);
		return true;
	}
	return false;
}

void DdsPublisher::DdsPublisherListener::on_publication_matched(
	eprosima::fastdds::dds::DataWriter* reader,
	const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
	if (info.current_count_change == 1)
	{
		std::cout << "Publisher matched." << std::endl;
		first_connected_ = true;
	}
	else if (info.current_count_change == -1)
	{
		std::cout << "Publisher unmatched." << std::endl;
	}
	else
	{
		std::cout << info.current_count_change
			<< " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
	}
}
