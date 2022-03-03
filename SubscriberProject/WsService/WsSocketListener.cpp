#include "WsSocketListener.h"

WsSocketListener::WsSocketListener()
{
}

void WsSocketListener::addClient(const std::shared_ptr<ClientListener>& client)
{
	idToClient[client->getClientId()] = client;
}

void WsSocketListener::removeClientById(v_int64 id) {
	if (idToClient.count(id) != 0) {
		idToClient.erase(id);
	}
}

std::shared_ptr<ClientListener> WsSocketListener::getClientById(v_int64 id) {
	if (idToClient.count(id) != 0) {
		return idToClient[id];
	}
	return nullptr;
}

void WsSocketListener::sendMessageToAllAsync(const oatpp::String& message) {
	std::lock_guard<std::mutex> m(m_writeMessage);
	for (auto& pair : idToClient) {
		pair.second->sendMessageAsync(message);
	}
}

std::atomic<v_int32> WsSocketListener::SOCKETS(0); 

void WsSocketListener::onAfterCreate_NonBlocking(const std::shared_ptr<ClientListener::AsyncWebSocket>& socket, const std::shared_ptr<const ParameterMap>& params) {
	//TODO initialize
	int id;
	//TODO mutex
	auto client = std::make_shared<ClientListener>(socket, this, id);
	socket->setListener(client);
	client->sendMessageAsync("Connected");
	addClient(client);
}

void WsSocketListener::onBeforeDestroy_NonBlocking(const std::shared_ptr<ClientListener::AsyncWebSocket>& socket) {
	auto client = std::static_pointer_cast<ClientListener>(socket->getListener());
	removeClientById(client->getClientId());
	client->invalidateSocket();
}