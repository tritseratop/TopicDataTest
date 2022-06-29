#include "WsClient/WSClient.hpp"
#include "WsClient/AppComponent.hpp"

#include "oatpp-websocket/AsyncWebSocket.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/WebSocket.hpp"

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include <iostream>
#include <thread>

void WSClient::run(const OnMessageRead& on_message_read)
{
	//oatpp::base::Environment::init();
	{
		ClientAppComponent client_component(config);
		OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
						connectionProvider,
						"connectionProvider");
		OATPP_COMPONENT(
			std::shared_ptr<oatpp::async::Executor>, client_executor, "client_executor");

		auto connector = oatpp::websocket::Connector::createShared(connectionProvider);

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		client_executor->execute<ClientCoroutine>(connector, this, on_message_read);
		/*oatpp::websocket::Config config;
        config.maskOutgoingMessages = true;
        config.readBufferSize = 64;
        ClientCoroutine::SOCKET->setConfig(config);*/
		std::mutex socketWriteMutex;
		//executor->execute<ClientSenderCoroutine>(ClientCoroutine::SOCKET, &socketWriteMutex);
		/*std::thread test_handler([&executor, this, &socketWriteMutex] {
            while (true) {
                std::string msg;
                std::cin >> msg;
                std::lock_guard<std::mutex> lock(socketWriteMutex);
                executor->execute<ClientSenderCoroutine>(ClientCoroutine::SOCKET, oatpp::String(msg.c_str()));
            }
        });
        test_handler.join();*/
		client_executor->join();
	}
	//oatpp::base::Environment::destroy();
}

void WSClient::stop()
{
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, client_executor, "client_executor");
	client_executor->stop();
}

void WSClient::setLogin(std::string login_)
{
	login = login_;
}

void WSClient::cache(int64_t disp)
{
	cache_.push_back(disp);
}

std::deque<int64_t> WSClient::getCache()
{
	return cache_;
}