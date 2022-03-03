#ifndef APP_COMPONENT_HPP
#define APP_COMPONENT_HPP

#include "WsSocketListener.h"
#include "Temp.h"

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

class AppComponent {
private:
	const Configure config_;
public:
	AppComponent(const Configure& config_) : config_(config_) {}

	// ������� ����������� executor
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
		return std::make_shared<oatpp::async::Executor>(
			4, // ���-�� ������� data-processing 
			1, // ���-�� ������� �����/������
			1  // ���-�� ������� ��������
			);
		}());


	// ������� ���������, ������� ������� ����
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
		return oatpp::network::tcp::server::ConnectionProvider::createShared(
			{ config_.WS_HOST.c_str(), static_cast<v_uint16>(config_.WS_PORT), oatpp::network::Address::IP_4 }
		);
		}());

	// ������� ������
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
		return oatpp::web::server::HttpRouter::createShared();
		}());

	// ������� ���������, ������� ������������ (��������������?) ������� � ������� �������
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)("http", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
		return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
		}());

	// ������������/�������������� DTO 
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
		}());

	// ����������� ����� ������� ���� SocketListener, ����� �� ������ ����� ����������� �� ����������� ����� 
	OATPP_CREATE_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener)([] {
		return std::make_shared<WsSocketListener>();
		}());

	// ���������� ����������� �� ��������� websocket
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler)("websocket", [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
		OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
		auto connectionHanler = oatpp::websocket::AsyncConnectionHandler::createShared(executor);
		connectionHanler->setSocketInstanceListener(sock_listener);
		return connectionHanler;
		}());
};

#endif //!APP_COMPONENT_HPP
