#ifndef WSClient_hpp
#define WSClient_hpp

#include "WSListener.hpp"

class WSClient {
private:
	std::string login;
	const Configure config;
public:
	WSClient(const Configure& config_) : config(config_) {}
	void run();
	void runWithoutCoroutine();
	void setLogin(std::string login_);
};

#endif //WSClient_hpp 

