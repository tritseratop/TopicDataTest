#ifndef ADAPTER_UNIT_H_
#define ADAPTER_UNIT_H_

#include "Lib/Common/DataCacher.h"
#include "Lib/WsService/ClientListener.h"
#include "Utilities/Ws/Callbacks.h"

#include <unordered_map>

namespace scada_ate::ws
{
class Group
{
private:
	const int64_t id_;
	std::unordered_map<int64_t, std::shared_ptr<ClientListener>> clients_;
	mutable std::mutex clients_mutex_;
	std::shared_ptr<Cacher> cacher_;

public:
	Group(int64_t id, std::shared_ptr<Cacher> cacher = nullptr);

	void addClient(const std::shared_ptr<ClientListener>& client);
	void removeClientById(int64_t id);

	void sendCacherDataToAllAsync();

	void sendTextMessageToAllAsync(const oatpp::String& message);
	void sendCloseToAllAsync();

	void runTestMessageSending(TestCallback& test_callback);
	void sendTextMessageToAllAsync(const oatpp::String& message,
								   const BeforeMessageSend& before_msg_send);

	bool hasClients() const;
	int64_t getId() const;
};
} // namespace scada_ate::ws

#endif //!ADAPTER_UNIT_H_