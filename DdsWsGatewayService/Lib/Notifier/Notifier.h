#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include "Lib/Notifier/DataCacher.h"
#include "Lib/WsService/WsServer.h"
#include "Utilities/WsCommonClasses.h"

#include <deque>
#include <unordered_map>

class Notifier
{
public:
	Notifier(WebsockServer& ws_server, DataCacher& cacher);

	bool sendDdsData();
	bool sendDdsAlarm();

	void runNotifying(int64_t sleep = 100);
	void stopNotifying();

	bool is_data_cache_empty();

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;

	MediateDtoMapper mapper_;

	DataCacher& cacher_;
	WebsockServer& server_;
};

#endif //!DATA_OBSERVER_H_