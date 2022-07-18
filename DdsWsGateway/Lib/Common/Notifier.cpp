#include <utility>

#include "Notifier.h"

namespace scada_ate
{
Notifier::Notifier(ws::Server& ws_server, DataCacher& cacher)
	: server_(ws_server)
	, cacher_(cacher)
	, stop_sending_data_(false)
	, stop_sending_alarm_(false)
{ }

bool Notifier::sendDdsData()
{
	return false;
}

void Notifier::runNotifying(int64_t sleep)
{
	while (!stop_sending_data_)
	{
		sendDdsData();
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

void Notifier::stopNotifying()
{
	while (true)
	{
		if (cacher_.getDataCacheCopy().empty())
		{
			break;
		}
	}
	stop_sending_data_ = true;
}

bool Notifier::sendDdsAlarm()
{
	return false;
}
} // namespace scada_ate