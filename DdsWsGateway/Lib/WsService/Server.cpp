#include "Lib/WsService/Server.h"

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

namespace scada_ate::ws
{
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
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	// Остановка Executor после завершения всех задач
	executor->waitTasksFinished();
	executor->stop();
	executor->join();

	server_->stop();
}

void Server::stop()
{
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
	// Остановка Executor до завершения работы сервера
	executor->stop();
	executor->join();

	server_->stop();
}

bool Server::sendClose()
{
	OATPP_COMPONENT(std::shared_ptr<SocketListener>, sock_listener, "sock_listener");
	sock_listener->sendCloseToAllAsync();
	return true;
}
} // namespace scada_ate::ws