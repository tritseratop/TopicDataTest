#include "../DataTransferService/DdsService/DdsSubscriber.h"
#include "../DataTransferService/WsService/WsServer.h"
#include <gtest/gtest.h>
#include "../include/TestUtility.h"

std::vector<SubscriberConfig> configs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 100},
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
    DataMapper mapper;

    DataDto d = mapper.mapDdsData(std::move(dds_data.first));
    EXPECT_EQ(dds_data.second, d);

    DataExUnion data_ex_union = getEqualDdsDataEx(3, 5);
    DataDto d2 = mapper.mapDdsDataEx(data_ex_union.data_ex, data_ex_union.dto_to_change);
    EXPECT_EQ(d2, data_ex_union.dto);

    DataExUnion data_ex_union1 = getEqualDdsDataEx(3, 0);
    DataDto d3 = mapper.mapDdsDataEx(data_ex_union1.data_ex, data_ex_union1.dto_to_change);
    EXPECT_EQ(d3, data_ex_union1.dto);

    DataExUnion data_ex_union2 = getEqualDdsDataEx(500, 1000);
    DataDto d4 = mapper.mapDdsDataEx(data_ex_union2.data_ex, data_ex_union2.dto_to_change);
    EXPECT_EQ(d4, data_ex_union2.dto);

    DataExUnion data_ex_union3 = getEqualDdsDataEx(0, 0);
    DataDto d5 = mapper.mapDdsDataEx(data_ex_union3.data_ex, data_ex_union3.dto_to_change);
    EXPECT_EQ(d5, data_ex_union3.dto);

    DataExUnion data_ex_union4 = getEqualDdsDataEx(0, 3);
    DataDto d6 = mapper.mapDdsDataEx(data_ex_union4.data_ex, data_ex_union4.dto_to_change);
    EXPECT_EQ(d6, data_ex_union4.dto);
}

TEST(DdsDataTransmitionTest, DdsDataTransmition) {
    SubscriberService* mysub = new SubscriberService(config, nullptr);

    mysub->setVectorSizesInDataTopic();
    if (mysub->initSubscribers())
    {
        mysub->runSubscribers();
    }

    auto dds_data = getEqualDdsData(4);
    auto data = mysub->getDataCacheCopy();
    EXPECT_EQ(dds_data.second, data.front());

    delete mysub;
}

void recievingDdsData(const ServiceConfig<SubscriberConfig>& conf)
{
    SubscriberService* mysub = new SubscriberService(conf, nullptr);

    mysub->setVectorSizesInDataTopic();
    if (mysub->initSubscribers())
    {
        mysub->runSubscribers();
    }

    auto dds_data = getEqualDdsData(4);
    auto data = mysub->getDataCacheCopy();

    size_t sum = 0;
    for (const auto& c : conf.configs)
    {
        sum += c.samples;
    }

    EXPECT_EQ(data.size(), sum);

    delete mysub;
}

TEST(DdsDataTransmitionTest, DdsDataLostedPackages) {
    ServiceConfig<SubscriberConfig> conf({
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
            1000
        });
    std::vector<SubscriberConfig> confs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 75, 100},
        });

    conf.configs = confs;
    recievingDdsData(conf);
    //recievingDdsData(100, 100);
    //recievingDdsData(100, 200);
    //recievingDdsData(100, 500);
    //recievingDdsData(100, 1000);
}

//TEST(WsDataTransmitionTest, DdsDataTransmition) {
//    oatpp::base::Environment::init();
//
//    Configure conf;
//    WebsockServer server(conf);
//    SubscriberService* mysub = new SubscriberService(config, &server);
//
//    std::thread tcp_thread([](SubscriberService* mysub) {
//        mysub->setVectorSizesInDataTopic();
//        if (mysub->initSubscribers())
//        {
//            mysub->runSubscribers();
//        }
//        }, mysub);
//
//    server.run();
//
//    delete mysub;
//    oatpp::base::Environment::destroy();
//}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
}