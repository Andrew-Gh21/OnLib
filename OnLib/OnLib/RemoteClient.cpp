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
	msg.header.messageType = static_cast<uint16_t>(ClientRequest::Login);
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
	ServerResponse response = static_cast<ServerResponse>(msg.header.messageType);

	switch (response)
	{
	case ServerResponse::ValidationSuccessfull:
		break;
	case ServerResponse::SuccesfullLogin:
		emit LoginSuccessfull();
		break;
	case ServerResponse::InvalidLoggin:
	{
		std::vector<data::LogginErrors> errors;
		msg >> errors;
		emit LoginInvalid(errors);
		break;
	}
	case ServerResponse::SuccesfullRegister:
		emit RegisterSuccesfull();
		break;
	case ServerResponse::InvalidRegister:
	{
		std::vector<data::RegisterErrors> errors;
		msg >> errors;
		emit RegisterInvalid(errors);
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
