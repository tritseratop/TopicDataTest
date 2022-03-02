#ifndef DDSPUBLISHER_H_
#define DDSPUBLISHER_H_

#include <thread>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../ConfigTopic/ConfigTopicPubSubTypes.h"
#include "PublisherFactory.h"

struct ServiceConfig
{
    // participant params
    std::string participant_name;

    // TCP params
    std::string ip;
    uint16_t port = 4042;
    std::vector<std::string> whitelist;

    // subscribers params
    std::vector<PublisherConfig> pub_configs;

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

class PublisherService
{
public:

    PublisherService(const ServiceConfig& config);
    virtual ~PublisherService();

    bool initConfigPub();
    void runConfigPub(
        uint32_t number,
        uint32_t sleep);

    bool initPublishers();
    void runPublishers();
    void changeSubsConfig(const ServiceConfig& config);

    void setDdsData(DDSData* data, size_t size);
    void setDdsDataEx(DDSDataEx* data, size_t size);

    bool createNewPublisher(const PublisherConfig& config);

private:
    ServiceConfig config_;

    PublisherFactory factory_;
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
        {
        }

        ~DdsPublisherListener() override
        {
        }

        void on_publication_matched(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
        int matched_;

        bool first_connected_;

    } listener_;

    void runThread(
        uint32_t number,
        uint32_t sleep);

    void setVectorSizesInDataTopic();

public:
    bool publish(
        eprosima::fastdds::dds::DataWriter* writer,
        const DdsPublisherListener* listener,
        uint32_t samples_sent);
};

#endif /* DDSPUBLISHER_H_ */