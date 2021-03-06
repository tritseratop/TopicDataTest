#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include "Lib/DdsService/AbstractSubscriber.h"
#include "Lib/DdsService/ConfigSubscriber.h"
#include "Lib/DdsService/Factory.h"

#include "Lib/Common/DataCacher.h"

#include "Utilities/Common/PackageAnalyser.h"
#include "Utilities/Dds/Config.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>

#include <unordered_map>
#include <vector>

namespace scada_ate::dds::subscriber
{
class Service
{
public:
	Service(const ParticipantConfig<Config>& config);
	virtual ~Service();

	bool initSubscribers(std::unordered_map<CacheId, std::shared_ptr<void>> cachers);

private:
	bool initParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();

	bool initSubscriber(const Config& config, std::shared_ptr<void> cacher);
	void deleteSubscribers();

public:
	void runSubscribers();
	void stopSubscribers();
	void notifyingWsService();

	void changeSubscribersConfig(const ParticipantConfig<Config>& config);

	std::vector<AbstractSubscriber*> getSubscribers() const;

	bool initConfigSubscriber();
	void runConfigSubscriber(uint32_t samples);

private:
	void setVectorSizesInDataTopic();

private:
	ParticipantConfig<Config> config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;

	Factory factory_;
	std::vector<AbstractSubscriber*> subscribers_;

	ConfigSubscriber* config_subscriber_;

	PackageAnalyser* analyser_;

	bool stop_;
};
} // namespace scada_ate::dds::subscriber

#endif //!DDS_SUBSCRIBER_H_