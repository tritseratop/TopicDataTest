#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/WsCommonClasses.h"
#include "Utilities/WsDto.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class WebsockServer
{
public:
	WebsockServer(const Configure& config, DataCacher& cacher);
	void run();
	void runTestPacketSending();
	void runDataSending();
	void stop();
	bool isConnected() const;
	DataCacher& getDataCacher();

	bool sendData(WsDataDto::Wrapper data);
	bool sendData(oatpp::String data);
	bool sendClose();
	//virtual bool sendAlarm(MediateAlarmDto data) override;

	void cache(int64_t disp);
	std::deque<int64_t> getCache();

private:
private:
	AppComponent components_;
	std::shared_ptr<oatpp::network::Server> server_;
	const Configure config_;
	bool stop_ = false;

	DataCacher& cacher_;
	ThreadSafeDeque<int64_t> cache_;
};

#endif //!WS_SERVER_H_