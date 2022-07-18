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

	std::atomic<bool> stop_ = false;
	int64_t period_ms_ = 100;

public:
	Group(int64_t id, std::shared_ptr<Cacher> cacher = nullptr);

	void addClient(const std::shared_ptr<ClientListener>& client);
	void removeClientById(int64_t id);

	void runSendingUpdatedValues();
	bool sendUpdatedValuesToAllAsync();

	void runSendingCachedValues();
	bool sendCachedValuesToAllAsync();

	void stopSending();

	void sendMessageToAllAsync(const oatpp::String& message);
	void sendCloseToAllAsync();

	// Ìועמהû הכÿ עוסעמג
	void runTestCallback(TestCallback& test_callback);
	void sendMessageToAllAsync(const oatpp::String& message,
							   const BeforeMessageSend& before_msg_send);

	bool hasClients() const;
	int64_t getId() const;
};
} // namespace scada_ate::ws

#endif //!ADAPTER_UNIT_H_