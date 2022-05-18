#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include "ConfigSubscriber.h"
#include "SubscriberFactory.h"
#include "Utilities/PackageAnalyser.h"
#include "Utilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "Utilities/WsCommonClasses.h"

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
	SubscriberService(const ServiceConfigForTest<SubscriberConfig>& config, DataCacher& cacher);
	virtual ~SubscriberService();

	bool initSubscribers();

private:
	bool initParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();

	bool initSubscriber(const SubscriberConfig& config);
	void deleteSubscribers();

public:
	void runSubscribers();
	void notifyingWsService();

	void changeSubscribersConfig(const ServiceConfigForTest<SubscriberConfig>& config);

	std::vector<AbstractDdsSubscriber*> getSubscribers() const;

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

	std::deque<MediateDataDto> getDataCacheCopy() const;

private:
	void setVectorSizesInDataTopic();

private:
	ServiceConfigForTest<SubscriberConfig> config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;

	SubscriberFactory factory_;
	std::vector<AbstractDdsSubscriber*> subscribers_;

	DataCacher& cacher_;
	ConfigSubscriber* config_subscriber_;

	PackageAnalyser* analyser_;

	bool stop_ws_server_;
};

#endif //!DDS_SUBSCRIBER_H_