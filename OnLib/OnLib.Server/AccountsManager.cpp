#include "AccountsManager.h"

bool AccountsManager::ValidateLogin(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::LoginErrors>& errors)
{
	std::optional<std::string> optionalPassword;
	database << "select password from user where name = ?"
		<< input.name
		>> [&optionalPassword](std::optional<std::string>pass)
	{
		optionalPassword = pass;
	};

	if (!optionalPassword.has_value())
	{
		errors.push_back(data::LoginErrors::InvalidUser);
	}
	
	if (optionalPassword.has_value() && optionalPassword.value() != input.password)
	{
		errors.push_back(data::LoginErrors::InvalidPassword);
	}

	if (users.find(client->GetId()) != users.end())
	{
		errors.push_back(data::LoginErrors::UserAlreadyConnected);
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

bool AccountsManager::DeleteUser(std::shared_ptr<net::ClientConnection> client, data::User input)
{
	constexpr static const char* query =
		"select count(password) from user "
		"where user.id=? and user.password = ?";

	bool passwordMatch;
	auto output = [&passwordMatch](bool match)
	{
		passwordMatch = match;
	};

	database << query << input.id << input.password
		>> output;

	if (passwordMatch)
	{
		constexpr static const char* queryDeleteUser =
			"delete from user"
			"where user.id=?";

		database << queryDeleteUser << input.id;

		return true;
	}

	return false;
}
