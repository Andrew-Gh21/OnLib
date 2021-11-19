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
	public:
		using LogFunction = std::function<void(LogMessageType, const std::string& message)>;
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
		LogFunction log;
	};

	/// <summary>
	/// Remote server class
	/// </summary>
	class Server
	{
	public:
		using LogFunction = std::function<void(LogMessageType, const std::string& message)>;
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
		LogFunction log;
	};
}