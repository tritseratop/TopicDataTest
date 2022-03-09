#ifndef WS_SERVER_H_
#define WS_SERVER_H_

#include "Controller.h"
#include "AppComponent.h"
#include "oatpp/network/Server.hpp"
#include "../../include/CommonClasses.h"

class WebsockServer : public IServer {
public:
	WebsockServer(const Configure& config);
	void run();
	virtual bool sendData(DataDto data) override;
	virtual bool sendAlarm(AlarmDto data) override;
private:
	const Configure config_;
};

#endif //!WS_SERVER_H_