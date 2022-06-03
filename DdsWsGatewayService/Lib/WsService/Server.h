#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/WsCommonClasses.h"
#include "Utilities/WsDto.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class Server
{
public:
	Server(const WsConfigure& config, DataCacher& cacher);
	void run();
	void stop();

	bool sendData(WsDataDto::Wrapper data)
	{
		return false;
	};
	bool sendData(oatpp::String data)
	{
		return false;
	};

	bool sendClose();

private:
	//AppComponent components_;
	std::shared_ptr<oatpp::network::Server> server_;
	const WsConfigure config_;
	bool stop_ = false;

	DataCacher& cacher_;
};

#endif //!WS_SERVER_H_