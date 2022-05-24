#include "Lib/WsService/WsServer.h"
#include "Utilities/WsTestUtility.h"

WebsockServer::WebsockServer(const Configure& config, DataCacher& cacher)
	: config_(config)
	, cacher_(cacher)
	, components_(this, config)
{
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	auto myController = std::make_shared<Controller>();
	router->addController(myController);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler, "http");

	/*OATPP_COMPONENT(
		std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler, "server_websocket");*/
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
					server_connection_provider,
					"server_connection_provider");

	server_ = oatpp::network::Server::createShared(server_connection_provider, connection_handler);

	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
}

void WebsockServer::run()
{
	server_->run([this]() { return !stop_; });

	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	executor->stop();
	executor->join();
}

void WebsockServer::runTestPacketSending()
{
	//std::thread server_thread([this]() { server_->run([this]() { return !stop_; }); });

	while (!isConnected())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "executor");
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->runTestMessageSending(0, 1'000'000'000'000'000, 100);
	//executor->waitTasksFinished();
	//executor->stop();
	//executor->join();

	//stop();
	//server_thread.join();
}

void WebsockServer::runDataSending()
{
	while (!isConnected())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->runDataSending(100);
}

void WebsockServer::stop()
{
	stop_ = true;
}

bool WebsockServer::isConnected() const
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	return sock_listener->isConnected();
}

DataCacher& WebsockServer::getDataCacher()
{
	return cacher_;
}

bool WebsockServer::sendData(WsDataDto::Wrapper data)
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	return true;
}

bool WebsockServer::sendData(oatpp::String data)
{
	return true;
}

bool WebsockServer::sendClose()
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->sendClose();
	return true;
}

void WebsockServer::cache(int64_t disp)
{
	cache_.push_back(disp);
}

std::deque<int64_t> WebsockServer::getCache()
{
	return cache_.getDequeCopy();
}

//bool WebsockServer::sendAlarm(MediateAlarmDto data) {
//	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
//	//TODO маппинг
//	sock_listener->sendMessageToAllAsync(oatpp::String("I'm here"));
//	return true;
//}