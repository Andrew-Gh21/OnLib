#include "AccountsManager.h"

bool AccountsManager::ValidateLogin(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::LogginErrors>& errors)
{
	if (std::regex_match(input.name, std::regex("/^|\s+/")))
	{
		errors.push_back(data::LogginErrors::InvalidUser);
	}
	if (std::regex_match(input.password, std::regex("/^|\s+/")))
	{
		errors.push_back(data::LogginErrors::InvalidPassword);
	}
	if (users[client->GetId()] != input)
	{
		errors.push_back(data::LogginErrors::UserAlreadyConnected);
	}

	return errors.empty();
}

bool AccountsManager::ValidateRegister(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::RegisterErrors>& errors)
{
	if (std::regex_match(input.name,std::regex("/^|\s+/")))
	{
		errors.push_back(data::RegisterErrors::InvalidUsername);
	}
	if (std::regex_match(input.password, std::regex("/^|\s+/")))
	{
		errors.push_back(data::RegisterErrors::InvalidPassword);
	}

	uint8_t count;
	database << "select count(*)from user"
				"where name = ? " << input.name >> count;
	if (count == 1)
	{
		errors.push_back(data::RegisterErrors::UsernameAlreadyExists);
	}
	return errors.empty();
}

void AccountsManager::Login(uint64_t clientId, data::User user)
{
	users.insert(std::make_pair(clientId, user));
}

void AccountsManager::Logout(uint64_t clientId)
{
	users.erase(clientId);
}
