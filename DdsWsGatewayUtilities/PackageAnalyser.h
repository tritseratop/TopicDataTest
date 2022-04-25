#ifndef PACKAGE_ANALYSER_H_
#define PACKAGE_ANALYSER_H_

#include "ThreadSafeQueue/ThreadSafeQueue.h"
#include "TimeConverter/TimeConverter.hpp"

#include <fstream>

struct PackageTimestamp
{
    int64_t dispatch_time;
    int64_t receiving_time;
    int64_t delivery_time;
    uint64_t size;
};


struct TransitionInfo
{
    int64_t delivery_time = 0;
    int64_t max_delivery_time = 0;
    int64_t min_delivery_time = 0;
    uint64_t size = 0;
};

class PackageAnalyser {

protected:
    static PackageAnalyser* analyser_;
    static std::mutex gen_instance_;
 
    // ctor
    explicit PackageAnalyser(const char* fname = "logger.txt");
    void resetStart();
    void resetStart(int64_t start);
    void addDataToAnalyse(std::string name);
    void pushDataTimestamp(std::string name, int64_t distance);
    void pushPackageTimestamp(PackageTimestamp timestamp);
    // dtor
    ~PackageAnalyser();

public:
    static PackageAnalyser* getInstance(const char* fname = "logger.txt");
    PackageAnalyser(const PackageAnalyser&) = delete;
    void operator= (const PackageAnalyser&) = delete;

    void writeResults() const;

private:
    int64_t start = 0;
    int64_t finish = 0;
    size_t dds_package_size = 0;
    size_t packages_number = 0;

    ThreadSafeDeque<PackageTimestamp> packages_;
    std::unordered_map<std::string, std::deque<int64_t>> data_to_analyse;

    mutable std::fstream file;
};

#endif //!PACKAGE_ANALYSER_H_