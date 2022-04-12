#include "WsServer.h"

WebsockServer::WebsockServer(const Configure& config) 
	: config_(config)
	, json_object_mapper(createMapper())
{
}

std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> WebsockServer::createMapper()
{
	auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
	auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

	/* enable beautifier */
	serializeConfig->useBeautifier = true;
	return oatpp::parser::json::mapping::ObjectMapper::createShared(serializeConfig, deserializeConfig);
}

void WebsockServer::run() {
	AppComponent components(config_);
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	auto myController = std::make_shared<Controller>();
	myController->addEndpointsToRouter(router);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

	oatpp::network::Server server(connectionProvider, connectionHandler);

	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	server.run();
}
	
bool WebsockServer::sendData(WsDataDto::Wrapper data) {
	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
	sock_listener->sendMessageToAllAsync(json_object_mapper->writeToString(data));
	return true;
}

//bool WebsockServer::sendAlarm(AlarmDto data) {
//	OATPP_COMPONENT(std::shared_ptr<WsSocketListener>, sock_listener);
//	//TODO маппинг
//	sock_listener->sendMessageToAllAsync(oatpp::String("I'm here"));
//	return true;
//}