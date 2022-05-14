#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "../../Utilities/WsCommonClasses.h"
#include "Lib/DataObserver/DataCacher.h"

#include <deque>
#include <unordered_map>

class Gateway
{
public:
	Gateway(std::vector<IServer*> servers);

	bool sendDdsData();

	bool sendDdsAlarm();

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;
	MediateDtoMapper ws_mapper_;

	DataCacher cacher_;
	std::vector<IServer*> servers_;
};

#endif //!GATEWAY_H_