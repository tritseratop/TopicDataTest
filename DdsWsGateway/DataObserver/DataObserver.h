#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include "DataCacher.h"
#include "../../DdsWsGatewayUtilities/WsCommonClasses.h"

#include <deque>
#include <unordered_map>

class DataObserver
{
public:
	DataObserver(std::vector<IServer*> servers, DataCacher* cacher);

	bool sendDdsData();

	bool sendDdsAlarm();

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;
	MediateDtoToWsDtoMapper ws_mapper_;

	DataCacher* cacher_;
	std::vector<IServer*> servers_;
};

#endif//!DATA_OBSERVER_H_