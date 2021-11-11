#include "Connection.h"
#include "Remote.h"

namespace net
{
#pragma region Connection implementation

	uint64_t Connection::Hash(uint64_t input)
	{
		return input ^ 0xABAC;
	}

	bool Connection::IsConnected() const
	{
		return socket.is_open();
	}

	void Connection::Disconnect()
	{
		if (IsConnected())
			asio::post(context, [this]() {socket.close(); });
	}

	void Connection::Send(const Message& message)
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

	void Connection::ReadHeader()
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
					log(LogMessageType::Error, "Could not start read header routine. Connection closed!");
					socket.close();
				}
			}
		);
	}

	void Connection::ReadBody()
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
					log(LogMessageType::Error, "Could not start read body routine. Connection closed!");
					socket.close();
				}
			}
		);
	}

	void Connection::WriteHeader()
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
					log(LogMessageType::Error, "Could not start write header routine. Connection closed!");
					socket.close();
				}

			});
	}

	void Connection::WriteBody()
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
					log(LogMessageType::Error, "Could not start write body routine. Connection closed!");
					socket.close();
				}

			});
	}

#pragma endregion

#pragma region SERVER Connection implementation

	ServerConnection::ServerConnection(LogFunction& f, asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in)
		: Connection(f, context, std::move(socket), in)
	{

	}

	void ServerConnection::AddToQueue()
	{
		messagesIn.PushBack({ nullptr, temporary });
		ReadHeader();
	}

	void ServerConnection::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
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
					log(LogMessageType::Error, "Could not connect to server!");
				}
			});
	}

	void ServerConnection::WriteValidation()
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
					log(LogMessageType::Error, "Could not write validation.");
					socket.close();
				}
			});
	}

	void ServerConnection::ReadValidation()
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
					log(LogMessageType::Error, "Could not read validation");
					socket.close();
				}
			});
	}

#pragma endregion

#pragma region Client connection implementation

	ClientConnection::ClientConnection(LogFunction& l, asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in)
		: Connection(l, context, std::move(socket), in)
	{
		handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
		handshakeCheck = Hash(handshakeOut);
	}

	void ClientConnection::ConnectToClient(Server* remote, uint32_t id = 0)
	{
		if (socket.is_open())
		{
			this->id = id;
			WriteValidation();
			ReadValidation(remote);
		}
	}

	void ClientConnection::AddToQueue()
	{
		messagesIn.PushBack({ this->shared_from_this(), temporary });
		ReadHeader();
	}

	void ClientConnection::WriteValidation()
	{
		asio::async_write(socket, asio::buffer(&handshakeOut, sizeof(uint64_t)),
			[this](std::error_code error, std::size_t length)
			{
				if (error)
				{
					socket.close();
					log(LogMessageType::Error, "Could not write validation");
				}
			});
	}

	void ClientConnection::ReadValidation(Server* remote)
	{
		asio::async_read(socket, asio::buffer(&handshakeIn, sizeof(uint64_t)),
			[this, remote](std::error_code error, std::size_t length)
			{
				if (!error)
				{
					if (handshakeIn == handshakeCheck)
					{
						log(LogMessageType::Info, "Client validated!");
						remote->OnClientValidated(this->shared_from_this());

						ReadHeader();
					}
					else
					{
						log(LogMessageType::Error, "Client validation failed");
						socket.close();
					}
				}
				else
				{
					log(LogMessageType::Error, "Could not read validation");
					socket.close();
				}
			});
	}
#pragma endregion
}