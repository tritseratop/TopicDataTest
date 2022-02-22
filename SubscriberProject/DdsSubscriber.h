#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include <unordered_map>
#include <vector>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <fastrtps/attributes/ParticipantAttributes.h>

#include "../ConfigTopic/ConfigTopicPubSubTypes.h"
#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "SubscriberFactory.h"

struct ServiceConfig
{
	// participant params
	std::string participant_name;

	// TCP params
	std::string ip;
	uint16_t port = 4042;
	std::vector<std::string> whitelist;

	// subscribers params
	std::vector<SubscriberConfig> sub_configs;
};

class DdsSubscriber {
public:
	DdsSubscriber();
	virtual ~DdsSubscriber();

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

	bool createParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();
	bool initSubscribers(const ServiceConfig& config);
	bool createNewSubscriber(const SubscriberConfig& config);
	void runSubscribers();

private:

	ServiceConfig config_;

	SubscriberFactory factory_;

	// ���������� � ����������������� �����
	std::vector<AbstractDdsSubscriber*> subscribers_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* config_subscriber_;
	eprosima::fastdds::dds::DataReader* config_reader_;
	eprosima::fastdds::dds::Topic* config_topic_; 
	eprosima::fastdds::dds::TypeSupport config_type_; // TODO �� ����� ��� ���� ?
	ConfigTopic config_topic_data_;

	class ConfigSubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		ConfigSubscriberListener(
			DdsSubscriber* subscriber);
		~ConfigSubscriberListener() override;

		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override;

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

		int matched_;
		uint32_t samples_; // TODO atomic??
		DdsSubscriber* subscriber_;
	} config_listener_;

	// ���������� � ��������
	std::vector<eprosima::fastdds::dds::Subscriber*> dds_subs_; // TODO: �������� ���� �� �� ����� ����� �������
	std::unordered_map<
		eprosima::fastdds::dds::Subscriber*,
		eprosima::fastdds::dds::DataReader*> readers_;
	std::unordered_map<
		eprosima::fastdds::dds::Subscriber*,
		eprosima::fastdds::dds::Topic*> topics_;
	//std::unordered_map<eprosima::fastdds::dds::Subscriber*, eprosima::fastdds::dds::TypeSupport> subscribers_;

	// �������
	//DDSDataSubscriberCreator* ddsDataSubscriberCreator;
	//DDSDataExSubscriberCreator* ddsDataExSubscriberCreator;
};

// TODO: ������� ������?
eprosima::fastdds::dds::TypeSupport initTypeSupportByTopic(std::string topic_name, uint16_t vector_size);

#endif //!DDS_SUBSCRIBER_H_