#pragma once
#include "Networking.h"
#include "MultiLogger.h"

#pragma comment(lib, "sqlite3.lib")
#include "sqlite_modern_cpp.h"

#include "User.h"
#include "Enums.h"
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
	void SendLoginResponse(Client client, const data::User& data);
	void SendRegisterResponse(Client client, const data::User& data);
	void SendSearchResponse(Client client, const std::vector<data::Book>& books);

private:
	MultiLogger logger;
	AccountsManager accountsManager;
	BooksManager booksManager;
	sqlite::database database;
};

