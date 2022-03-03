#pragma once

class Configure {
public:
	Configure(int N = 3, int M = 5
		, const std::string& wshost = "localhost", int wsport = 8080)
		: MAX_CLIENT_COUNT(N)
		, MAX_MESSAGE_BUF_COUNT(M)
		, WS_HOST(wshost.c_str())
		, WS_PORT(wsport)
	{}

	int getMaxClientCount() const {
		return MAX_CLIENT_COUNT;
	}
	int getMaxMessageBufCount() const {
		return MAX_MESSAGE_BUF_COUNT;
	}
	int getPort() const {
		return WS_PORT;
	}
	std::string getIp() const {
		return WS_HOST;
	}

public:
	int MAX_CLIENT_COUNT;
	int MAX_MESSAGE_BUF_COUNT;;
	std::string WS_HOST;
	int WS_PORT;
};