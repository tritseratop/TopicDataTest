#include "Lib/Common/Notifier.h"
#include "Lib/DdsService/SubscriberService.h"
#include "Utilities/TestUtilities/WsTestUtility.h"

void runWsConnection(TestCallback& test_callback)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		WsConfigure ws_conf;
#ifdef _DEBUG
		ws_conf.host = "127.0.0.1";
#else
		ws_conf.host = "192.168.0.185";
		//ws_conf.host = "127.0.0.1";
#endif
		auto group = std::make_shared<Group>(0);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		group->runTestMessageSending(std::move(test_callback));

		server.stopAfterAllTaskFinished();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}

int main(int argc, char* argv[])
{
	size_t str_size = 1000;
	if (argc > 1)
	{
		str_size = strtol(argv[1], NULL, 10);
	}
	int64_t init_disp = 1'000'000'000'000'000;

	size_t number = 1800;

	std::vector<size_t> string_sizes{1000,
									 1600,
									 5000,
									 10'000,
									 50'000,
									 100'000,
									 250'000,
									 500'000,
									 1'000'000,
									 2'500'000,
									 5'000'000,
									 10'000'000};

	ThreadSafeDeque<int64_t> server_cache;
	BeforeMessageSend before_msg_send = [&server_cache](oatpp::String& message) {
		replaceTimeToJson(message);
	};

	TestCallback test_callback = [&before_msg_send, &string_sizes, number](Group& adapter) {
		while (!adapter.hasClients())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		TestPacket test_packet{1'000'000'000'000'000, "a"};
		for (auto& string_size : string_sizes)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			test_packet.str = std::string(string_size, 'a');
			for (int i = 0; i < number; ++i)
			{
				auto message = nlohmann::json(test_packet).dump();
				adapter.sendTextMessageToAllAsync(oatpp::String(message), before_msg_send);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		adapter.sendCloseToAllAsync();
	};

	runWsConnection(test_callback);
}