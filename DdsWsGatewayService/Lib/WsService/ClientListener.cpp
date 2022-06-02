#include "Lib/WsService/ClientListener.h"
#include "Lib/WsService/AdapterUnit.h"
#include "Utilities/TimeConverter/TimeConverter.hpp"
#include "Utilities/WsTestUtility.h"

ClientListener::ClientListener(const std::shared_ptr<AsyncWebSocket>& socket,
							   int64_t id,
							   std::shared_ptr<AdapterUnit> adapter_unit)
	: socket_(socket)
	, id_(id)
	, adapter_unit_(adapter_unit)
{ }

int64_t ClientListener::getClientId() const
{
	return id_;
}

std::shared_ptr<AdapterUnit> ClientListener::getAdapterUnit()
{
	return adapter_unit_;
}

oatpp::async::CoroutineStarter ClientListener::onPing(const std::shared_ptr<AsyncWebSocket>& socket,
													  const oatpp::String& message)
{
	OATPP_LOGI(TAG, "onPing");
	return oatpp::async::synchronize(&write_lock_, socket->sendPongAsync(message));
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

void ClientListener::sendMessageAsync(const oatpp::String& message)
{
	class SendMessageCoroutine : public oatpp::async::Coroutine<SendMessageCoroutine>
	{
	private:
		oatpp::async::Lock* m_lock;
		std::shared_ptr<AsyncWebSocket> websocket;
		oatpp::String message_;

	public:
		SendMessageCoroutine(oatpp::async::Lock* lock,
							 const std::shared_ptr<AsyncWebSocket> socket,
							 const oatpp::String& message)
			: m_lock(lock)
			, websocket(socket)
			, message_(message)
		{ }

		Action act() override
		{
			return oatpp::async::synchronize(m_lock, websocket->sendOneFrameTextAsync(message_))
				.next(finish());
		}
	};

	if (socket_)
	{
		async_executor_->execute<SendMessageCoroutine>(&write_lock_, socket_, message);
	}
}

void ClientListener::sendCloseAsync()
{
	class SendCloseCoroutine : public oatpp::async::Coroutine<SendCloseCoroutine>
	{
	private:
		oatpp::async::Lock* lock_;
		std::shared_ptr<AsyncWebSocket> websocket_;

	public:
		SendCloseCoroutine(oatpp::async::Lock* lock, const std::shared_ptr<AsyncWebSocket> socket)
			: lock_(lock)
			, websocket_(socket)
		{ }

		Action act() override
		{
			return oatpp::async::synchronize(lock_, websocket_->sendCloseAsync()).next(finish());
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
		socket_.reset();
	}
}

void ClientListener::sendMessageAsync(const oatpp::String& message,
									  const BeforeMessageSend& before_message_send)
{
	class SendMessageCoroutine : public oatpp::async::Coroutine<SendMessageCoroutine>
	{
	private:
		oatpp::async::Lock* m_lock;
		std::shared_ptr<AsyncWebSocket> websocket;
		oatpp::String message_;
		const BeforeMessageSend before_message_send_;

	public:
		SendMessageCoroutine(oatpp::async::Lock* lock,
							 const std::shared_ptr<AsyncWebSocket> socket,
							 const oatpp::String& message,
							 const BeforeMessageSend& before_message_send)
			: m_lock(lock)
			, websocket(socket)
			, message_(message)
			, before_message_send_(before_message_send)
		{ }

		Action act() override
		{
			before_message_send_(message_);
			return oatpp::async::synchronize(m_lock, websocket->sendOneFrameTextAsync(message_))
				.next(finish());
		}
	};

	if (socket_)
	{
		async_executor_->execute<SendMessageCoroutine>(
			&write_lock_, socket_, message, before_message_send);
	}
}