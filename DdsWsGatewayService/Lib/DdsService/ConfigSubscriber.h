#ifndef CONFIG_SUBSCRIBER_H_
#define CONFIG_SUBSCRIBER_H_

#include "Utilities/ConfigTopic/ConfigTopicPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

class ConfigSubscriber
{
public:
	ConfigSubscriber(eprosima::fastdds::dds::DomainParticipant* participant);
	~ConfigSubscriber();

	bool initSubscriber();
	void runSubscriber(uint32_t samples);

private:
	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport type_;
	ConfigTopic topic_data_;

	class Listener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		Listener(ConfigSubscriber* subscriber);
		~Listener() override;

		void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

		ConfigSubscriber* subscriber_;
		uint32_t samples_; // TODO atomic??
		int matched_;
	} listener_;
};

#endif //!CONFIG_SUBSCRIBER_H_