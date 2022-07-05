#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/WsCommonClasses.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class Server
{
public:
	Server();
	void run();

	void stopAfterAllTaskFinished();
	void stop();

	bool sendClose();

	bool sendData(oatpp::String data)
	{
		return false;
	};

private:
	std::shared_ptr<oatpp::network::Server> server_;
};

#endif //!WS_SERVER_H_