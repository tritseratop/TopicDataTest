#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "AppComponent.h"
#include "Controller.h"

#include "../../Utilities/WsCommonClasses.h"
#include "../../Utilities/WsDto.h"

#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class WebsockServer : public IServer
{
public:
	WebsockServer(const Configure& config);
	void run();
	void stop() override;
	bool isConnected() const override;
	virtual bool sendData(WsDataDto::Wrapper data) override;
	bool sendClose() override;
	//virtual bool sendAlarm(MediateAlarmDto data) override;
private:
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper;
	const Configure config_;
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>
	createMapper(bool useBeautifier = false);
	bool stop_ = false;
	AppComponent components;
	std::shared_ptr<oatpp::network::Server> server_p;
};

#endif //!WS_SERVER_H_