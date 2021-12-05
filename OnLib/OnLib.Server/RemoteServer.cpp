#include "RemoteServer.h"
#include <iostream>

RemoteServer::RemoteServer(uint16_t port, const MultiLogger& logger, const sqlite::database& db) :
	net::Server(port), logger(logger), database(db), accountsManager(database), booksManager(database)
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
	msg.header.messageType = static_cast<uint16_t>(data::ServerResponse::ValidationSuccessfull);
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
	auto request = static_cast<data::ClientRequest>(message.header.messageType);;

	switch (request)
	{
	case data::ClientRequest::Login:
	{
		data::User userData;
		net::Deserialize(message, userData);
		SendLoginResponse(client, userData);
		break;
	}
		
	case data::ClientRequest::Register:
	{
		data::User userData;
		net::Deserialize(message, userData);
		SendRegisterResponse(client, userData);
		break;
	}
	case data::ClientRequest::RequestDisplayBooks:
	{
		std::vector<data::Book> displayBooks; // = booksManager.GetDisplayBooks()
		net::Message response;
		response.header.messageType = static_cast<int32_t>(data::ServerResponse::DisplayBooksRecieved);
		// response << displayBooks;
		MessageClient(client, response);
		break;
	}
	default:
		log(LogMessageType::Warning, "User " + std::to_string(client->GetId()) + " sent an invalid message. Disconnecting!");
		client->Disconnect();
		break;
	}
}

void RemoteServer::SendLoginResponse(Client client, data::User data)
{
	net::Message response;
	std::vector<data::LoginErrors> errors;

	bool logInSuccesfull = accountsManager.ValidateLogin(client, data, errors);
	response.header.messageType = static_cast<uint8_t>(logInSuccesfull ? 
		data::ServerResponse::SuccesfullLogin : data::ServerResponse::InvalidLogin);

	if (logInSuccesfull)
	{
		accountsManager.Login(client->GetId(), data);
	}
	else
	{
		net::Serialize(response, std::cbegin(errors), std::cend(errors));
	}

	MessageClient(client, response);
}

void RemoteServer::SendRegisterResponse(Client client, data::User data)
{
	net::Message response;
	std::vector<data::RegisterErrors> errors;

	bool registerSuccessfull = accountsManager.ValidateRegister(client, data, errors);
	response.header.messageType = static_cast<uint8_t>(registerSuccessfull ?
		data::ServerResponse::SuccesfullRegister : data::ServerResponse::InvalidRegister);

	if (!registerSuccessfull)
		net::Serialize(response, std::cbegin(errors), std::cend(errors));

	MessageClient(client, response);
}