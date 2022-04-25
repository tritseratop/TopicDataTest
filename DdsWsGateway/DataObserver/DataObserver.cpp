#include <utility>

#include "DataObserver.h"

DataObserver::DataObserver(std::vector<IServer*> servers, DataCacher* cacher)
	: servers_(servers)
	, cacher_(cacher)
	, stop_sending_data_(false)
	, stop_sending_alarm_(false)
{
}

bool DataObserver::sendDdsData()
{
	for (auto server : servers_)
	{
		if (server != nullptr)
		{
			auto data = cacher_->popDdsDto();
			if (data.has_value())
			{
				auto ws_dto = ws_mapper_.mapDataDto(data.value());
				server->sendData(ws_dto);
			}
			
		}
	}
	return true;
}

bool DataObserver::sendDdsAlarm()
{
	return false;
}