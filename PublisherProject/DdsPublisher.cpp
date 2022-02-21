#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include "DdsPublisher.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

DdsPublisher::DdsPublisher()
	: participant_(nullptr)
	, publisher_(nullptr)
	, topic_(nullptr)
	, writer_(nullptr)
	, type_(new ConfigTopicPubSubType())
	, data_listener_(this)
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

bool DdsPublisher::init()
{
	config_.subscriber_id(0);
	config_.vector_size(10000);
	config_.topictype_name("FirstType");

	data_.time_service(8888);
	data_.time_source(1111);

	DomainParticipantQos participantQos;
	participantQos.name("Participant_publisher");
	participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
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

	data_topic_ = participant_->create_topic("DDSData", "DDSData", TOPIC_QOS_DEFAULT);
	if (data_topic_ == nullptr)
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
	data_writer_ = publisher_->create_datawriter(data_topic_, DATAWRITER_QOS_DEFAULT, &data_listener_);
	if (data_writer_ == nullptr)
	{
		return false;
	}

	return true;
}

void DdsPublisher::run(
	uint32_t number,
	uint32_t sleep = 1000)
{
	uint32_t samples_sent = 0;
	while (samples_sent < number)
	{
		if (publish(writer_, &listener_, samples_sent))
		{
			samples_sent++;
			std::cout << "Subscriber id: " << config_.subscriber_id()
				<< " with vector size: " << config_.vector_size()
				<< " with topic type name: " << config_.topictype_name()
				<< " SENDED." << std::endl;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

void DdsPublisher::runData(
	uint32_t number,
	uint32_t sleep = 1000)
{
	uint32_t samples_sent = 0;
	while (samples_sent < number)
	{
		if (publishData(data_writer_, &data_listener_, samples_sent))
		{
			samples_sent++;
			std::cout << "time_service: " << data_.time_service()
				<< " with time_source: " << data_.time_source()
				<< " RECEIVED." << std::endl;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

bool DdsPublisher::publish(DataWriter* writer, const DdsPublisherListener* listener, uint32_t samples_sent)
{
	//std::lock_guard<std::mutex> guard(std::mutex());
	if (listener > 0 && listener_.first_connected_)
	{
		writer->write(&config_);
		return true;
	}
	return false;
}

bool DdsPublisher::publishData(DataWriter* writer, const DDSDataListener* listener, uint32_t samples_sent)
{
	//std::lock_guard<std::mutex> guard(std::mutex());
	if (listener > 0 && listener->first_connected_)
	{
		writer->write(&data_);
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

void DdsPublisher::DDSDataListener::on_publication_matched(
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