#ifndef WS_COMMON_CLASSES_H_
#define WS_COMMON_CLASSES_H_

#include "oatpp/core/Types.hpp"

#include <functional>

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

#endif //!WS_COMMON_CLASSES_H_