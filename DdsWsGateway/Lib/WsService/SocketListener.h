#ifndef WS_SOCKET_LISTENER_H_
#define WS_SOCKET_LISTENER_H_

#include "Lib/WsService/ClientListener.h"
#include "Lib/WsService/Group.h"

#include <map>
#include <unordered_map>

namespace scada_ate::ws
{
class SocketListener : public oatpp::websocket::AsyncConnectionHandler::SocketInstanceListener
{
private:
	static constexpr const char* TAG = "Server_WSInstanceListener";

	std::atomic<int64_t> groups_counter_;
	std::unordered_map<int64_t, std::shared_ptr<Group>> groups_;
	std::mutex groups_mutex_;

public:
	SocketListener(std::unordered_map<int64_t, std::shared_ptr<Group>> groups);

	void sendCloseToAllAsync();

	void onAfterCreate_NonBlocking(const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
								   const std::shared_ptr<const ParameterMap>& params) override;

	int64_t getClientId();
	std::shared_ptr<Group> getGroupPtr(oatpp::String group_id);

	void onBeforeDestroy_NonBlocking(
		const std::shared_ptr<ClientListener::AsyncWebSocket>& socket) override;
};
} // namespace scada_ate::ws

#endif //!WS_SOCKET_LISTENER_H_
