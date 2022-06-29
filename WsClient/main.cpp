#include "WsClient/WSClient.hpp"

#include "DdsWsGatewayService/Utilities/PackageAnalyser.h"
#include "DdsWsGatewayService/Utilities/TestUtilities/WsTestUtility.h"
#include "DdsWsGatewayService/Utilities/WsCommonClasses.h"

void main()
{
	WsConfigure configure;
#ifdef _DEBUG
	configure.host = "127.0.0.1";
#else
	configure.host = "192.168.0.185";
	//configure.host = "127.0.0.1";
#endif
	PackageAnalyser* package_analyser = PackageAnalyser::getInstance("ws_client_data_speed.txt");
	OnMessageRead on_message_read = [package_analyser](const oatpp::String& message) {
		auto receiving_time = TimeConverter::GetTime_LLmcs();
		auto prev_size = package_analyser->returnLastPackageSize();
		if (prev_size != 0 && prev_size != message->size())
		{
			std::string init_info = "\nsize: " + std::to_string(prev_size);
			std::cout << init_info << std::endl;
			std::lock_guard<std::mutex> guard(std::mutex());
			package_analyser->writeResultsAndClear(init_info);
		}
		auto dispatch_time = getTimeFromJsonString(message);
		package_analyser->pushPackageTimestamp(
			{dispatch_time, receiving_time, receiving_time - dispatch_time, message->size()});
	};

	ThreadSafeDeque<int64_t> client_cache;
	OnMessageRead on_message_read_2 = [&client_cache](const oatpp::String& message) {
		auto receiving_time = TimeConverter::GetTime_LLmcs();
		auto dispatch_time = getTimeFromJsonString(message);
		client_cache.push_back(receiving_time - dispatch_time);
	};

	WSClient wsclient(configure);
	wsclient.run(on_message_read);

	auto cache = client_cache.getDequeCopy();
	int64_t sum = 0;
	for (auto el : cache)
	{
		sum += el;
	}

	for (const auto el : cache)
	{
		std::cout << el << " " << std::endl;
	}

	std::cout << "avrg size" << sum / cache.size() << std::endl;

	system("pause");
}