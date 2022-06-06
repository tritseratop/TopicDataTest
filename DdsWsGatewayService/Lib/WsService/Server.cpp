#include "Lib/WsService/Server.h"

Server::Server()
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
}

void Server::stopAfterAllTaskFinished()
{
	// Остановка Executor после завершения работы сервера
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	executor->waitTasksFinished();
	executor->stop();
	executor->join();

	server_->stop();
}

void Server::stop()
{
	server_->stop();

	// Остановка Executor после завершения работы сервера
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	executor->stop();
	executor->join();
}

bool Server::sendClose()
{
	OATPP_COMPONENT(std::shared_ptr<SocketListener>, sock_listener, "sock_listener");
	sock_listener->sendCloseToAllAsync();
	return true;
}