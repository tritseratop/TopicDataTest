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

class DdsPublisher
{
public:

    DdsPublisher();
    virtual ~DdsPublisher();

    bool init();

    void run(
        uint32_t number,
        uint32_t sleep);

    void runData(
        uint32_t number,
        uint32_t sleep);

private:
    DDSData data_;

    ConfigTopic config_topic_data_;
    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::Publisher* publisher_;
    eprosima::fastdds::dds::Topic* topic_;
    eprosima::fastdds::dds::Topic* data_topic_;
    eprosima::fastdds::dds::DataWriter* writer_;
    eprosima::fastdds::dds::DataWriter* data_writer_;

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

    class DDSDataListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        DDSDataListener(DdsPublisher* publisher)
            : matched_(0)
            , first_connected_(false)
            , pub_(publisher)
        {
        }

        ~DDSDataListener() override
        {
        }

        void on_publication_matched(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

        DdsPublisher* pub_;
        int matched_;
        bool first_connected_;

    } data_listener_;

    void runThread(
        uint32_t number,
        uint32_t sleep);

public:
    bool publish(
        eprosima::fastdds::dds::DataWriter* writer,
        const DdsPublisherListener* listener,
        uint32_t samples_sent);

    bool publishData(
        eprosima::fastdds::dds::DataWriter* writer,
        const DDSDataListener* listener,
        uint32_t samples_sent);
};

#endif /* DDSPUBLISHER_H_ */