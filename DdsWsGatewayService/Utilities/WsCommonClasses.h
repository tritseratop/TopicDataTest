#ifndef WS_COMMON_CLASSES_H_
#define WS_COMMON_CLASSES_H_

#include "WsDto.h"

struct Configure
{
	int MAX_CLIENT_COUNT = 3;
	int MAX_MESSAGE_BUF_COUNT = 5;
	;
	std::string WS_HOST = "localhost";
	int WS_PORT = 8081;
};

class IServer
{
public:
	virtual bool sendData(WsDataDto::Wrapper data) = 0;
	virtual void stop() = 0;
	virtual bool isConnected() const = 0;
	virtual bool sendClose() = 0;
	//virtual bool sendAlarm(MediateAlarmDto data) = 0;
};

#endif //!WS_COMMON_CLASSES_H_