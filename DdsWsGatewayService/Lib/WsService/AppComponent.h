#ifndef APP_COMPONENT_HPP
#define APP_COMPONENT_HPP

#include "Lib/WsService/SocketListener.h"
#include "Utilities/WsCommonClasses.h"

#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

class AppComponent
{
private:
	const WsConfigure config_;
	std::unordered_map<int64_t, std::shared_ptr<Group>> groups_;

public:
	AppComponent(const WsConfigure& config_,
				 std::unordered_map<int64_t, std::shared_ptr<Group>> groups)
		: config_(config_)
		, groups_(groups)
	{ }

	// ������� ����������� executor
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor_)
	("server_executor", [] {
		return std::make_shared<oatpp::async::Executor>(1, // ���-�� ������� data-processing
														1, // ���-�� ������� �����/������
														1 // ���-�� ������� ��������
		);
	}());

	// ������� ���������, ������� ������� ����
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
						   connection_provider_)
	("server_connection_provider", [this] {
		return oatpp::network::tcp::server::ConnectionProvider::createShared(
			{config_.host.c_str(),
			 static_cast<v_uint16>(config_.port),
			 oatpp::network::Address::IP_4});
	}());

	// ������� ������
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router_)
	([] { return oatpp::web::server::HttpRouter::createShared(); }());

	// ������� ���������, ������� ������������ (��������������?) ������� � ������� �������
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
						   server_connection_handler_)
	("http", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
		return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
	}());

	// ������������/�������������� DTO
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper_)
	([] { return oatpp::parser::json::mapping::ObjectMapper::createShared(); }());

	// ����������� ����� ������� ���� SocketListener, ����� �� ������ ����� ����������� �� ����������� �����
	OATPP_CREATE_COMPONENT(std::shared_ptr<SocketListener>, sock_listener_)
	("sock_listener", [this] { return std::make_shared<SocketListener>(std::move(groups_)); }());

	// ���������� ����������� �� ��������� websocket
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
						   ws_connection_handler)
	("server_websocket", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "server_executor");
		OATPP_COMPONENT(std::shared_ptr<SocketListener>, sock_listener, "sock_listener");
		auto connectionHanler = oatpp::websocket::AsyncConnectionHandler::createShared(executor);
		connectionHanler->setSocketInstanceListener(sock_listener);
		return connectionHanler;
	}());
};

#endif //!APP_COMPONENT_HPP
