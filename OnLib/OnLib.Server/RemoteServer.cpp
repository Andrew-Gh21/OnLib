#include "RemoteServer.h"
#include <iostream>

RemoteServer::RemoteServer(uint16_t port) :
	net::Server(port)
{
	log = [](LogMessageType type, const std::string& msg)
	{
		std::cout << msg;
	};
}

void RemoteServer::OnClientValidated(Client client)
{

}

bool RemoteServer::OnClientConnect(Client client)
{
	return true;
}

void RemoteServer::OnClientDisconnect(Client client)
{
}

void RemoteServer::OnMessageRecieved(Client client, net::Message& message)
{
}
