#ifndef APP_COMPONENT_HPP
#define APP_COMPONENT_HPP

#include "Lib/WsService/WsSocketListener.h"
#include "Utilities/WsCommonClasses.h"

#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

class AppComponent
{
private:
	const Configure config_;
	WebsockServer* server_;

public:
	AppComponent(WebsockServer* server, const Configure& config_)
		: config_(config_)
		, server_(server)
	{ }

	// создает асинхронный executor
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor_)
	("server_executor", [] {
		return std::make_shared<oatpp::async::Executor>(4, // кол-во потоков data-processing
														1, // кол-во потоков ввода/вывода
														1 // кол-во потоков таймеров
		);
	}());

	// создает компонент, который слушает порт
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
						   connection_provider_)
	("server_connection_provider", [this] {
		return oatpp::network::tcp::server::ConnectionProvider::createShared(
			{config_.WS_HOST.c_str(),
			 static_cast<v_uint16>(config_.WS_PORT),
			 oatpp::network::Address::IP_4});
	}());

	// создает роутер
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)
	([] { return oatpp::web::server::HttpRouter::createShared(); }());

	// создает компонент, который обрабатывает (перенаправляет?) запросы с помощью роутера
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
						   serverConnectionHandler)
	("http", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
		return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
	}());

	// сериализация/десериализация DTO
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper_)
	([] { return oatpp::parser::json::mapping::ObjectMapper::createShared(); }());

	// Обязательно нужно создать ОДИН SocketListener, чтобы на каждое новое подключение не создавались новые
	OATPP_CREATE_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener_)
	([this] { return std::make_shared<WsSocketListener>(server_); }());

	// обработчик подключения по протоколу websocket
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, wsConnectionHandler)
	("server_websocket", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
		OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
		auto connectionHanler = oatpp::websocket::AsyncConnectionHandler::createShared(executor);
		connectionHanler->setSocketInstanceListener(sock_listener);
		return connectionHanler;
	}());
};

#endif //!APP_COMPONENT_HPP
