#ifndef WSListener_hpp
#define WSListener_hpp

#include "../DdsWsGatewayService/Utilities/PackageAnalyser.h"
#include "../DdsWsGatewayService/Utilities/WsCommonClasses.h"
#include "../DdsWsGatewayService/Utilities/WsDto.h"

#include "oatpp-websocket/AsyncWebSocket.hpp"
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/Connector.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class WSClient;

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class WSListener : public oatpp::websocket::AsyncWebSocket::Listener
{
private:
	static constexpr const char* TAG = "Client_WSListener";

private:
	//std::mutex& m_writeMutex;
	/**
     * Buffer for messages. Needed for multi-frame messages.
     */
	oatpp::data::stream::BufferOutputStream m_messageBuffer;
	std::string login;
	WSClient* client;
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper;

	PackageAnalyser* analyser;
	bool first_package;
	WsDataDto::Wrapper dto;
	size_t prev_size;

public:
	/*WSListener(std::mutex& writeMutex)
        : m_writeMutex(writeMutex)
    {}*/

	/*WSListener(std::mutex& writeMutex) : m_writeMutex(writeMutex){}

    WSListener(WSClient* client_, std::mutex& writeMutex)
        : client(client_) 
        , m_writeMutex(writeMutex)
    {}*/
	WSListener()
		: client(nullptr)
		, first_package(false)
		, prev_size(0)
	{
		json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	}

	WSListener(WSClient* client_)
		: client(client_)
		, first_package(false)
		, prev_size(0)
	{
		json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	}

	void setLogin(std::string login_);
	std::string getLogin();

	/**
     * Called on "ping" frame.
     */
	CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket,
							const oatpp::String& message) override;

	/**
     * Called on "pong" frame
     */
	CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket,
							const oatpp::String& message) override;

	/**
     * Called on "close" frame
     */
	CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket,
							 v_uint16 code,
							 const oatpp::String& message) override;

	/**
     * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
     */
	CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket,
								 v_uint8 opcode,
								 p_char8 data,
								 oatpp::v_io_size size) override;
};

class ClientCoroutine : public oatpp::async::Coroutine<ClientCoroutine>
{
public:
	static std::shared_ptr<oatpp::websocket::AsyncWebSocket> SOCKET;

private:
	std::shared_ptr<oatpp::websocket::AsyncWebSocket> m_socket;
	std::shared_ptr<oatpp::websocket::Connector> m_connector;
	WSClient* client;

public:
	ClientCoroutine(const std::shared_ptr<oatpp::websocket::Connector>& connector)
		: m_connector(connector)
		, m_socket(nullptr)
	{ }

	ClientCoroutine(const std::shared_ptr<oatpp::websocket::Connector>& connector,
					WSClient* client_)
		: m_connector(connector)
		, m_socket(nullptr)
		, client(client_)
	{ }
	Action act() override;
	Action
	onConnected(const oatpp::provider::ResourceHandle<oatpp::data::stream::IOStream>& connection);
	Action onFinishListen();
};

#endif // WSListener_hpp