#pragma once
#include "Networking.h"

class RemoteServer :
    public net::Server
{
public:
	RemoteServer(uint16_t port);
    void OnClientValidated(Client client) override;

protected:
	bool OnClientConnect(Client client) override;
	void OnClientDisconnect(Client client) override;
	void OnMessageRecieved(Client client, net::Message& message) override;
};

