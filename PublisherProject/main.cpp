#include "DdsPublisher.h"
#include "../include/TestUtility.h"

#define TEST_MODE
#ifdef TEST_MODE

std::vector<PublisherConfig> configs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 1000}, 
//        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 100, 50},
//        {2, 10000, "DDSData3", "DDSData", TopicType::DDS_DATA, 100, 10}
    });

ServiceConfig<PublisherConfig> config({
    "Participant_pub",
    Transport::TCP,
    "127.0.0.1",
    4042,
    {"127.0.0.1"},
    configs,
    1000, 
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000
    });

#endif

DDSData getDdsData()
{
    DDSData ddsData;
    ddsData.time_source(0);
    ddsData.time_service(0);
    //std::vector<int> v(10, 1);
    //DataCollectionInt dataCollectionInt;
    //dataCollectionInt.value(v);
    //ddsData.data_int(dataCollectionInt);
    return ddsData;
}

void sendingDdsData(const ServiceConfig<PublisherConfig>& conf)
{
    PublisherService* mypub = new PublisherService(conf);
    if (mypub->initPublishers())
    {
        auto dds_data = getEqualDdsData(4).first;
        auto dds_data_ex = getEqualDdsDataEx(4).data_ex;
        //DDSData dds_data = getDdsData();
        mypub->setDdsData(&dds_data, sizeof(dds_data));
        mypub->runPublishers();
    }
    delete mypub;
}

int main(
    int argc,
    char** argv)
{
    ServiceConfig<PublisherConfig> conf({
        "Participant_pub",
        Transport::TCP,
        "127.0.0.1",
        4042,
        {"127.0.0.1"},
        configs,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000
    });
    std::cout << "Starting publisher 1" << std::endl;
    std::vector<PublisherConfig> confs1({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 100},
        });
    conf.configs = confs1;
    sendingDdsData(conf);

    std::cout << "Starting publisher 2" << std::endl;
    std::vector<PublisherConfig> confs2({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 75, 100},
        });
    conf.configs = confs2;
    sendingDdsData(conf);
    //sendingDdsData(100, 100);
    //sendingDdsData(100, 100);
    //sendingDdsData(100, 100);
    system("pause");
}