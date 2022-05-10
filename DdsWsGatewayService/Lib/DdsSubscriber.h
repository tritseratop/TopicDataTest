#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include "../Utilities/ConfigTopic/ConfigTopicPubSubTypes.h"
#include "../Utilities/PackageAnalyser.h"
#include "../Utilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../Utilities/WsCommonClasses.h"
#include "DdsService/SubscriberFactory.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <fastrtps/attributes/ParticipantAttributes.h>

#include <unordered_map>
#include <vector>

class SubscriberService
{
public:
	SubscriberService(const ServiceConfigForTest<SubscriberConfig>& config,
					  std::vector<IServer*> servers);
	virtual ~SubscriberService();

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

	void changeSubsConfig(const ServiceConfigForTest<SubscriberConfig>& config);

	bool createParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();
	bool initSubscribers();
	bool createNewSubscriber(const SubscriberConfig& config);
	void runSubscribers();
	void notifyingWsService();
	std::vector<AbstractDdsSubscriber*> getSubscribers() const;
	void setVectorSizesInDataTopic();

	std::deque<MediateDataDto> getDataCacheCopy() const;

private:
	bool stop_ws_server_;

	ServiceConfigForTest<SubscriberConfig> config_;

	SubscriberFactory factory_;
	DataObserver observer_;
	DataCacher cacher_;

	PackageAnalyser* analyser_;

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
		ConfigSubscriberListener(SubscriberService* subscriber);
		~ConfigSubscriberListener() override;

		void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

		int matched_;
		uint32_t samples_; // TODO atomic??
		SubscriberService* subscriber_;
	} config_listener_;

	void deleteSubscribers();
};

#endif //!DDS_SUBSCRIBER_H_