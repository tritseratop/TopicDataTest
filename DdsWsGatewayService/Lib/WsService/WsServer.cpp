#include "Lib/WsService/WsServer.h"

WsServer::WsServer(const WsConfigure& config, DataCacher& cacher)
	: config_(config)
	, cacher_(cacher)
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
}

void WsServer::run()
{
	server_->run([this]() { return !stop_; });

	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	executor->stop();
	executor->join();
}

void WsServer::stop()
{
	stop_ = true;
}

bool WsServer::sendClose()
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener, "sock_listener");
	sock_listener->sendCloseToAllAsync();
	return true;
}

void WsServer::cache(int64_t disp)
{
	cache_.push_back(disp);
}

std::deque<int64_t> WsServer::getCache()
{
	return cache_.getDequeCopy();
}

//bool WsServer::sendAlarm(MediateAlarmDto data) {
//	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
//	//TODO маппинг
//	sock_listener->sendMessageToAllAsync(oatpp::String("I'm here"));
//	return true;
//}