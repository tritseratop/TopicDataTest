#ifndef PACKAGE_ANALYSER_H_
#define PACKAGE_ANALYSER_H_

#include "Utilities/Common/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/Common/TimeConverter/TimeConverter.hpp"

#include <fstream>
#include <unordered_map>

namespace scada_ate
{
struct PackageTimestamp
{
	int64_t dispatch_time;
	int64_t receiving_time;
	int64_t delivery_time;
	size_t size;
};

struct TransitionInfo
{
	int64_t delivery_time = 0;
	int64_t max_delivery_time = 0;
	int64_t min_delivery_time = 0;
	size_t size = 0;
};

class PackageAnalyser
{

protected:
	static PackageAnalyser* analyser_;
	static std::mutex gen_instance_;
	mutable std::mutex package_;

	explicit PackageAnalyser(const std::string& fname = "logger.txt");

public:
	~PackageAnalyser();

public:
	static PackageAnalyser* getInstance(const std::string& = "logger.txt");
	PackageAnalyser(const PackageAnalyser&) = delete;
	void operator=(const PackageAnalyser&) = delete;

	void addDataToAnalyse(std::string name);
	void pushDataTimestamp(std::string name, int64_t distance);
	void pushPackageTimestamp(PackageTimestamp timestamp);
	size_t returnLastPackageSize();
	void setInitialInfo(std::string info);

	void writeResultsAndClear(std::string info);

private:
	void writeResults();
	void clear();

private:
	int64_t start = 0;
	int64_t finish = 0;
	size_t dds_package_size = 0;
	size_t packages_number = 0;
	std::string init_info = "";

	ThreadSafeDeque<PackageTimestamp> packages_;
	std::unordered_map<std::string, std::deque<int64_t>> data_to_analyse;

	mutable std::fstream file;
};
} // namespace scada_ate
#endif //!PACKAGE_ANALYSER_H_