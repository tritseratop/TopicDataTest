#ifndef WSClient_hpp
#define WSClient_hpp

#include "../DdsWsGatewayService/Utilities/WsCommonClasses.h"
#include "WSListener.hpp"

class WSClient
{
private:
	std::string login;
	const WsConfigure config;
	std::deque<int64_t> cache_;

public:
	WSClient(const WsConfigure& config_)
		: config(config_)
	{ }
	void run(const OnMessageRead& on_message_read);
	void stop();
	void setLogin(std::string login_);

	void cache(int64_t disp);
	std::deque<int64_t> getCache();
};

#endif //WSClient_hpp
