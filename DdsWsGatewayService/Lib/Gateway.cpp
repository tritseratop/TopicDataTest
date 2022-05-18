#include <utility>

#include "Gateway.h"

Gateway::Gateway(WebsockServer& ws_server, SubscriberService& dds_service)
	: ws_server_(ws_server)
	, dds_service_(dds_service)
	, cacher_(5)
	, stop_sending_data_(false)
	, stop_sending_alarm_(false)
{ }

bool Gateway::sendDdsData()
{
	auto data = cacher_.popDdsDto();
	if (data.has_value())
	{
		auto ws_dto = ws_mapper_.toWsDataDto(data.value());
		ws_server_.sendData(ws_dto);
		return true;
	}
	return false;
}

bool Gateway::sendDdsAlarm()
{
	return false;
}