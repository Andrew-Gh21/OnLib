#include "RemoteClient.h"
#include <iostream>

RemoteClient::RemoteClient(QObject* parent) :
	net::Client(), QObject(parent)
{
	log = [](LogMessageType type, const std::string& msg)
	{
		std::cout << msg;
	};
}

void RemoteClient::OnLoginRequest(const data::User& user)
{
	net::Message msg;
	msg.header.messageType = static_cast<uint16_t>(ClientRequest::Login);
	data::User::Serialize(msg, user);
	Send(msg);
}