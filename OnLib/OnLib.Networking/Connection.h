#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"

#include <memory>
#include <functional>
#include "BlockingQueue.h"
#include "Message.h"
#include "LogMessageType.h"

namespace net
{
	class Server;

	/// <summary>
	/// Connection base class for handling remote connections
	/// </summary>
	class Connection
	{
	protected:
		using LogFunction = std::function<void(LogMessageType, const std::string& message)>;
	public:
		Connection(LogFunction& f, asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in) :
			log(f), context(context), socket(std::move(socket)), messagesIn(in) {}
		virtual ~Connection() {};

		void Disconnect();
		bool IsConnected() const;

		/// <summary>
		/// Send a message to remote
		/// </summary>
		/// <param name="message">message to be sent</param>
		/// <returns></returns>
		void Send(const Message& message);

		void SetResultHandler(LogFunction& handler) { log = handler; }

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
		std::function<void(LogMessageType, const std::string& message)>& log;
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
		ServerConnection(LogFunction& log, asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);

		void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

	protected:
		void WriteValidation() override;
		void ReadValidation();
		void AddToQueue() override;
	};

	class ClientConnection : public Connection, public std::enable_shared_from_this<ClientConnection>
	{
	public:
		ClientConnection(LogFunction& log, asio::io_context& context, asio::ip::tcp::socket socket, BlockingQueue<OwnedMessage>& in);
		void ConnectToClient(Server* remote, uint32_t id);

		int GetId() const noexcept { return id; }

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