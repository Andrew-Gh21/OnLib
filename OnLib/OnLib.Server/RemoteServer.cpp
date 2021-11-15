#include "RemoteServer.h"
#include <iostream>
#include <User.h>


RemoteServer::RemoteServer(uint16_t port, const MultiLogger& logger, const sqlite::database& db) :
	net::Server(port), logger(logger), database(db)
{
	log = [&logger](LogMessageType type, const std::string& msg)
	{
		static std::unordered_map<LogMessageType, LogSeverity> convertor = {
			{LogMessageType::Info, LogSeverity::Info},
			{LogMessageType::Warning, LogSeverity::Warn},
			{LogMessageType::Error, LogSeverity::Error},

		};

		LogMessage message;
		message << msg;

		logger.Send(message, convertor[type]);
	};
}

void RemoteServer::OnClientValidated(Client client)
{
	net::Message msg;
	msg.header.messageType = static_cast<uint16_t>(ServerResponse::ValidationSuccessfull);
	client->Send(msg);
}

bool RemoteServer::OnClientConnect(Client client)
{
	return true;
}

void RemoteServer::OnClientDisconnect(Client client)
{
	if (client)
	{
		log(LogMessageType::Info, "User" + std::to_string(client->GetId()) + " disconnected");
	}
}

void RemoteServer::OnMessageRecieved(Client client, net::Message& message)
{
	std::cout << message;
	auto action = static_cast<ClientRequest>(message.header.messageType);
	data::User u;

	switch (action)
	{
	case ClientRequest::Login:
		u = data::User::Deserialize(message);
		log(LogMessageType::Info, "Login attempt: " + u.name + " , " + u.password);
		break;
	case ClientRequest::Register:
		break;
	default:
		log(LogMessageType::Warning, "User " + std::to_string(client->GetId()) + " sent an invalid message. Disconnecting!");
		client->Disconnect();
		break;
	}
}
