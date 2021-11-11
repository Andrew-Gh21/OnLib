#include "Remote.h"

namespace net 
{
#pragma region Server implementation

	bool Server::Start()
	{
		try
		{
			WaitForConnection();
			log(LogMessageType::Info, "Server started");
			runningThread = std::thread([this]() {context.run(); });
		}
		catch (std::exception& e)
		{

			return false;
		}

		return true;
	}

	void Server::Stop()
	{
		context.stop();

		if (runningThread.joinable())
			runningThread.join();
	}

	void Server::ProcessMessages(std::size_t maxMsgCount = -1, bool sleep = false)
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

	void Server::MessageClient(Client client, const Message& message)
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

	void Server::MessageAllClients(const Message& message, Client ignore)
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

	void Server::WaitForConnection()
	{
		acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					Client newConnection =
						std::make_shared<ClientConnection>(log, context, std::move(socket), messagesIn);

					if (OnClientConnect(newConnection))
					{
						clients.push_back(std::move(newConnection));
						clients.back()->ConnectToClient(this, clientsIdCounter++);

						log(LogMessageType::Info, "Client connected!");
					}
					else
					{
						log(LogMessageType::Warning, "Client's connection refused!");
					}
				}
				else
				{
					log(LogMessageType::Error, "Error on client connection!");
				}

				WaitForConnection();
			});
	}

#pragma endregion

#pragma region Client methods implementation


	bool Client::Connect(const std::string& host, const uint16_t port)
	{
		try
		{
			asio::ip::tcp::resolver resolver(context);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

			connection = std::make_unique<ServerConnection>(log, context, asio::ip::tcp::socket(context), messagesIn);
			connection->ConnectToServer(endpoints);
			runningThread = std::thread([this]() { context.run(); });
		}
		catch (std::exception& e)
		{
			return false;
		}

		return true;
	}

	bool Client::IsConnected() const
	{
		return connection ? connection->IsConnected() : false;
	}

	void Client::Disconnect()
	{
		if (IsConnected())
			connection->Disconnect();

		context.stop();

		if (runningThread.joinable())
			runningThread.join();

		connection.release();
	}

	void Client::Send(const Message& msg) const
	{
		connection->Send(msg);
	}
#pragma endregion
}