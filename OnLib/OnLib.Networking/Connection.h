#pragma once

#include <memory>
#include "asio.hpp"
#include "BlockingQueue.h"
#include "Message.h"

namespace net
{
	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in) :
			context(context), socket(std::move(socket)), messagesIn(in) {}
		virtual ~Connection() {};

		virtual bool Disconnect();
		virtual bool IsConnected() const;
		virtual bool Send(const Message& message);

	protected:
		asio::ip::tcp::socket socket;
		asio::io_context& context;
		BlockingQueue<Message> messagesOut;
		BlockingQueue<OwnedMessage>& messagesIn;
	};

	class ServerConnection : public Connection, public std::enable_shared_from_this<ServerConnection>
	{
	public:
		ServerConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);

		bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);
	};

	class ClientConnection : public Connection, public std::enable_shared_from_this<ClientConnection>
	{
	public:
		ClientConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);
		void ConnectToClient(uint32_t id);

	protected:
		/*uint64_t handshakeIn;
		uint64_t handshakeOut;
		uint64_t handshakeCheck;*/
		uint32_t id;
	};
}

bool net::Connection::IsConnected() const
{
	return socket.is_open();
}

net::ServerConnection::ServerConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in) 
	: Connection(context, socket, in)
{

}

inline bool net::ServerConnection::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
{
	asio::async_connect(socket, endpoints,
		[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
		{
			if (!ec)
			{
				ReadValidation();
			}
		});
}

net::ClientConnection::ClientConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in)
	: Connection(context, socket, in)
{
}

inline void net::ClientConnection::ConnectToClient(uint32_t id = 0)
{
	if (socket.is_open())
		this->id = id;
}