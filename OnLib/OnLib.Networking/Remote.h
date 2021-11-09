#pragma once

#include "BlockingQueue.h"
#include "Message.h"
#include "Connection.h"

namespace net
{
	/// <summary>
	/// Remote client class
	/// </summary>
	class Client
	{
		using Server = std::unique_ptr<ServerConnection>;

	public:
		Client() : socket(context) {}
		virtual ~Client() { Disconnect(); }

		bool Connect(const std::string& host, const uint16_t port);
		void Disconnect();
		bool IsConnected() const;
		void Send(const Message& msg) const;
		BlockingQueue<OwnedMessage>& Incoming() { return messagesIn; }
	protected:
		asio::io_context context;
		std::thread runningThread;
		asio::ip::tcp::socket socket;
		Server connection;
		BlockingQueue<OwnedMessage> messagesIn;
	};

	/// <summary>
	/// Remote server class
	/// </summary>
	class Server
	{
		using Client = std::shared_ptr<ClientConnection>;

	public:
		Server(uint16_t port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}
		virtual ~Server() { Stop(); }

		bool Start();
		void Stop();

		void WaitForConnection();
		void MessageClient(Client client, const Message& message);
		void MessageAllClients(const Message& msg, Client ignore = nullptr);

		/// <summary>
		/// Process available messages
		/// </summary>
		/// <param name="maxMsgCount">Maximum number of messages to be processed</param>
		/// <param name="sleep">Sleep when there are no messages</param>
		void ProcessMessages(std::size_t maxMsgCount, bool sleep);

		/// <summary>
		/// Fired when a client has been validated
		/// </summary>
		/// <param name="client"> the client that was validated</param>
		virtual void OnClientValidated(Client client) = 0;
	protected:

		/// <summary>
		/// Fired when a client connects. Connection can be blocked by returning false
		/// </summary>
		/// <param name="client">the client</param>
		/// <returns>client is authorized to connect or not</returns>
		virtual bool OnClientConnect(Client client) = 0;

		/// <summary>
		/// Fired when a client disconnects
		/// </summary>
		/// <param name="client">the client</param>
		virtual void OnClientDisconnect(Client client) = 0;

		/// <summary>
		/// Fired when the server recieves a message
		/// </summary>
		/// <param name="client">source client</param>
		/// <param name="message">message</param>
		virtual void OnMessageRecieved(Client client, Message& message) = 0;
		

	protected:
		asio::io_context context;
		std::thread runningThread;
		BlockingQueue<OwnedMessage> messagesIn;
		std::deque<Client> clients;
		asio::ip::tcp::acceptor acceptor;
		uint32_t clientsIdCounter;
	};
}

#pragma region Server implementation

inline bool net::Server::Start()
{
	try
	{
		WaitForConnection();
		runningThread = std::thread([this]() {context.run(); });
	}
	catch (std::exception& e)
	{

		return false;
	}

	return true;
}

inline void net::Server::Stop()
{
	context.stop();

	if (runningThread.joinable())
		runningThread.join();
}

inline void net::Server::ProcessMessages(std::size_t maxMsgCount = -1, bool sleep = false)
{
	if (sleep)
		messagesIn.Wait(); 

	std::size_t processed = 0;
	while (processed < maxMsgCount && !messagesIn.Empty())
	{
		auto message = messagesIn.PopFront();
		OnMessageRecieved(message.remote, message.message);
		processed++;
	}
}

inline void net::Server::MessageClient(Client client, const Message& message)
{
	if (client && client->IsConnected())
	{
		client->Send(message);
	}
	else
	{
		OnClientDisconnect(client);
		client.reset();

		clients.erase(std::remove(std::begin(clients), std::end(clients), client), std::end(clients));
	}
}

inline void net::Server::MessageAllClients(const Message& message, Client ignore)
{
	bool invalidClients = false;

	for (auto& client : clients)
	{
		if (client && client->IsConnected())
		{
			if (client != ignore)
				client->Send(message);
		}
		else
		{
			OnClientDisconnect(client);
			client.reset();
			invalidClients = true;
		}
	}

	if (invalidClients)
		clients.erase(std::remove(std::begin(clients), std::end(clients), nullptr), std::end(clients));
}

inline void net::Server::WaitForConnection()
{
	acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				Client newConnection =
					std::make_shared<ClientConnection>(context, std::move(socket), messagesIn);

				if (OnClientConnect(newConnection))
				{
					clients.push_back(std::move(newConnection));
					clients.back()->ConnectToClient(this, clientsIdCounter++);

					// CONNECTION APPROVED
				}
				else
				{
					// CONNECTION DENIED
				}
			}
			else
			{
				// ERROR
			}

			WaitForConnection();
		});
}

#pragma endregion

#pragma region Client methods implementation


inline bool net::Client::Connect(const std::string& host, const uint16_t port)
{
	try
	{
		asio::ip::tcp::resolver resolver(context);
		asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
		
		connection = std::make_unique<ServerConnection>(context, asio::ip::tcp::socket(context), messagesIn);
		connection->ConnectToServer(endpoints);
		runningThread = std::thread([this]() { context.run(); });
	}
	catch (std::exception& e)
	{
		return false;
	}

	return true;
}

inline bool net::Client::IsConnected() const
{
	return connection ? connection->IsConnected() : false;
}

inline void net::Client::Disconnect()
{
	if (IsConnected())
		connection->Disconnect();

	context.stop();

	if (runningThread.joinable())
		runningThread.join();

	connection.release();
}

inline void net::Client::Send(const Message& msg) const
{
	connection->Send(msg);
}
#pragma endregion