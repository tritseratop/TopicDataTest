#include "PublisherProject/Service.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

namespace scada_ate::dds::publisher
{
using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

Service::Service(const ParticipantConfig<Config>& config)
	: participant_(nullptr)
	, config_(config)
	, publisher_(nullptr)
	, topic_(nullptr)
	, writer_(nullptr)
	, type_(new ConfigTopicPubSubType())
	, stop(false)
{ }

Service::Service()
	: participant_(nullptr)
	, publisher_(nullptr)
	, topic_(nullptr)
	, writer_(nullptr)
	, type_(new ConfigTopicPubSubType())
	, stop(false)
{ }

Service::~Service()
{
	deletePublishers();

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

bool Service::initConfigPub()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	config_topic_data_.subscriber_id(0);
	config_topic_data_.vector_size(10000);
	config_topic_data_.topictype_name("FirstType");

	DomainParticipantQos qos;
	qos.name(config_.participant_name);

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(5,
																							   0);

	qos.transport().use_builtin_transports = false;

	std::shared_ptr<TCPv4TransportDescriptor> descriptor =
		std::make_shared<TCPv4TransportDescriptor>();

	std::vector<std::string> whitelist({"127.0.0.1"});
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

void Service::runConfigPub(uint32_t number, uint32_t sleep = 1000)
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

bool Service::initPublishers()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;
	//TODO ??? ?????????
	setVectorSizesInDataTopic();

	DomainParticipantQos qos;
	qos.name(config_.participant_name);

	qos.wire_protocol().builtin.discovery_config.leaseDuration = Duration_t(5, 0);
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(2,
																							   0);

	qos.transport().use_builtin_transports = false;

	if (config_.transport == Transport::TCP)
	{
		std::shared_ptr<TCPv4TransportDescriptor> descriptor =
			std::make_shared<TCPv4TransportDescriptor>();

		/*std::vector<std::string> whitelist({ "127.0.0.1" });
		for (std::string ip : whitelist)
		{
			descriptor->interfaceWhiteList.push_back(ip);
			std::cout << "Whitelisted " << ip << std::endl;
		}*/

		descriptor->sendBufferSize = 0;
		descriptor->receiveBufferSize = 0;

		descriptor->set_WAN_address(config_.ip);

		descriptor->add_listener_port(config_.port);

		qos.transport().user_transports.push_back(descriptor);
	}
	else
	{
		std::shared_ptr<UDPv4TransportDescriptor> descriptor =
			std::make_shared<UDPv4TransportDescriptor>();

		descriptor->sendBufferSize = 0;
		descriptor->receiveBufferSize = 0;

		qos.transport().user_transports.push_back(descriptor);
	}

	if (participant_ == nullptr)
	{
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, qos);
	}
	if (participant_ == nullptr)
	{
		return false;
	}

	if (!config_.configs.empty())
	{
		for (const auto& config : config_.configs)
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

void Service::runPublishers()
{
	std::vector<std::thread> threads;
	for (auto& pub : publishers_)
	{
		threads.push_back(std::thread([&]() { pub->run(); }));
	}
	for (auto& t : threads)
	{
		t.join();
	}
}

void Service::testRunPublishers(BeforeTopicSend& before_topic_send)
{
	std::vector<std::thread> threads;
	for (auto& pub : publishers_)
	{
		threads.push_back(std::thread([&]() { pub->testRun(before_topic_send); }));
	}
	for (auto& t : threads)
	{
		t.join();
	}
}

void Service::testRunPublishers(std::vector<BeforeTopicSend>& before_topic_send)
{
	std::vector<std::thread> threads;
	for (size_t i = 0; i < publishers_.size(); ++i)
	{
		threads.push_back(std::thread(
			[this, i, &before_topic_send]() { publishers_[i]->testRun(before_topic_send[i]); }));
	}
	for (auto& t : threads)
	{
		t.join();
	}
}

void Service::changeSubsConfig(const ParticipantConfig<Config>& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already runConfigPub" << std::endl;
	}
	else
	{
		config_ = config;
		deletePublishers();
		initPublishers();
	}
}

void Service::setData()
{
	for (auto pub : publishers_)
	{
		pub->setData();
	}
}

void Service::setDdsData(DDSData* data)
{
	for (auto pub : publishers_)
	{
		if (pub->getTopicType() == TopicType::DDS_DATA)
		{
			pub->setData(static_cast<void*>(data));
		}
	}
}

void Service::setDdsDataEx(DDSDataEx* data)
{
	for (auto pub : publishers_)
	{
		if (pub->getTopicType() == TopicType::DDS_DATA_EX)
		{
			pub->setData(static_cast<void*>(data));
		}
	}
}

bool Service::createNewPublisher(const Config& config)
{
	// TODO: ?????? ?? ?????? ? SUBSCRIBER_QOS_DEFAULT
	AbstractDdsPublisher* pub = factory_.createPublisher(participant_, config);
	if (pub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	publishers_.push_back(pub);
	return true;
}

bool Service::publish(DataWriter* writer,
					  const DdsPublisherListener* listener,
					  uint32_t samples_sent)
{
	//std::lock_guard<std::mutex> guard(std::mutex());
	if (listener != nullptr && listener_.first_connected_)
	{
		writer->write(&config_topic_data_);
		return true;
	}
	return false;
}

void Service::DdsPublisherListener::on_publication_matched(
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
				  << " is not a valid value for PublicationMatchedStatus current count change"
				  << std::endl;
	}
}

void Service::setVectorSizesInDataTopic()
{
	scada_ate::typetopics::SetMaxSizeDataChar(config_.MaxSizeSizeDataChar);
	scada_ate::typetopics::SetMaxSizeDataCollectionInt(config_.MaxSizeDataCollectionInt);
	scada_ate::typetopics::SetMaxSizeDataCollectionFloat(config_.MaxSizeDataCollectionFloat);
	scada_ate::typetopics::SetMaxSizeDataCollectionDouble(config_.MaxSizeDataCollectionDouble);
	scada_ate::typetopics::SetMaxSizeDataCollectionChar(config_.MaxSizeDataCollectionChar);

	scada_ate::typetopics::SetMaxSizeDataExVectorChar(config_.MaxSizeSizeDataExVectorChar);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorInt(config_.MaxSizeDDSDataExVectorInt);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorFloat(config_.MaxSizeDDSDataExVectorFloat);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorDouble(config_.MaxSizeDDSDataExVectorDouble);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorChar(config_.MaxSizeDDSDataExVectorChar);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSAlarmVectorAlarm);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSExVectorAlarms);
}

void Service::deletePublishers()
{
	for (auto& sub : publishers_)
	{
		delete sub;
	}
	publishers_.clear();
}
} // namespace scada_ate::dds::publisher