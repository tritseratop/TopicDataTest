#include "DdsPublisher.h"

#define TEST_MODE

#ifdef TEST_MODE

std::vector<PublisherConfig> configs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 10, 1000}, 
        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 10, 1000}
    });

ServiceConfig config({
    "Participant_pub",
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

int main(
    int argc,
    char** argv)
{

    std::cout << "Starting publisher." << std::endl;

    PublisherService* mypub = new PublisherService(config);
    if (mypub->initPublishers())
    {
        DDSData data = getDdsData();
        mypub->setDdsData(&data, sizeof(data));
        mypub->runPublishers();
    }

    delete mypub;
    return 0;
}