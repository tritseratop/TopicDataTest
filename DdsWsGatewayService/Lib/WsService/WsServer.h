#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/WsCommonClasses.h"
#include "Utilities/WsDto.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class WsServer
{
public:
	WsServer(const WsConfigure& config, DataCacher& cacher);
	void run();
	void runTestPacketSending(std::vector<TestPacket> packet);
	void runDataSending();
	void stop();
	bool hasConnections() const;
	DataCacher& getDataCacher();

	bool sendData(WsDataDto::Wrapper data)
	{
		return false;
	};
	bool sendData(oatpp::String data)
	{
		return false;
	};
	bool sendClose();
	//virtual bool sendAlarm(MediateAlarmDto data) override;

	void cache(int64_t disp);
	std::deque<int64_t> getCache();

private:
	//AppComponent components_;
	std::shared_ptr<oatpp::network::Server> server_;
	const WsConfigure config_;
	bool stop_ = false;

	DataCacher& cacher_;
	ThreadSafeDeque<int64_t> cache_;
};

#endif //!WS_SERVER_H_