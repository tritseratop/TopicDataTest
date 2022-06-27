#include "Lib/WsService/Group.h"

Group::Group(int64_t id, std::shared_ptr<Cacher> cacher)
	: id_(id)
	, cacher_(cacher)
{ }

void Group::addClient(const std::shared_ptr<ClientListener>& client)
{
	std::lock_guard<std::mutex> guard(clients_mutex_);
	clients_[client->getClientId()] = client;
}

void Group::removeClientById(int64_t id)
{
	std::lock_guard<std::mutex> guard(clients_mutex_);
	clients_.erase(id);
}

void Group::sendCacherDataToAllAsync()
{
	auto data = cacher_->popAsString();
	if (data.has_value())
	{
		auto data_to_send = oatpp::String(std::move(data.value()));
		std::lock_guard<std::mutex> m(clients_mutex_);
		for (auto& [id, client] : clients_)
		{
			client->sendMessageAsync(data_to_send);
		}
	}
	else
	{
		//TODO
	}
}

void Group::sendTextMessageToAllAsync(const oatpp::String& message)
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendMessageAsync(message);
	}
}

void Group::sendCloseToAllAsync()
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendCloseAsync();
	}
}

void Group::runTestMessageSending(TestCallback& test_callback)
{
	test_callback(*this);
}

void Group::sendTextMessageToAllAsync(const oatpp::String& message,
									  const BeforeMessageSend& before_msg_send)
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendMessageAsync(message, before_msg_send);
	}
}

bool Group::hasClients() const
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	return clients_.size() > 0;
}

int64_t Group::getId() const
{
	return id_;
}