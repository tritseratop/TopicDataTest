#include "Lib/WsService/AdapterUnit.h"

AdapterUnit::AdapterUnit(int64_t id)
	: id_(id)
{ }

void AdapterUnit::addClient(const std::shared_ptr<ClientListener>& client)
{
	std::lock_guard<std::mutex> guard(clients_mutex_);
	clients_[client->getClientId()] = client;
}

void AdapterUnit::removeClientById(int64_t id)
{
	std::lock_guard<std::mutex> guard(clients_mutex_);
	clients_.erase(id);
}

void AdapterUnit::sendTextMessageToAllAsync(const oatpp::String& message)
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendMessageAsync(message);
	}
}

void AdapterUnit::sendCloseToAllAsync()
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendCloseAsync();
	}
}

void AdapterUnit::runTestMessageSending(TestCallback& test_callback)
{
	test_callback(*this);
}

void AdapterUnit::sendTextMessageToAllAsync(const oatpp::String& message,
											const BeforeMessageSend& before_msg_send)
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	for (auto& [id, client] : clients_)
	{
		client->sendMessageAsync(message, before_msg_send);
	}
}

bool AdapterUnit::hasClients() const
{
	std::lock_guard<std::mutex> m(clients_mutex_);
	return clients_.size() > 0;
}

int64_t AdapterUnit::getId() const
{
	return id_;
}