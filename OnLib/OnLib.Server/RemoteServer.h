#pragma once
#include "Networking.h"
#include "MultiLogger.h"

#pragma comment(lib, "sqlite3.lib")
#include "sqlite_modern_cpp.h"

#include "ClientRequest.h"
#include "ServerResponse.h"

class RemoteServer :
    public net::Server
{
public:
	RemoteServer(uint16_t port, const MultiLogger& logger, const sqlite::database& db);
    void OnClientValidated(Client client) override;

protected:
	bool OnClientConnect(Client client) override;
	void OnClientDisconnect(Client client) override;
	void OnMessageRecieved(Client client, net::Message& message) override;

private:
	MultiLogger logger;
	sqlite::database database;
};

