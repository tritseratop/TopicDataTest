#include "DdsPublisher.h"

int main(
    int argc,
    char** argv)
{
    std::vector<uint16_t> sizes = { 100, 500, 1000, 2000, 4000, 7500, 10000 };
    std::vector<uint32_t> v_sleep = { 100 };
    uint32_t samples = 3000;

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
            if (std::string(argv[2]) == "UDP")
            {
                transport = Transport::UDP;
            }

            if (std::string(argv[3]) == "1")
            {
                isSync = true;
            }

        }
    }

    ServiceConfig<PublisherConfig> default_service_config({
        "Participant_pub",
        transport,
        ip,
        4042,
        {"127.0.0.1"},
        {},
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
        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 1000, isSync
    };
    PublisherConfig ddsdataex_config = {
        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 1000, isSync
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

    PublisherService* mypub = new PublisherService();

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