#ifndef Controller_hpp
#define Controller_hpp

#include "oatpp-websocket/Handshaker.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace scada_ate::ws
{
class Controller : public oatpp::web::server::api::ApiController
{
private:
	typedef Controller __ControllerType;

private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
					websocket_connection_handler_,
					"server_websocket");

public:
	Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{ }

public:
	ENDPOINT_ASYNC("GET", "/", Root)
	{
		ENDPOINT_ASYNC_INIT(Root)
		const char* pageTemplate = "<html lang='en'>"
								   "<head>"
								   "<meta charset=utf-8/>"
								   "</head>"
								   "<body>"
								   "<p>Hello Async WebSocket Server!</p>"
								   "<p>"
								   "<code>websocket endpoint is: localhost:8000/ws</code>"
								   "</p>"
								   "</body>"
								   "</html>";
		Action act() override
		{
			return _return(controller->createResponse(Status::CODE_200, pageTemplate));
		}
	};

	ENDPOINT_ASYNC("GET", "ws/test/{adapter-unit-id}", WS)
	{
		ENDPOINT_ASYNC_INIT(WS) Action act() override
		{
			auto group_id = request->getPathVariable("adapter-unit-id");

			auto response = oatpp::websocket::Handshaker::serversideHandshake(
				request->getHeaders(), controller->websocket_connection_handler_);

			auto params = std::make_shared<oatpp::network::ConnectionHandler::ParameterMap>();

			(*params)["adapter-unit-id"] = group_id;

			response->setConnectionUpgradeParameters(params);

			return _return(response);
		};
	};
};
} // namespace scada_ate::ws

#include OATPP_CODEGEN_END(ApiController)

#endif // !Controller_hpp