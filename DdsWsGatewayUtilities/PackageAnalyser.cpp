#include "PackageAnalyser.h"

PackageAnalyser* PackageAnalyser::getInstance(const char* fname)
{
	{
		std::lock_guard<std::mutex> guard(PackageAnalyser::gen_instance_);
		if (analyser_ == nullptr) {
			analyser_ = new PackageAnalyser();
		}
	}
	return analyser_;
}

PackageAnalyser::PackageAnalyser(const char* fname)
{
	start = TimeConverter::GetTime_LLmcs();
	file.open(fname, std::ofstream::out | std::ofstream::trunc);
}

PackageAnalyser::~PackageAnalyser() {}

void PackageAnalyser::resetStart()
{
	start = TimeConverter::GetTime_LLmcs();
}
void PackageAnalyser::resetStart(int64_t start)
{
	this->start = start;
}
void PackageAnalyser::addDataToAnalyse(std::string name)
{
	data_to_analyse[name];
}
void PackageAnalyser::pushDataTimestamp(std::string name, int64_t distance)
{
	data_to_analyse.at(name).push_back(distance);
}
void PackageAnalyser::pushPackageTimestamp(PackageTimestamp timestamp)
{
	packages_.push_back(timestamp);
}

void PackageAnalyser::writeResults() const
{
	TransitionInfo info;
	if (!packages_.empty())
	{
		info.max_delivery_time = packages_.front().value().delivery_time;
		info.min_delivery_time = packages_.front().value().delivery_time;
		for (const auto& p : packages_.getDequeCopy())
		{
			info.max_delivery_time = info.max_delivery_time > p.delivery_time ?
				info.max_delivery_time : p.delivery_time;

			info.min_delivery_time = info.min_delivery_time < p.delivery_time ?
				info.min_delivery_time : p.delivery_time;

			info.delivery_time += p.delivery_time;
			info.size += p.size;
		}
		file << "Package size:\t\t\t" << dds_package_size;
		file << "Losted packages:\t\t" + packages_number - packages_.size();
		file << "Sum of delivery time:\t" + info.delivery_time;
		file << "Total delivery time:\t" + finish - start;
		file << "Average delivery time:\t" + info.delivery_time / packages_.size();
		file << "Max delivery time:\t\t" + info.max_delivery_time;
		file << "Min delivery time:\t\t" + info.min_delivery_time;
	}
	for (const auto& d : data_to_analyse)
	{
		if (!d.second.empty())
		{
			int64_t sum = 0;
			for (const auto& p : d.second)
			{
				sum += p;
			}
			file << "Average time for" + d.first + ":\t" << sum / d.second.size();
		}
	}
}

PackageAnalyser* PackageAnalyser::analyser_ = nullptr;

std::mutex PackageAnalyser::gen_instance_;