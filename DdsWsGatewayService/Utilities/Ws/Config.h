#ifndef WS_CONFIG_H_
#define WS_CONFIG_H_

namespace scada_ate::ws
{
struct ConnectionConfig
{
	std::string path = "ws";
};

struct Config
{
	int port = 8081;
	std::string host = "localhost";
	std::vector<ConnectionConfig> connections;
};
} // namespace scada_ate::ws

#endif //!WS_CONFIG_H_