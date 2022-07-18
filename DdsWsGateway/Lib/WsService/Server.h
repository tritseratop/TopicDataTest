#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "oatpp/network/Server.hpp"

namespace scada_ate::ws
{
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
} // namespace scada_ate::ws

#endif //!WS_SERVER_H_