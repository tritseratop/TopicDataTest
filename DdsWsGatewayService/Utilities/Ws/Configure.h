#ifndef WS_CONFIGURE_H_
#define WS_CONFIGURE_H_

namespace scada_ate::ws
{
struct ConnectionConfigure
{
	std::string path = "ws";
};

struct Configure
{
	int port = 8081;
	std::string host = "localhost";
	std::vector<ConnectionConfigure> connections;
};
} // namespace scada_ate::ws

#endif //!WS_CONFIGURE_H_