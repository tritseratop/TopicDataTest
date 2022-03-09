#include "../DataTransferService/DdsService/DdsSubscriber.h"
#include "../DataTransferService/WsService/WsServer.h"
#include <gtest/gtest.h>
//#include "../include/test_runner.h"

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


void TestDataTransition()
{
    oatpp::base::Environment::init();

    Configure conf;
    WebsockServer server(conf);
    SubscriberService* mysub = new SubscriberService(config, &server);

    std::thread tcp_thread([](SubscriberService* mysub) {
        mysub->setVectorSizesInDataTopic();
        if (mysub->initSubscribers())
        {
            mysub->runSubscribers();
        }
        }, mysub);

    server.run();

    delete mysub;
    oatpp::base::Environment::destroy();
}

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}