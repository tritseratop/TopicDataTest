#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "DdsWsGatewayService/Utilities/Ws/Configure.h"

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

namespace scada_ate::ws
{
class ClientAppComponent
{
private:
	const Configure config;

public:
	ClientAppComponent(const Configure& config_)
		: config(config_)
	{ }

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, client_executor)
	("client_executor", [] { return std::make_shared<oatpp::async::Executor>(1, 1, 1); }());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
						   connectionProvider)
	("connectionProvider", [this] {
		return oatpp::network::tcp::client::ConnectionProvider::createShared(
			{config.host.c_str(),
			 static_cast<v_uint16>(config.port),
			 oatpp::network::Address::IP_4});
	}());
};
} // namespace scada_ate::ws

#endif // !AppComponent_hpp
