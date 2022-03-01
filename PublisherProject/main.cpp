#include "DdsPublisher.h"

#define TEST_MODE

#ifdef TEST_MODE

std::vector<PublisherConfig> configs({
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA}
    });

ServiceConfig config({
    "Participant_pub",
    "127.0.0.1",
    4042,
    {"127.0.0.1"},
    configs
    });

#endif

int main(
    int argc,
    char** argv)
{
    std::cout << "Starting publisher." << std::endl;
    int samples = 10;

    DdsPublisher* mypub = new DdsPublisher(config);
    if (mypub->initPublishers())
    {
        mypub->runPublishers(static_cast<uint32_t>(samples), 1000);
    }

    delete mypub;
    return 0;
}