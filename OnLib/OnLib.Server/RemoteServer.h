#pragma once
#include "Networking.h"
#include "MultiLogger.h"

#pragma comment(lib, "sqlite3.lib")
#include "sqlite_modern_cpp.h"

#include "ClientRequest.h"
#include "ServerResponse.h"
#include "User.h"
#include "Errors.h"
#include "AccountsManager.h"
#include "BooksManager.h"

class RemoteServer :
    public net::Server
{
public:
	RemoteServer(uint16_t port, const MultiLogger& logger, const sqlite::database& db);
	RemoteServer() = default;
    void OnClientValidated(Client client) override;

	RemoteServer(const RemoteServer& other) = delete;
	RemoteServer(RemoteServer&& other) = default;

	RemoteServer& operator=(const RemoteServer& other) = delete;
	RemoteServer& operator=(RemoteServer&& other) = default;

	~RemoteServer() = default;

protected:
	bool OnClientConnect(Client client) override;
	void OnClientDisconnect(Client client) override;
	void OnMessageRecieved(Client client, net::Message& message) override;

private:
	void SendLoginResponse(Client client, data::User data);
	void SendRegisterResponse(Client client, data::User data);
private:
	MultiLogger logger;
	AccountsManager accountsManager;
	BooksManager booksManager;
	sqlite::database database;
};

