#ifndef WSClient_hpp
#define WSClient_hpp

#include "WsClient/WSListener.hpp"

#include "DdsWsGateway/Utilities/Ws/Config.h"

namespace scada_ate::ws
{
class WSClient
{
private:
	std::string login;
	const Config config;
	std::deque<int64_t> cache_;

public:
	WSClient(const Config& config_)
		: config(config_)
	{ }
	void run(const OnMessageRead& on_message_read);
	void stop();
	void setLogin(std::string login_);

	void cache(int64_t disp);
	std::deque<int64_t> getCache();
};
} // namespace scada_ate::ws

#endif //WSClient_hpp
