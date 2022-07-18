#include "Lib/WsService/SocketListener.h"

namespace scada_ate::ws
{
SocketListener::SocketListener(std::unordered_map<int64_t, std::shared_ptr<Group>> groups)
	: groups_counter_(0)
	, groups_(std::move(groups))
{ }

void SocketListener::sendCloseToAllAsync()
{
	std::lock_guard<std::mutex> m(groups_mutex_);
	for (auto& [id, group] : groups_)
	{
		group->sendCloseToAllAsync();
	}
}

void SocketListener::onAfterCreate_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
	const std::shared_ptr<const ParameterMap>& params)
{
	auto group_id = params->find("adapter-unit-id")->second;
	auto group = getGroupPtr(group_id);

	auto client = std::make_shared<ClientListener>(socket, getClientId(), group);
	socket->setListener(client);

	if (group == nullptr)
	{
		client->sendCloseAsync();
	}
	else
	{
		group->addClient(client);
	}
}

int64_t SocketListener::getClientId()
{
	return groups_counter_++;
}

std::shared_ptr<Group> SocketListener::getGroupPtr(oatpp::String group_id)
{
	int64_t group_id_number = atoll(group_id->c_str());

	std::lock_guard<std::mutex> guard(groups_mutex_);
	if (groups_.count(group_id_number) != 0)
	{
		return groups_.at(group_id_number);
	}
	return nullptr;
}

void SocketListener::onBeforeDestroy_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket)
{
	auto client = std::static_pointer_cast<ClientListener>(socket->getListener());
	auto client_id = client->getClientId();
	auto group = client->getGroup();

	group->removeClientById(client_id);

	client->invalidateSocket();
}
} // namespace scada_ate::ws