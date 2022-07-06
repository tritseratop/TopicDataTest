#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include "Lib/DdsService/AbstractSubscriber.h"
#include "Lib/DdsService/ConfigSubscriber.h"
#include "Lib/DdsService/Factory.h"

#include "Lib/Common/DataCacher.h"

#include "Utilities/Common/PackageAnalyser.h"
#include "Utilities/Dds/Configure.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>

#include <unordered_map>
#include <vector>

namespace scada_ate::dds::subscriber
{
class Service
{
public:
	Service(const ServiceConfigForTest<Configure>& config, std::shared_ptr<DataCacher> cacher);
	virtual ~Service();

	bool initSubscribers();

private:
	bool initParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();

	bool initSubscriber(const Configure& config);
	void deleteSubscribers();

public:
	void runSubscribers();
	void stopSubscribers();
	void notifyingWsService();

	void changeSubscribersConfig(const ServiceConfigForTest<Configure>& config);

	std::vector<AbstractSubscriber*> getSubscribers() const;

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

private:
	void setVectorSizesInDataTopic();

private:
	ServiceConfigForTest<Configure> config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;

	Factory factory_;
	std::vector<AbstractSubscriber*> subscribers_;

	std::shared_ptr<DataCacher> cacher_;
	ConfigSubscriber* config_subscriber_;

	PackageAnalyser* analyser_;

	bool stop_;
};
} // namespace scada_ate::dds::subscriber

#endif //!DDS_SUBSCRIBER_H_