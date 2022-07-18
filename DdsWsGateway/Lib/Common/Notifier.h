#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include "Lib/Common/DataCacher.h"
#include "Lib/WsService/Server.h"

#include <deque>
#include <unordered_map>

namespace scada_ate
{
class Notifier
{
public:
	Notifier(ws::Server& ws_server, DataCacher& cacher);

	bool sendDdsData();
	bool sendDdsAlarm();

	void runNotifying(int64_t sleep = 100);
	void stopNotifying();

	bool is_data_cache_empty();

private:
	ws::Server& server_;
	DataCacher& cacher_;

	bool stop_sending_data_;
	bool stop_sending_alarm_;

	MediateDtoMapper mapper_;
};
} // namespace scada_ate

#endif //!DATA_OBSERVER_H_