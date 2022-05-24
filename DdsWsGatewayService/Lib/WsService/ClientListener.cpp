#include "Lib/WsService/ClientListener.h"
#include "Lib/WsService/WsSocketListener.h"
#include "Utilities/TimeConverter/TimeConverter.hpp"
#include "Utilities/WsTestUtility.h"

v_int64 ClientListener::getClientId()
{
	return client_id_;
}

oatpp::async::CoroutineStarter ClientListener::onPing(const std::shared_ptr<AsyncWebSocket>& socket,
													  const oatpp::String& message)
{
	OATPP_LOGI(TAG, "onPing");
	return socket->sendPongAsync(message);
}
oatpp::async::CoroutineStarter ClientListener::onPong(const std::shared_ptr<AsyncWebSocket>& socket,
													  const oatpp::String& message)
{
	OATPP_LOGI(TAG, "onPong");
	return nullptr;
}
oatpp::async::CoroutineStarter ClientListener::onClose(
	const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const oatpp::String& message)
{
	OATPP_LOGD(TAG, "onClose code = %d", code);
	return nullptr;
}

oatpp::async::CoroutineStarter
ClientListener::readMessage(const std::shared_ptr<AsyncWebSocket>& socket,
							v_uint8 opcode,
							p_char8 data,
							oatpp::v_io_size size)
{
	if (size == 0)
	{
		auto wholeMessage = message_buffer_.toString();
		message_buffer_.setCurrentPosition(0);
		// TODO что делать с принятыми сообщениями?
		return nullptr;
	}
	else if (size > 0)
	{
		message_buffer_.writeSimple(data, size);
	}
	return nullptr;
}

void ClientListener::runTestMessageSendingAsync(WebsockServer* server,
												int64_t sleep,
												int64_t initial_disp,
												size_t times)
{
	class TestPacketSenderCoroutine : public oatpp::async::Coroutine<TestPacketSenderCoroutine>
	{
	private:
		oatpp::async::Lock* lock_;
		std::shared_ptr<AsyncWebSocket> websocket_;
		int64_t sleep_;
		int64_t initial_disp_;
		WebsockServer* server_;
		size_t times_ = 0;
		size_t counter = 0;

	public:
		TestPacketSenderCoroutine(oatpp::async::Lock* lock,
								  const std::shared_ptr<AsyncWebSocket> socket,
								  int64_t sleep,
								  int64_t initial_disp,
								  size_t times,
								  WebsockServer* server = nullptr)
			: lock_(lock)
			, websocket_(socket)
			, sleep_(sleep)
			, server_(server)
			, times_(times)
			, initial_disp_(initial_disp)
		{ }

		Action act() override
		{
			if (counter == times_)
			{
				return yieldTo(&TestPacketSenderCoroutine::close);
			}
			TestPacket packet{initial_disp_ + counter, ""};
			counter++;
			auto packet_json = nlohmann::json(packet).dump();
			auto oatpp_packet_json = oatpp::String(packet_json);
			if (server_ != nullptr)
			{
				server_->cache(getTimeFromJsonString(packet_json));
			}
			return oatpp::async::synchronize(lock_,
											 websocket_->sendOneFrameTextAsync(oatpp_packet_json))
				.next(repeat());
		}

		Action close()
		{
			return oatpp::async::synchronize(lock_, websocket_->sendCloseAsync()).next(finish());
		}
	};

	if (socket_)
	{
		async_executor_->execute<TestPacketSenderCoroutine>(
			&write_lock_, socket_, sleep, initial_disp, times, server);
	}
}

void ClientListener::runDataSendingAsync(WebsockServer* server, int64_t sleep)
{
	class DataSenderCoroutine : public oatpp::async::Coroutine<DataSenderCoroutine>
	{
	private:
		oatpp::async::Lock* lock_;
		std::shared_ptr<AsyncWebSocket> socket_;
		int64_t sleep_;
		WebsockServer* server_;
		DataCacher& cacher_;
		MediateDtoMapper mapper_;

	public:
		DataSenderCoroutine(oatpp::async::Lock* lock,
							const std::shared_ptr<AsyncWebSocket> socket,
							int64_t sleep,
							WebsockServer* server = nullptr)
			: lock_(lock)
			, socket_(socket)
			, sleep_(sleep * 1000)
			, server_(server)
			, cacher_(server_->getDataCacher())
		{ }

		Action act() override
		{
			auto dto = cacher_.popDdsDto();
			if (dto.has_value())
			{
				oatpp::String data(mapper_.toString(dto.value()));
				return oatpp::async::synchronize(lock_, socket_->sendOneFrameTextAsync(data))
					.next(waitRepeat(std::chrono::microseconds(sleep_)));
			}
			else
			{
				return waitRepeat(std::chrono::microseconds(sleep_));
			}
		}

		Action prepareNewTextFrame() { }

		Action sendNewTextFrame() { }

		Action close()
		{
			return oatpp::async::synchronize(lock_, socket_->sendCloseAsync()).next(finish());
		}
	};

	if (socket_)
	{
		async_executor_->execute<DataSenderCoroutine>(&write_lock_, socket_, sleep, server);
	}
}

void ClientListener::sendCloseAsync()
{
	class SendCloseCoroutine : public oatpp::async::Coroutine<SendCloseCoroutine>
	{
	private:
		oatpp::async::Lock* m_lock;
		std::shared_ptr<AsyncWebSocket> websocket;

	public:
		SendCloseCoroutine(oatpp::async::Lock* lock, const std::shared_ptr<AsyncWebSocket> socket)
			: m_lock(lock)
			, websocket(socket)
		{ }

		Action act() override
		{
			return oatpp::async::synchronize(m_lock, websocket->sendCloseAsync()).next(finish());
		}
	};

	if (socket_)
	{
		async_executor_->execute<SendCloseCoroutine>(&write_lock_, socket_);
	}
}

void ClientListener::invalidateSocket()
{
	if (socket_)
	{
		//server_connection_provider_->invalidate(socket_->getConnection());
	}
	socket_.reset();
}