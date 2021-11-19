#pragma once
#include <unordered_map>
#include <cstdint>
#include <regex>
#include "User.h"
#include "Errors.h"
#include "sqlite_modern_cpp.h"
#include "RemoteServer.h"

class AccountsManager
{
private:
	std::unordered_map<uint64_t, data::User>users;
	sqlite::database& database;

public:
	AccountsManager(sqlite::database& db) :database(db) {};
	bool ValidateLogin(RemoteServer::Client client, data::User input, std::vector<data::LogginErrors>& errors);
	bool ValidateRegister(RemoteServer::Client client, data::User input,std::vector<data::RegisterErrors>&errors);
	void Login(uint64_t clientId, data::User user);
	void Logout(uint64_t clientId);
};

