#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"

#include <memory>
#include <functional>
#include "BlockingQueue.h"
#include "Message.h"

namespace net
{
	class Server;

	/// <summary>
	/// Connection base class for handling remote connections
	/// </summary>
	class Connection
	{
	public:
		Connection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in) :
			context(context), socket(std::move(socket)), messagesIn(in) {}
		virtual ~Connection() {};

		bool Disconnect();
		bool IsConnected() const;

		/// <summary>
		/// Send a message to remote
		/// </summary>
		/// <param name="message">message to be sent</param>
		/// <returns></returns>
		bool Send(const Message& message);

		void SetResultHandler(const std::function<void(std::error_code, const std::string& message)>& handler) { asioResult = handler; }

	protected:
		/// <summary>
		/// ASYNC - Read a message header
		/// </summary>
		void ReadHeader();

		/// <summary>
		/// ASYNC - Read a message body
		/// </summary>
		void ReadBody();

		/// <summary>
		/// ASYNC - Write a header to the remote
		/// </summary>
		void WriteHeader();

		/// <summary>
		/// ASYNC - Write a body to the remote
		/// </summary>
		void WriteBody();

		virtual void WriteValidation() = 0;

		/// <summary>
		/// Add a message to the queue when the whole message has been processed
		/// </summary>
		virtual void AddToQueue() = 0;

		/// <summary>
		/// Generate a token based on an input. Used to verify the client's authenticity
		/// </summary>
		/// <param name="input">input to be scrambled</param>
		/// <returns>Token</returns>
		virtual uint64_t Hash(uint64_t input);

	protected:
		std::function<void(std::error_code, const std::string& message)> asioResult;
		Message temporary;
		asio::ip::tcp::socket socket;
		asio::io_context& context;
		BlockingQueue<Message> messagesOut;
		BlockingQueue<OwnedMessage>& messagesIn;

		uint64_t handshakeOut;
		uint64_t handshakeIn;
	};

	class ServerConnection : public Connection
	{
	public:
		ServerConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);

		bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

	protected:
		void WriteValidation() override;
		void ReadValidation();
		void AddToQueue() override;
	};

	class ClientConnection : public Connection, public std::enable_shared_from_this<ClientConnection>
	{
	public:
		ClientConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);
		void ConnectToClient(Server* remote, uint32_t id);

	protected:
		void WriteValidation() override;
		void ReadValidation(Server* remote);
		void AddToQueue() override;

	protected:
		uint64_t handshakeCheck;

		/// <summary>
		/// Client ID
		/// </summary>
		uint32_t id;
	};
}

#pragma region Connection implementation

inline uint64_t net::Connection::Hash(uint64_t input)
{
	return input ^ 0xABAC;
}

bool net::Connection::IsConnected() const
{
	return socket.is_open();
}

inline bool net::Connection::Disconnect()
{
	if (IsConnected())
		asio::post(context, [this]() {socket.close(); });
}

inline bool net::Connection::Send(const Message& message)
{
	asio::post(context, [this, message]()
		{
			messagesOut.PushBack(message);

			// Prevent providing another WriteHeader workload when one is still active
			bool idle = messagesOut.Empty();
			if (idle)
				WriteHeader();
		});
}

inline void net::Connection::ReadHeader()
{
	asio::async_read(socket, asio::buffer(&temporary.header, sizeof(MessageHeader)),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				if (temporary.header.messageSize > 0)
				{
					temporary.body.resize(temporary.header.messageSize);
					ReadBody();
				}
				else
				{
					AddToQueue();
				}
			}
			else
			{
				asioResult(error, "Could not start read header routine. Connection closed!");
				socket.close();
			}
		}
	);
}

inline void net::Connection::ReadBody()
{
	asio::async_read(socket, asio::buffer(temporary.body.data(), temporary.body.size()),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				AddToQueue();
			}
			else
			{
				asioResult(error, "Could not start read body routine. Connection closed!");
				socket.close();
			}
		}
	);
}

inline void net::Connection::WriteHeader()
{
	asio::async_write(socket, asio::buffer(&messagesOut.Front().header, sizeof(MessageHeader)),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				if (messagesOut.Front().body.size() > 0)
				{
					WriteBody();
				}
				else
				{
					messagesOut.PopFront();

					if (!messagesOut.Empty())
					{
						WriteHeader();
					}
				}
			}
			else
			{
				asioResult(error, "Could not start write header routine. Connection closed!");
				socket.close();
			}

		});
}

inline void net::Connection::WriteBody()
{
	asio::async_write(socket, asio::buffer(messagesOut.Front().body.data(), messagesOut.Front().body.size()),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				messagesOut.PopFront();

				if (!messagesOut.Empty())
				{
					WriteHeader();
				}
			}
			else
			{
				asioResult(error, "Could not start write body routine. Connection closed!");
				socket.close();
			}

		});
}

#pragma endregion

#pragma region SERVER Connection implementation

net::ServerConnection::ServerConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in)
	: Connection(context, std::move(socket), in)
{

}

inline void net::ServerConnection::AddToQueue()
{
	messagesIn.PushBack({ nullptr, temporary });
	ReadHeader();
}

inline bool net::ServerConnection::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
{
	asio::async_connect(socket, endpoints,
		[this](std::error_code error, asio::ip::tcp::endpoint endpoint)
		{
			if (!error)
			{
				ReadValidation();
			}
			else
			{
				asioResult(error, "Could not connect to server!");
			}
		});
}

inline void net::ServerConnection::WriteValidation()
{
	asio::async_write(socket, asio::buffer(&handshakeOut, sizeof(uint64_t)),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				ReadHeader();
			}
			else
			{
				asioResult(error, "Could not write validation.");
				socket.close();
			}
		});
}

inline void net::ServerConnection::ReadValidation()
{
	asio::async_read(socket, asio::buffer(&handshakeIn, sizeof(uint64_t)),
		[this](std::error_code error, std::size_t length)
		{
			if (!error)
			{
					handshakeOut = Hash(handshakeIn);
					WriteValidation();
			}
			else
			{
				asioResult(error, "Could not read validation");
				socket.close();
			}
		});
}

#pragma endregion

#pragma region Client connection implementation

net::ClientConnection::ClientConnection(asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in)
	: Connection(context, std::move(socket), in)
{
	handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
	handshakeCheck = Hash(handshakeOut);
}

inline void net::ClientConnection::ConnectToClient(Server* remote, uint32_t id = 0)
{
	if (socket.is_open())
	{
		this->id = id;
		WriteValidation();
		ReadValidation(remote);
	}
}

inline void net::ClientConnection::AddToQueue()
{
	messagesIn.PushBack({ this->shared_from_this(), temporary });
	ReadHeader();
}

inline void net::ClientConnection::WriteValidation()
{
	asio::async_write(socket, asio::buffer(&handshakeOut, sizeof(uint64_t)),
		[this](std::error_code error, std::size_t length)
		{
			if (error)
			{
				socket.close();
				asioResult(error, "Could not write validation");
			}
		});
}

inline void net::ClientConnection::ReadValidation(Server* remote)
{
	asio::async_read(socket, asio::buffer(&handshakeIn, sizeof(uint64_t)),
		[this, remote](std::error_code error, std::size_t length)
		{
			if (!error)
			{
				if (handshakeIn == handshakeCheck)
				{
					asioResult(error, "Client validated!");
					remote->OnClientValidated(this->shared_from_this());

					ReadHeader();
				}
				else
				{
					asioResult(error, "Client validation failed");
					socket.close();
				}
			}
			else
			{
				asioResult(error, "Could not read validation");
				socket.close();
			}
		});
}
#pragma endregion