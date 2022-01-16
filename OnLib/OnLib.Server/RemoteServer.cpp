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

	std::string message;
	LogMessage msg;
	if (!booksManager.SetupSearchExtension(message))
		logger.Send(msg << message, LogSeverity::Error);
}

void RemoteServer::OnClientValidated(Client client)
{
	net::Message msg;
	msg.header.messageType = data::EnumToNumber(data::ServerResponse::ValidationSuccessfull);
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
	auto request = static_cast<data::ClientRequest>(message.header.messageType);

	switch (request)
	{
	case data::ClientRequest::Login:
	{
		data::User userData;
		net::Deserialize(message, userData);
		std::transform(userData.name.begin(), userData.name.end(), userData.name.begin(), 
			[](char c) {return std::tolower(c); });

		SendLoginResponse(client, userData);
		break;
	}

	case data::ClientRequest::Register:
	{
		data::User userData;
		net::Deserialize(message, userData);
		std::transform(userData.name.begin(), userData.name.end(), userData.name.begin(),
			[](char c) {return std::tolower(c); });

		SendRegisterResponse(client, userData);
		break;
	}
	case data::ClientRequest::Logout:
	{
		accountsManager.Logout(client->GetId());
		break;
	}
	case data::ClientRequest::DeleteAccount:
	{
		std::string password;
		net::Deserialize(message, password, true);

		net::Message response;
		data::ServerResponse result = accountsManager.DeleteUser(client->GetId(), password)
			? data::ServerResponse::DeleteAccountSuccesful : data::ServerResponse::DeleteAccountFailure;
		response.header.messageType = data::EnumToNumber(result);
		MessageClient(client, response);

		if (result == data::ServerResponse::DeleteAccountSuccesful)
			accountsManager.Logout(client->GetId());

		break;
	}
	case data::ClientRequest::RequestDisplayBooks:
	{
		std::vector<data::Book> displayBooks = booksManager.GetNewestFiveBooksFromEachCategory();
		net::Message response;
		response.header.messageType = data::EnumToNumber(data::ServerResponse::DisplayBooksRecieved);
		net::Serialize(response, std::cbegin(displayBooks), std::cend(displayBooks));
		MessageClient(client, response);
		break;
	}
	case data::ClientRequest::RequestLentBooks:
	{
		auto borrowed = booksManager.GetLendedBooks(accountsManager.GetUserId(client->GetId()));
		net::Message response;
		response.header.messageType = data::EnumToNumber(data::ServerResponse::BorrowedBooksRecieved);
		net::Serialize(response, std::cbegin(borrowed), std::cend(borrowed));
		MessageClient(client, response);
		break;
	}
	case data::ClientRequest::RateBook:
	{
		int rating;
		uint64_t bookId;
		message >> bookId >> rating;
		booksManager.Rate(bookId, accountsManager.GetUserId(client->GetId()), rating);
		break;
	}
	case data::ClientRequest::BorrowBook:
	{
		uint64_t bookId;
		message >> bookId;
		booksManager.AddLendedBookToUser(bookId, accountsManager.GetUserId(client->GetId()));
		break;
	}
	case data::ClientRequest::ReturnBook:
	{
		uint64_t bookId;
		message >> bookId;
		booksManager.ReturnBook(bookId, accountsManager.GetUserId(client->GetId()));
		break;
	}
	case data::ClientRequest::SearchBooks:
	{
		std::string keyword;
		net::Deserialize(message, keyword, true);
		auto books = booksManager.Search(keyword);
		SendSearchResponse(client, books);
		break;
	}
	case data::ClientRequest::ExtendReturnDate:
	{
		uint64_t bookId;
		message >> bookId;
		booksManager.ExtendDate(bookId, accountsManager.GetUserId(client->GetId()));
		break;
	}
	default:
		log(LogMessageType::Warning, "User " + std::to_string(client->GetId()) + " sent an invalid message. Disconnecting!");
		client->Disconnect();
		break;
	}
}

void RemoteServer::SendLoginResponse(Client client, const data::User& data)
{
	net::Message response;
	std::vector<data::LoginErrors> errors;

	bool logInSuccesfull = accountsManager.ValidateLogin(client, data, errors);
	response.header.messageType = data::EnumToNumber(logInSuccesfull ?
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

void RemoteServer::SendRegisterResponse(Client client, const data::User& data)
{
	net::Message response;
	std::vector<data::RegisterErrors> errors;

	bool registerSuccessfull = accountsManager.ValidateRegister(client, data, errors);

	response.header.messageType = data::EnumToNumber(registerSuccessfull ?
		data::ServerResponse::SuccesfullRegister : data::ServerResponse::InvalidRegister);

	if (registerSuccessfull)
		accountsManager.SaveRegisterDetails(data);
	else
		net::Serialize(response, std::cbegin(errors), std::cend(errors));

	MessageClient(client, response);
}

void RemoteServer::SendSearchResponse(Client client, const std::vector<data::Book>& books)
{
	net::Message response;
	response.header.messageType = data::EnumToNumber(data::ServerResponse::SearchedBooksRecieved);
	net::Serialize(response, std::cbegin(books), std::cend(books));
	MessageClient(client, response);
}
