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
	Notifier(WsServer& ws_server, DataCacher& cacher);

	bool sendDdsData();
	bool sendDdsAlarm();

	void runNotifying(int64_t sleep = 100);
	void stopNotifying();

	bool is_data_cache_empty();

private:
	WsServer& server_;
	DataCacher& cacher_;

	bool stop_sending_data_;
	bool stop_sending_alarm_;

	MediateDtoMapper mapper_;
};

#endif //!DATA_OBSERVER_H_