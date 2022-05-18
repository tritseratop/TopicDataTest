#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "Lib/DdsService/DdsSubscriber.h"
#include "Lib/Notifier/DataCacher.h"
#include "Lib/WsService/WsServer.h"
#include "Utilities/WsCommonClasses.h"

#include <deque>
#include <unordered_map>

class Gateway
{
public:
	Gateway(WebsockServer& ws_server, SubscriberService& dds_service);

	bool sendDdsData();

	bool sendDdsAlarm();

private:
	bool stop_sending_data_;
	bool stop_sending_alarm_;
	MediateDtoMapper ws_mapper_;

	DataCacher cacher_;
	WebsockServer& ws_server_;
	SubscriberService& dds_service_;
};

#endif //!GATEWAY_H_