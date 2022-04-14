#ifndef PACKAGE_ANALYSIS_H_
#define PACKAGE_ANALYSIS_H_

#include "../DataTransferService/ThreadSafeQueue/ThreadSafeQueue.h"
#include "DdsCommonClasses.h"
#include "logger/Logger.h"

class PackageAnalyser {

protected:
    static PackageAnalyser* analyser_;
    static std::mutex gen_instance_;

    // ctor
    explicit PackageAnalyser();

    // dtor
    ~PackageAnalyser();

public:
    static PackageAnalyser* getInstance();


    PackageAnalyser(const PackageAnalyser&) = delete;
    void operator= (const PackageAnalyser&) = delete;

    void writeResults();

    logger::Logger* log;

    ThreadSafeDeque<PackageInfo> packages_;
    ThreadSafeDeque<int64_t> dto_mapping_times_;
    ThreadSafeDeque<int64_t> ws_mapping_times_;
};

#endif //!PACKAGE_ANALYSIS_H_