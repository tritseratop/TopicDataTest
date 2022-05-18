#ifndef WSClient_hpp
#define WSClient_hpp

#include "WSListener.hpp"

class WSClient
{
private:
	std::string login;
	const Configure config;
	std::deque<int64_t> cache_;

public:
	WSClient(const Configure& config_)
		: config(config_)
	{ }
	void run();
	void stop();
	void setLogin(std::string login_);

	void cache(int64_t disp);
	std::deque<int64_t> getCache();
};

#endif //WSClient_hpp
