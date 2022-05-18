#include "WsSocketListener.h"

WsSocketListener::WsSocketListener() { }

void WsSocketListener::addClient(const std::shared_ptr<ClientListener>& client)
{
	std::lock_guard<std::mutex> guard(m_change_clients_);
	clients_[client->getClientId()] = client;
}

void WsSocketListener::removeClientById(v_int64 id)
{
	if (clients_.count(id) != 0)
	{
		std::lock_guard<std::mutex> guard(m_change_clients_);
		clients_.erase(id);
	}
}

std::shared_ptr<ClientListener> WsSocketListener::getClientById(v_int64 id)
{
	if (clients_.count(id) != 0)
	{
		return clients_[id];
	}
	return nullptr;
}

void WsSocketListener::sendMessageToAllAsync(const oatpp::String& message, WebsockServer* server)
{
	std::lock_guard<std::mutex> m(m_write_message_);
	for (auto& pair : clients_)
	{
		pair.second->sendMessageAsync(message, server);
	}
}

void WsSocketListener::sendClose()
{
	std::lock_guard<std::mutex> m(m_write_message_);
	for (auto& pair : clients_)
	{
		pair.second->sendCloseAsync();
	}
}

std::atomic<v_int32> WsSocketListener::SOCKETS(0);

void WsSocketListener::onAfterCreate_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
	const std::shared_ptr<const ParameterMap>& params)
{
	//TODO initialize
	auto client = std::make_shared<ClientListener>(socket, this, SOCKETS);
	++SOCKETS;
	socket->setListener(client);
	addClient(client);
}

void WsSocketListener::onBeforeDestroy_NonBlocking(
	const std::shared_ptr<ClientListener::AsyncWebSocket>& socket)
{
	auto client = std::static_pointer_cast<ClientListener>(socket->getListener());
	removeClientById(client->getClientId());
	client->invalidateSocket();
}

bool WsSocketListener::isConnected()
{
	return clients_.size() > 0;
}