#include "DdsPublisher.h"
#include "../include/test_runner.h"

#define TEST_MODE

#ifdef TEST_MODE

std::vector<PublisherConfig> configs({
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, 10, 1000}, 
        {1, 10000, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 10, 1000}
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

void TestPubsCreation()
{

}

int main(
    int argc,
    char** argv)
{
    TestRunner tr;
    RUN_TEST(tr, TestPubsCreation);


    std::cout << "Starting publisher." << std::endl;

    DdsPublisher* mypub = new DdsPublisher(config);
    if (mypub->initPublishers())
    {
        mypub->runPublishers();
    }

    delete mypub;
    return 0;
}