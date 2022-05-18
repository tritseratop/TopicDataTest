#include "PackageAnalyser.h"

PackageAnalyser* PackageAnalyser::getInstance(const std::string& fname)
{
	{
		std::lock_guard<std::mutex> guard(PackageAnalyser::gen_instance_);
		if (analyser_ == nullptr)
		{
			analyser_ = new PackageAnalyser(fname);
		}
	}
	return analyser_;
}

PackageAnalyser::PackageAnalyser(const std::string& fname)
{
	start = TimeConverter::GetTime_LLmcs();
	file.open(fname, std::ofstream::out | std::ofstream::trunc);
}

PackageAnalyser::~PackageAnalyser() { }

void PackageAnalyser::setInitialInfo(std::string info)
{
	init_info = info;
}

void PackageAnalyser::addDataToAnalyse(std::string name)
{
	data_to_analyse[name];
}
void PackageAnalyser::pushDataTimestamp(std::string name, int64_t distance)
{
	data_to_analyse[name].push_back(distance);
}
void PackageAnalyser::pushPackageTimestamp(PackageTimestamp timestamp)
{
	packages_.push_back(timestamp);
}

void PackageAnalyser::writeResults() const
{
	TransitionInfo info;
	if (!init_info.empty())
	{
		file << init_info << std::endl;
	}
	if (!packages_.empty())
	{
		info.max_delivery_time = packages_.front().value().delivery_time;
		info.min_delivery_time = packages_.front().value().delivery_time;
		for (const auto& p : packages_.getDequeCopy())
		{
			info.max_delivery_time = info.max_delivery_time > p.delivery_time
										 ? info.max_delivery_time
										 : p.delivery_time;

			info.min_delivery_time = info.min_delivery_time < p.delivery_time
										 ? info.min_delivery_time
										 : p.delivery_time;

			info.delivery_time += p.delivery_time;
			info.size += p.size;
		}

		file << "Package size:\t\t\t" << packages_.front().value().size << std::endl;
		file << "Packages number:\t\t" << packages_.size() << std::endl;
		if (packages_number != 0)
		{
			file << "Losted packages:\t\t" << packages_number - packages_.size() << std::endl;
		}
		file << "Total delivery time:\t"
			 << packages_.back().value().receiving_time - packages_.front().value().receiving_time
			 << std::endl;
		file << "Sum of delivery time:\t" << info.delivery_time << std::endl;
		file << "Avrg delivery time:\t\t" << info.delivery_time / packages_.size() << std::endl;
		file << "Max delivery time:\t\t" << info.max_delivery_time << std::endl;
		file << "Min delivery time:\t\t" << info.min_delivery_time << std::endl;
		file << "First delivery time:\t\t" << packages_.front().value().delivery_time << std::endl;
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
			file << d.first + ":\t" << sum / d.second.size() << std::endl;
		}
	}
}

void PackageAnalyser::clear()
{
	packages_.clear();
	data_to_analyse.clear();
	init_info.clear();
}

PackageAnalyser* PackageAnalyser::analyser_ = nullptr;

std::mutex PackageAnalyser::gen_instance_;