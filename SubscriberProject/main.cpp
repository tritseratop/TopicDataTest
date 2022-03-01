#include "DdsSubscriber.h"

#define TEST_MODE

#ifdef TEST_MODE

std::vector<SubscriberConfig> configs({
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, 10, 1000},
        {1, 10000, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 10, 1000}
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

#endif

int main(
    int argc,
    char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    DdsSubscriber* mysub = new DdsSubscriber(config);
    if (mysub->initSubscribers())
    {
        mysub->runSubscribers();
    }

    delete mysub;
    return 0;
}