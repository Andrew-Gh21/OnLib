#pragma once
#include <unordered_map>
#include <cstdint>
#include <regex>

#include "Connection.h"

#include "User.h"
#include "Errors.h"
#include "sqlite_modern_cpp.h"

class AccountsManager
{
private:
	std::unordered_map<uint64_t, data::User>users;
	sqlite::database& database;

public:
	AccountsManager(sqlite::database& db) :database(db) {};
	bool ValidateLogin(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::LoginErrors>& errors);
	bool ValidateRegister(std::shared_ptr<net::ClientConnection> client, data::User input,std::vector<data::RegisterErrors>&errors);
	void Login(uint64_t clientId, data::User user);
	void Logout(uint64_t clientId);
	bool DeleteUser(uint64_t clientId, const std::string& password);
	bool ChangePassword(std::shared_ptr<net::ClientConnection> client, data::User input, const std::string& newPassword);
	bool ChangeUserName(std::shared_ptr<net::ClientConnection> client, data::User input, const std::string& newName);
	uint64_t GetUserId(uint64_t clientId) const noexcept { return users.at(clientId).id; }
	data::User GetUser(uint64_t clientId) const noexcept { return users.at(clientId); }
};

