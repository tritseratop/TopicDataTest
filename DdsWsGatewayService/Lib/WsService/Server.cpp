#include "Lib/WsService/Server.h"

Server::Server(const WsConfigure& config, DataCacher& cacher)
	: config_(config)
	, cacher_(cacher)
{
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	auto my_controller = std::make_shared<Controller>();
	router->addController(my_controller);

	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler, "http");
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
					server_connection_provider,
					"server_connection_provider");

	server_ = oatpp::network::Server::createShared(server_connection_provider, connection_handler);
}

void Server::run()
{
	server_->run();

	// Остановка Executor после завершения работы сервера
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	executor->stop();
	executor->join();
}

void Server::stop()
{
	server_->stop();
}

bool Server::sendClose()
{
	OATPP_COMPONENT(std::shared_ptr<SocketListener>, sock_listener, "sock_listener");
	sock_listener->sendCloseToAllAsync();
	return true;
}

//bool Server::sendAlarm(MediateAlarmDto data) {
//	OATPP_COMPONENT(std::shared_ptr<SocketListener>, sock_listener);
//	//TODO маппинг
//	sock_listener->sendMessageToAllAsync(oatpp::String("I'm here"));
//	return true;
//}