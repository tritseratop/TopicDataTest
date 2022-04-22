#include "../DdsWsGateway/DdsSubscriber.h"
#include "../DdsWsGateway/WsService/WsServer.h"
#include <gtest/gtest.h>
#include "../DdsWsGatewayUtilities/DdsTestUtility.h"
#include "../DdsWsGatewayUtilities/WsTestUtility.h"
#include "../DdsWsGatewayUtilities/DataObserver.h"

std::vector<SubscriberConfig> configs({
        {0, 100, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 100},
//        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 100, 100},
//        {2, 10000, "DDSData3", "DDSData", TopicType::DDS_DATA, 100, 100},
    });

ServiceConfig<SubscriberConfig> config({
    "Participant_sub",
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

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(DtoTest, DdsDataConversation) {
    auto dds_data = getEqualDdsData(4);
    DdsTopicToMediateDtoMapper mapper;

    AdditionalTopicInfo info = getAdditionalTopicInfo(4);

    MediateDataDto d = mapper.mapDdsData(std::move(dds_data.first), info);
    EXPECT_EQ(dds_data.second, d);

    DataExUnion data_ex_union = getEqualDdsDataEx(3, 5);
    MediateDataDto d2 = mapper.mapDdsDataEx(data_ex_union.dto_to_change, data_ex_union.data_ex, info);
    EXPECT_EQ(d2, data_ex_union.dto);

    DataExUnion data_ex_union1 = getEqualDdsDataEx(3, 0);
    MediateDataDto d3 = mapper.mapDdsDataEx(data_ex_union1.dto_to_change, data_ex_union1.data_ex, info);
    EXPECT_EQ(d3, data_ex_union1.dto);

    info = getAdditionalTopicInfo(500);
    DataExUnion data_ex_union2 = getEqualDdsDataEx(500, 1000);
    MediateDataDto d4 = mapper.mapDdsDataEx(data_ex_union2.dto_to_change, data_ex_union2.data_ex, info);
    EXPECT_EQ(d4, data_ex_union2.dto);

    DataExUnion data_ex_union3 = getEqualDdsDataEx(0, 0);
    MediateDataDto d5 = mapper.mapDdsDataEx(data_ex_union3.dto_to_change, data_ex_union3.data_ex, info);
    EXPECT_EQ(d5, data_ex_union3.dto);

    DataExUnion data_ex_union4 = getEqualDdsDataEx(0, 3);
    MediateDataDto d6 = mapper.mapDdsDataEx(data_ex_union4.dto_to_change, data_ex_union4.data_ex, info);
    EXPECT_EQ(d6, data_ex_union4.dto);
}

TEST(DtoTest, WsDataConversation) {
    MediateDtoToWsDtoMapper ws_mapper;
    std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper
        = oatpp::parser::json::mapping::ObjectMapper::createShared();
    
    auto data = getWsDataUnion(20);
    auto dto = ws_mapper.mapDataDto(data.data_dto);
    EXPECT_EQ(json_object_mapper->writeToString(data.ws_dto), json_object_mapper->writeToString(dto));

    auto data1 = getWsDataUnion(0);
    auto dto1 = ws_mapper.mapDataDto(data1.data_dto);
    EXPECT_EQ(json_object_mapper->writeToString(data1.ws_dto), json_object_mapper->writeToString(dto1));
}

//TEST(DdsDataTransmitionTest, DdsDataTransmition) {
//    SubscriberService* mysub = new SubscriberService(config, nullptr);
//
//    mysub->setVectorSizesInDataTopic();
//    if (mysub->initSubscribers())
//    {
//        mysub->runSubscribers();
//    }
//
//    auto dds_data = getEqualDdsData(config.configs[0].vector_size);
//    auto data = mysub->getDataCacheCopy();
//    EXPECT_EQ(dds_data.second, data.front());
//
//    delete mysub;
//}

//TEST(DdsDataTransmitionTest, DdsDataLostedPackages) {
//    std::vector<uint16_t> sizes = { 10, 100, 1000, 10000 };
//    std::vector<uint32_t> v_sleep = { 100 };
//    uint32_t samples = 50;
//    std::string ip = "127.0.0.1";
//    Transport transport = Transport::TCP;
//    ServiceConfig<SubscriberConfig> default_service_config({
//        "Participant_sub",
//        transport,
//        ip,
//        4042,
//        {"127.0.0.1"},
//        configs,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000,
//        10000
//    });
//    SubscriberConfig ddsdata_config = {
//        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100
//    };
//    SubscriberConfig ddsdataex_config = {
//        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100
//    };
//    std::vector<ServiceConfig<SubscriberConfig>> configs;
//    for (auto size : sizes)
//    {
//        ddsdata_config.vector_size = size;
//        ddsdata_config.info = getAdditionalTopicInfo(size);
//        ddsdataex_config.vector_size = size;
//        ddsdataex_config.info = getAdditionalTopicInfo(size);
//        for (auto sleep : v_sleep)
//        {
//            ddsdata_config.sleep = sleep;
//            default_service_config.configs = { ddsdata_config };
//            configs.push_back(default_service_config);
//
//            ddsdataex_config.sleep = sleep;
//            default_service_config.configs = { ddsdataex_config };
//            configs.push_back(default_service_config);
//
//        }
//    }
//    
//    SubscriberService* mysub = new SubscriberService(default_service_config, nullptr);
//
//    int i = 1;
//    for (auto conf : configs)
//    {
//        std::cout << "\n\n" << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_type_name
//            << " size: " << conf.configs[0].vector_size << std::endl;
//        mysub->changeSubsConfig(conf);
//        mysub->runSubscribers();
//    }
//    delete mysub;
//
//}

TEST(WsDataTransmitionTest, DdsDataTransmition) {
    oatpp::base::Environment::init();

    std::vector<uint16_t> sizes = { 10 };
    std::vector<uint32_t> v_sleep = { 100 };
    uint32_t samples = 50;
    std::string ip = "127.0.0.1";
    Transport transport = Transport::TCP;

    ServiceConfig<SubscriberConfig> default_service_config({
        "Participant_sub",
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
    SubscriberConfig ddsdata_config = {
        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100
    };
    SubscriberConfig ddsdataex_config = {
        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100
    };
    std::vector<ServiceConfig<SubscriberConfig>> configs;
    for (auto size : sizes)
    {
        ddsdata_config.vector_size = size;
        ddsdata_config.info = getAdditionalTopicInfo(size);
        ddsdataex_config.vector_size = size;
        ddsdataex_config.info = getAdditionalTopicInfo(size);
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

    oatpp::base::Environment::init();

    Configure ws_conf;
    WebsockServer server(ws_conf);
    std::thread tcp_thread([](WebsockServer& server) {
        server.run();
        }, std::ref(server));

    SubscriberService* mysub = new SubscriberService(default_service_config, &server);
    
    int i = 1;
    for (auto conf : configs)
    {
        std::cout << "\n\n" << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_type_name
            << " size: " << conf.configs[0].vector_size << std::endl;
        mysub->changeSubsConfig(conf);
        std::thread ws_thread([](SubscriberService* mysub) {
            mysub->notifyingWsService();
            }, mysub);
        mysub->runSubscribers();
        ws_thread.join();
    }

    delete mysub;

    server.stop();

    std::this_thread::sleep_for(std::chrono::seconds(5000));

    oatpp::base::Environment::destroy();
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
}