#ifndef WS_COMMON_CLASSES_H_
#define WS_COMMON_CLASSES_H_

#include "WsDto.h"
#include <functional>

class ClientListener;
class Group;

using TestCallback = std::function<void(Group&)>;
using BeforeMessageSend = std::function<void(oatpp::String& message)>;

using OnMessageRead = std::function<void(const oatpp::String& message)>;

struct ConnectionConfigure
{
	std::string path = "ws";
};

struct WsConfigure
{
	int port = 8081;
	std::string host = "localhost";
	std::vector<ConnectionConfigure> connections;
};

class IServer
{
public:
	virtual bool sendData(WsDataDto::Wrapper data) = 0;
	virtual bool sendData(oatpp::String data) = 0;
	virtual void stop() = 0;
	virtual bool hasConnections() const = 0;
	virtual bool sendClose() = 0;
	//virtual bool sendAlarm(MediateAlarmDto data) = 0;
};

#endif //!WS_COMMON_CLASSES_H_