#ifndef DDSPUBLISHER_H_
#define DDSPUBLISHER_H_

#include "PublisherProject/Factory.h"

#include "DdsWsGatewayService/Utilities/Dds/Callbacks.h"
#include "DdsWsGatewayService/Utilities/Dds/ConfigTopic/ConfigTopicPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <mutex>
#include <thread>

namespace scada_ate::dds::publisher
{
class Service
{
public:
	Service(const ParticipantConfig<Config>& config);
	Service();
	virtual ~Service();

	bool initConfigPub();
	void runConfigPub(uint32_t number, uint32_t sleep);

	bool initPublishers();
	void runPublishers();
	void testRunPublishers(BeforeTopicSend&);
	void testRunPublishers(std::vector<BeforeTopicSendData>&);
	void changeSubsConfig(const ParticipantConfig<Config>& config);
	void setData();
	void setDdsData(DDSData* data);
	void setDdsDataEx(DDSDataEx* data);

	bool createNewPublisher(const Config& config);

private:
	ParticipantConfig<Config> config_;

	Factory factory_;
	std::vector<AbstractDdsPublisher*> publishers_;

	ConfigTopic config_topic_data_;
	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Publisher* publisher_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::DataWriter* writer_;

	eprosima::fastdds::dds::TypeSupport type_;

	bool stop;

	class DdsPublisherListener : public eprosima::fastdds::dds::DataWriterListener
	{
	public:
		DdsPublisherListener()
			: matched_(0)
			, first_connected_(false)
		{ }

		~DdsPublisherListener() override { }

		void on_publication_matched(
			eprosima::fastdds::dds::DataWriter* writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
		int matched_;

		bool first_connected_;

	} listener_;

	void runThread(uint32_t number, uint32_t sleep);

	void deletePublishers();
	void setVectorSizesInDataTopic();

public:
	bool publish(eprosima::fastdds::dds::DataWriter* writer,
				 const DdsPublisherListener* listener,
				 uint32_t samples_sent);
};
} // namespace scada_ate::dds::publisher

#endif /* DDSPUBLISHER_H_ */