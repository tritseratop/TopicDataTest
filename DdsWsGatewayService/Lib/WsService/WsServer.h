#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Lib/WsService/AppComponent.h"
#include "Lib/WsService/Controller.h"

#include "Utilities/WsCommonClasses.h"
#include "Utilities/WsDto.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class WebsockServer
{
public:
	WebsockServer(const Configure& config);
	void run();
	void stop();
	bool isConnected() const;
	bool sendData(WsDataDto::Wrapper data);
	bool sendData(oatpp::String data);
	bool sendClose();
	//virtual bool sendAlarm(MediateAlarmDto data) override;

	void cache(int64_t disp);
	std::deque<int64_t> getCache();

private:
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>
	createMapper(bool useBeautifier = false);

	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper;
	const Configure config_;
	bool stop_ = false;
	AppComponent components;
	std::shared_ptr<oatpp::network::Server> server_p;

	std::deque<int64_t> cache_;
};

#endif //!WS_SERVER_H_