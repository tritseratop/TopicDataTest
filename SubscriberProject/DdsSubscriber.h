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

	// vector_size
	size_t MaxSizeDataCollectionInt = 0;
	size_t MaxSizeDataCollectionFloat = 0;
	size_t MaxSizeDataCollectionDouble = 0;
	size_t MaxSizeDataCollectionChar = 0;

	size_t MaxSizeDDSDataExVectorInt = 0;
	size_t MaxSizeDDSDataExVectorFloat = 0;
	size_t MaxSizeDDSDataExVectorDouble = 0;
	size_t MaxSizeDDSDataExVectorChar = 0;

	size_t MaxSizeDDSAlarmVectorAlarm = 0;
	size_t MaxSizeDDSExVectorAlarms = 0;

	friend bool operator==(const ServiceConfig& lhs, const ServiceConfig& rhs);
};

class DdsSubscriber {
public: 
	DdsSubscriber(const ServiceConfig& config);
	virtual ~DdsSubscriber();

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

	void changeSubsConfig(const ServiceConfig& config);

	bool createParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();
	bool initSubscribers();
	bool createNewSubscriber(const SubscriberConfig& config);
	void runSubscribers();

private:

	ServiceConfig config_;

	SubscriberFactory factory_;

	// Контейнеры с пользовательскими типам
	std::vector<AbstractDdsSubscriber*> subscribers_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* config_subscriber_;
	eprosima::fastdds::dds::DataReader* config_reader_;
	eprosima::fastdds::dds::Topic* config_topic_; 
	eprosima::fastdds::dds::TypeSupport config_type_; // TODO не нужна как поле ?
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

	void setVectorSizesInDataTopic();
};

#endif //!DDS_SUBSCRIBER_H_