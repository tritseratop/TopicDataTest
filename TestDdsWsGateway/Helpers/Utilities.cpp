#include "TestDdsWsGateway/Helpers/Utilities.h"

#include "WsClient/WSClient.hpp"

#include "DdsWsGatewayService/Lib/Common/DataCacher.h"
#include "DdsWsGatewayService/Lib/WsService/Server.h"

void runWsConnection(TestCallback& test_callback, OnMessageRead& on_message_read)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		const WsConfigure ws_conf;
		auto group = std::make_shared<Group>(0);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		WSClient wsclient(ws_conf);
		std::thread client_thread(
			[&wsclient, &on_message_read]() { wsclient.run(on_message_read); });

		group->runTestMessageSending(std::move(test_callback));

		client_thread.join();
		server.stop();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}

void runWsConnection(TestCallback& test_callback,
					 OnMessageRead& on_message_read,
					 std::shared_ptr<Cacher> cacher)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		const WsConfigure ws_conf;
		auto group = std::make_shared<Group>(0, cacher);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		WSClient wsclient(ws_conf);
		std::thread client_thread(
			[&wsclient, &on_message_read]() { wsclient.run(on_message_read); });

		group->runTestMessageSending(std::move(test_callback));

		client_thread.join();
		server.stop();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}
