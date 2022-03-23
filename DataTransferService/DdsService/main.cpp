#include "DdsSubscriber.h"
#include "../WsService/WsServer.h"
#include "../../include/test_runner.h"


//#define TEST_MODE
//
//#ifdef TEST_MODE
//
//std::vector<SubscriberConfig> configs({
//        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 2, 1000},
//        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 2, 1000}
//    });
//
//ServiceConfig<SubscriberConfig> config({
//    "Participant_sub",
//    Transport::TCP,
//    "127.0.0.1",
//    4042,
//    {"127.0.0.1"},
//    configs, 
//    1000,
//    1000,
//    1000,
//    1000,
//    1000,
//    1000,
//    1000,
//    1000,
//    1000,
//    1000
//    });
//
//#endif
//
//void TestDataTransition()
//{
//    std::cout << "Starting subscriber." << std::endl;
//    SubscriberService* mysub = new SubscriberService(config, nullptr);
//    mysub->setVectorSizesInDataTopic();
//    if (mysub->initSubscribers())
//    {
//        mysub->runSubscribers();
//    }
//
//    std::vector<AbstractDdsSubscriber*> subs = mysub->getSubscribers();
//    const std::deque<DDSData>* ddsData = static_cast<const std::deque<DDSData>*>(subs[0]->getData());
//
//    delete mysub;
//    oatpp::base::Environment::destroy();
//}

//int main(
//    int argc,
//    char** argv)
//{
//    //TestRunner tr;
//    //RUN_TEST(tr, TestDataTransition);
//
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
//    
//    return 0;
//}