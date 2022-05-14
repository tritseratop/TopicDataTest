#ifndef DATA_OBSERVER_H_
#define DATA_OBSERVER_H_

#include "../../Utilities/WsCommonClasses.h"
#include "DataCacher.h"

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
	MediateDtoMapper mapper_;

	DataCacher* cacher_;
	std::vector<IServer*> servers_;
};

#endif //!DATA_OBSERVER_H_