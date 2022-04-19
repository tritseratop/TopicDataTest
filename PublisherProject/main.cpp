#include "DdsPublisher.h"

#define TEST_MODE
#ifdef TEST_MODE

std::vector<PublisherConfig> configs({
        {0, 100, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 100}, 
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
    std::vector<uint16_t> sizes = { 10 };
    std::vector<uint32_t> v_sleep = { 100 };
    uint32_t samples = 50;

    // изменяемые настройки
    std::string ip = "127.0.0.1";
    bool isWsServerRun = false;
    Transport transport = Transport::TCP;
    bool isSync = false;

    if (argc > 1)
    {
        ip = std::string(argv[1]);

        if (argc > 2)
        {
            if (argv[2] = "UDP")
            {
                transport = Transport::UDP;
            }

            if (argc > 3)
            {
                isSync = argv[3];
            }
            
        }
    }

    ServiceConfig<PublisherConfig> default_service_config({
        "Participant_pub",
        transport,
        ip,
        4042,
        {"127.0.0.1"},
        configs,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000
    });
    PublisherConfig ddsdata_config = {
        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 1000, 
    };
    PublisherConfig ddsdataex_config = {
        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 1000
    };

    std::vector<ServiceConfig<PublisherConfig>> configs;
    for (auto size : sizes)
    {
        ddsdata_config.vector_size = size;
        ddsdataex_config.vector_size = size;
        for (auto sleep : v_sleep)
        {
            ddsdata_config.sleep = sleep;
            default_service_config.configs = { ddsdata_config };
            configs.push_back(default_service_config);

            ddsdataex_config.sleep = sleep;
            default_service_config.configs = { ddsdataex_config };
            configs.push_back(default_service_config);

        }
    }

    PublisherService* mypub = new PublisherService(config);
    /*mypub->initPublishers();
    mypub->setData();
    mypub->runPublishers();*/

    int i = 1;
    for (auto conf : configs)
    {
        std::cout << "\n" << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_name
            << " size: " << conf.configs[0].vector_size << std::endl;
        mypub->changeSubsConfig(conf);
        mypub->setData();
        mypub->runPublishers();
    }

    delete mypub;
    system("pause");
}