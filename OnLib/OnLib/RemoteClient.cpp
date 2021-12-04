#include "RemoteClient.h"
#include "ServerResponse.h"
#include <iostream>

RemoteClient::RemoteClient(QObject* parent) :
	net::Client(), QObject(parent)
{
	log = [](LogMessageType type, const std::string& msg)
	{
		std::cout << msg;
	};

	processingThread = std::thread([this]()
		{
			while (true)
			{
				ProcessMessages(-1, true);
			}
		});
}

void RemoteClient::OnLoginRequest(const data::User& user)
{
	net::Message msg;
	msg.header.messageType = static_cast<uint16_t>(data::ClientRequest::Login);
	data::User::Serialize(msg, user);
	Send(msg);
}

void RemoteClient::OnRegisterRequest(const data::User& user)
{
	net::Message msg;
	msg.header.messageType = static_cast<uint16_t>(data::ClientRequest::Register);
	data::User::Serialize(msg, user);
	Send(msg);
}

RemoteClient::~RemoteClient()
{
	if (processingThread.joinable())
		processingThread.join();
}

void RemoteClient::OnMessageRecieved(net::Message& msg)
{
	data::ServerResponse response = static_cast<data::ServerResponse>(msg.header.messageType);

	switch (response)
	{
	case data::ServerResponse::ValidationSuccessfull:
		break;
	case data::ServerResponse::SuccesfullLogin:
	{
		net::Message requestBooksMessage;
		requestBooksMessage.header.messageType = static_cast<int32_t>(data::ClientRequest::RequestDisplayBooks);
		Send(requestBooksMessage);
		emit LoginSuccessfull();
		break;
	}
	case data::ServerResponse::InvalidLogin:
	{
		std::vector<data::LoginErrors> errors;
		msg >> errors;
		emit LoginInvalid(errors);
		break;
	}
	case data::ServerResponse::SuccesfullRegister:
		emit RegisterSuccesfull();
		break;
	case data::ServerResponse::InvalidRegister:
	{
		std::vector<data::RegisterErrors> errors;
		msg >> errors;
		emit RegisterInvalid(errors);
		break;
	}
	case data::ServerResponse::DisplayBooksRecieved:
	{
		std::vector<data::Book> books; // TODO
		emit DisplayBooksRecieved(books);
		break;
	}
	default:
		break;
	}
}

void RemoteClient::ProcessMessages(std::size_t maxCount, bool sleep)
{
	if (sleep)
		messagesIn.Wait();

	std::size_t processed = 0;
	while (processed < maxCount && !messagesIn.Empty())
	{
		auto message = messagesIn.PopFront();
		OnMessageRecieved(message.message);
		processed++;
	}
}
