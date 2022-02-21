#include "DdsSubscriber.h"


#define TEST_MODE

#ifdef TEST_MODE

std::vector<SubscriberConfiguration> configs({
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA}
    });
#endif

int main(
    int argc,
    char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    DdsSubscriber* mysub = new DdsSubscriber();
    if (mysub->initSubscribers(configs))
    {
        mysub->runSubscribers();
    }

    delete mysub;
    return 0;
}