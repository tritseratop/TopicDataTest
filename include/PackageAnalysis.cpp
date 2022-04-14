#include "PackageAnalysis.h"

PackageAnalyser* PackageAnalyser::getInstance() 
{
    {
        std::lock_guard<std::mutex> guard(PackageAnalyser::gen_instance_);
        if (analyser_ == nullptr) {
            analyser_ = new PackageAnalyser();
        }
    }
    return analyser_;
}

PackageAnalyser::PackageAnalyser() 
{
	log = logger::Logger::getInstance("DDS Subscriber", "logs.txt", true);
}

PackageAnalyser::~PackageAnalyser() {}

void PackageAnalyser::writeResults()
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
			info.size += p.package_size;
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Total delivery time:\t" + std::to_string(info.delivery_time);
		*log << logger::Logger::e_logType::LOG_INFO << "Average delivery time:\t" + std::to_string(info.delivery_time / packages_.size());
		*log << logger::Logger::e_logType::LOG_INFO << "Max delivery time:\t\t" + std::to_string(info.max_delivery_time);
		*log << logger::Logger::e_logType::LOG_INFO << "Min delivery time:\t\t" + std::to_string(info.min_delivery_time);
	}

	if (!dto_mapping_times_.empty())
	{
		int64_t sum = 0;
		auto deq = dto_mapping_times_.getDequeCopy();
		for (const auto& p : deq)
		{
			sum += p;
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Average dto mapping time:\t" + std::to_string(sum / dto_mapping_times_.size());
	}
	if (!ws_mapping_times_.empty())
	{
		int64_t sum = 0;
		auto deq = ws_mapping_times_.getDequeCopy();
		for (const auto& p : deq)
		{
			sum += p;
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Average wsdto mapping time:\t" + std::to_string(sum / ws_mapping_times_.size());
	}
	dto_mapping_times_.clear();
	ws_mapping_times_.clear();
	packages_.clear();
}

PackageAnalyser* PackageAnalyser::analyser_ = nullptr;

std::mutex PackageAnalyser::gen_instance_;