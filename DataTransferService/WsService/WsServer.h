#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Controller.h"
#include "AppComponent.h"
#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "../../include/WsCommonClasses.h"
#include "../../include/WsDto.h"

class WebsockServer : public IServer {
public:
	WebsockServer(const Configure& config);
	void run();
	void stop();
	virtual bool sendData(WsDataDto::Wrapper data) override;
	//virtual bool sendAlarm(AlarmDto data) override;
private:
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper;
	const Configure config_;
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> createMapper();
	bool stop_ = false;
};

#endif //!WS_SERVER_H_