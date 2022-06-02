#include "Lib/WsService/WsSocketListener.h"

WsSocketListener::WsSocketListener(
	std::unordered_map<int64_t, std::shared_ptr<AdapterUnit>> adapter_units)
	: adapter_units_counter_(0)
	, adapter_units_(std::move(adapter_units))
{ }

void WsSocketListener::sendCloseToAllAsync()
{
	std::lock_guard<std::mutex> m(adapter_units_mutex_);
	for (auto& [id, adapter_unit] : adapter_units_)
	{
		adapter_unit->sendCloseToAllAsync();
	}
}

void WsSocketListener::onAfterCreate_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
	const std::shared_ptr<const ParameterMap>& params)
{
	auto adapter_unit_id = params->find("adapter-unit-id")->second;
	auto adapter_unit = getAdapterUnitPtr(adapter_unit_id);

	auto client = std::make_shared<ClientListener>(socket, getClientId(), adapter_unit);
	socket->setListener(client);

	if (adapter_unit == nullptr)
	{
		client->sendCloseAsync();
	}
	else
	{
		adapter_unit->addClient(client);
	}
}

int64_t WsSocketListener::getClientId()
{
	return adapter_units_counter_++;
}

std::shared_ptr<AdapterUnit> WsSocketListener::getAdapterUnitPtr(oatpp::String adapter_unit_id)
{
	int64_t adapter_unit_id_number = atoll(adapter_unit_id->c_str());

	std::lock_guard<std::mutex> guard(adapter_units_mutex_);
	if (adapter_units_.count(adapter_unit_id_number) != 0)
	{
		return adapter_units_.at(adapter_unit_id_number);
	}
	return nullptr;
}

void WsSocketListener::onBeforeDestroy_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket)
{
	auto client = std::static_pointer_cast<ClientListener>(socket->getListener());
	auto client_id = client->getClientId();
	auto adapter_unit = client->getAdapterUnit();

	adapter_unit->removeClientById(client_id);

	client->invalidateSocket();
}
