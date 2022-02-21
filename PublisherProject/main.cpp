#include "DdsPublisher.h"

int main(
    int argc,
    char** argv)
{
    std::cout << "Starting publisher." << std::endl;
    int samples = 10;

    DdsPublisher* mypub = new DdsPublisher();
    if (mypub->init())
    {
        mypub->runData(static_cast<uint32_t>(samples), 1000);
    }

    delete mypub;
    return 0;
}