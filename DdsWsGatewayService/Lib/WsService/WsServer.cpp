#include "WsServer.h"
#include "Utilities/WsTestUtility.h"

WebsockServer::WebsockServer(const Configure& config)
	: config_(config)
	, json_object_mapper(createMapper())
	, components(config)
{
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	auto myController = std::make_shared<Controller>();
	router->addController(myController);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
					serverConnectionProvider,
					"serverConnectionProvider");

	server_p = oatpp::network::Server::createShared(serverConnectionProvider, connectionHandler);

	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
}

std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>
WebsockServer::createMapper(bool useBeautifier)
{
	auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
	auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

	/* enable beautifier */
	serializeConfig->useBeautifier = useBeautifier;
	return oatpp::parser::json::mapping::ObjectMapper::createShared(serializeConfig,
																	deserializeConfig);
}

void WebsockServer::run()
{
	server_p->run([this]() { return !stop_; });

	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor, "executor");
	executor->stop();
	executor->join();
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

bool WebsockServer::sendData(WsDataDto::Wrapper data)
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->sendMessageToAllAsync(json_object_mapper->writeToString(data));
	return true;
}

bool WebsockServer::sendData(oatpp::String data)
{
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->sendMessageToAllAsync(data, this);
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
	return cache_;
}

//bool WebsockServer::sendAlarm(MediateAlarmDto data) {
//	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
//	//TODO маппинг
//	sock_listener->sendMessageToAllAsync(oatpp::String("I'm here"));
//	return true;
//}