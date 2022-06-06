#include "../DdsWsGatewayService/Utilities/PackageAnalyser.h"
#include "../DdsWsGatewayService/Utilities/Test/WsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/WsCommonClasses.h"
#include "WSClient.hpp"

void main()
{
	WsConfigure configure;
#ifdef _DEBUG
	configure.host = "127.0.0.1";
#else
	configure.host = "192.168.0.186";
#endif
	PackageAnalyser* package_analyser = PackageAnalyser::getInstance("ws_client_data_speed.txt");
	ThreadSafeDeque<int64_t> client_cache;
	OnMessageRead on_message_read = [package_analyser](const oatpp::String& message) {
		auto prev_size = package_analyser->returnLastPackageSize();
		if (prev_size != 0 && prev_size != message->size())
		{
			std::string init_info = "\nsize: " + std::to_string(prev_size);
			std::cout << init_info << std::endl;
			package_analyser->setInitialInfo(init_info);
			package_analyser->writeResults();
			package_analyser->clear();
		}
		auto receiving_time = TimeConverter::GetTime_LLmcs();
		auto dispatch_time = getTimeFromJsonString(message);
		package_analyser->pushPackageTimestamp(
			{dispatch_time, receiving_time, receiving_time - dispatch_time, message->size()});
	};

	WSClient wsclient(configure);
	wsclient.run(on_message_read);
	system("pause");
}