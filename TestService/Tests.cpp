#include "../DataTransferService/DdsService/DdsSubscriber.h"
#include "../DataTransferService/WsService/WsServer.h"
#include <gtest/gtest.h>
#include "../include/TestUtility.h"

std::vector<SubscriberConfig> configs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 2, 1000},
        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 2, 1000}
    });

ServiceConfig config({
    "Participant_sub",
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

TEST(DtoTest, DdsDataConvertation) {
    auto dds_data = getEqualDdsData(4);
    DataMapper mapper;

    DataDto d = mapper.mapDdsData(std::move(dds_data.first));
    EXPECT_EQ(dds_data.second, d);

    DataExUnion data_ex_union = getEqualDdsDataEx(2);
    DataDto d2 = mapper.mapDdsDataEx(data_ex_union.data_ex, data_ex_union.dto_to_change);
    EXPECT_EQ(d2, data_ex_union.dto);

    DataDto	dto{
        100,
        {
            getFilledVector<int64_t>(2, 101),
            {0, 0},
            {'b', 'b'}
        },
        {
            getFilledVector<int64_t>(2, 101),
            {0, 0},
            {'b', 'b'}
        },
        {
            getFilledVector<int64_t>(2, 101),
            {0, 0},
            {'b', 'b'}
        },
        {
            {102, 102},
            getFilledVector(2, getFilledVector(4, 'a')),
            {'b', 'b'}
        }
    };
    DataDto d3 = mapper.mapDdsDataEx(data_ex_union.data_ex, DataDto());
    EXPECT_EQ(d3, dto);
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
    return RUN_ALL_TESTS();
}