#include "DdsSubscriber.h"

int main(
    int argc,
    char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    DdsSubscriber* mysub = new DdsSubscriber();
    if (mysub->initSubscribersFromConfig())
    {
        //mysub->(static_cast<uint32_t>(samples));
    }

    delete mysub;
    return 0;
}