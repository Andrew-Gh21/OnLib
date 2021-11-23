#include "AccountsManager.h"

bool AccountsManager::ValidateLogin(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::LogginErrors>& errors)
{
	std::string password;
	database << "select password from users where name = ?" << input.name >> password;

	if (std::regex_match(password, std::regex("/^|\s+/")))
	{
		errors.push_back(data::LogginErrors::InvalidUser);
	}
	
	if (password != input.password)
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
	if (std::regex_match(input.name,std::regex("/^|\s+/")) ||
		std::regex_match(input.password, std::regex("/^|\s+/")))
	{
		errors.push_back(data::RegisterErrors::EmptyField);
	}
	
	if (input.name.size() < 5)
	{
		errors.push_back(data::RegisterErrors::InvalidUsername);
	}

	if (std::regex_match(input.password, std::regex("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[a-zA-Z]).{8,}$"))!=true)
	{
		errors.push_back(data::RegisterErrors::InvalidPassword);
	}

	int count;
	database << "select count(*)from user where name = ? " 
			 << input.name >> count;

	if (count == 1)
	{
		errors.push_back(data::RegisterErrors::UsernameAlreadyExists);
	}
	else
	{
		if (errors.empty())
		{
			database << "insert into user (name,password) values (?,?)" << input.name << input.password;
		}
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
