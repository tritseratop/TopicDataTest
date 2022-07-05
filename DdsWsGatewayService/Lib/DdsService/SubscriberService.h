#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include "Lib/Common/DataCacher.h"
#include "Lib/DdsService/AbstractSubscriber.h"
#include "Lib/DdsService/ConfigSubscriber.h"
#include "Lib/DdsService/SubscriberFactory.h"
#include "Utilities/PackageAnalyser.h"

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
					  std::shared_ptr<DataCacher> cacher);
	virtual ~SubscriberService();

	bool initSubscribers();

private:
	bool initParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();

	bool initSubscriber(const SubscriberConfig& config);
	void deleteSubscribers();

public:
	void runSubscribers();
	void stopSubscribers();
	void notifyingWsService();

	void changeSubscribersConfig(const ServiceConfigForTest<SubscriberConfig>& config);

	std::vector<AbstractDdsSubscriber*> getSubscribers() const;

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

private:
	void setVectorSizesInDataTopic();

private:
	ServiceConfigForTest<SubscriberConfig> config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;

	SubscriberFactory factory_;
	std::vector<AbstractDdsSubscriber*> subscribers_;

	std::shared_ptr<DataCacher> cacher_;
	ConfigSubscriber* config_subscriber_;

	PackageAnalyser* analyser_;

	bool stop_;
};

#endif //!DDS_SUBSCRIBER_H_