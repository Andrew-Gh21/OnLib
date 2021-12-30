#include "AccountsManager.h"

bool AccountsManager::ValidateLogin(std::shared_ptr<net::ClientConnection> client, data::User input, std::vector<data::LoginErrors>& errors)
{
	bool passwordMatch;
	bool userMatch;
	constexpr static const char* queryMatchPassword = "select count(password) from user where name = ? and password = ? ";
	constexpr static const char* queryMatchName = "select count(name) from user where name = ? ";

	database << queryMatchPassword << input.password 
		>> passwordMatch;

	database << queryMatchName << input.password
		>> userMatch;

	if (userMatch != false)
	{
		errors.push_back(data::LoginErrors::InvalidUser);
	}
	
	if (passwordMatch != false)
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
	constexpr static const char* query = "select count(*) from user where name = ? ";

	database << query << input.name 
		>> count;

	if (count == 1)
	{
		errors.push_back(data::RegisterErrors::UsernameAlreadyExists);
	}
	else
	{
		if (errors.empty())
		{
			constexpr static const char* queryInsertion = "insert into user (name,password) values ( ? , ? ) ";
			database << queryInsertion << input.name << input.password;
		}
	}
	return errors.empty();
}

void AccountsManager::Login(uint64_t clientId, data::User user)
{
	database << "select id from user where name = ?" << user.name >> user.id;
	users.insert(std::make_pair(clientId, user));
}

void AccountsManager::Logout(uint64_t clientId)
{
	users.erase(clientId);
}

bool AccountsManager::DeleteUser(uint64_t clientId, const std::string& password)
{
	constexpr static const char* query =
		"select count(password) from user "
		"where user.id = ? and user.password = ? ";

	auto input = users[clientId];
	bool passwordMatch;

	database << query << input.id << password
		>> passwordMatch;

	if (passwordMatch)
	{
		constexpr static const char* queryDeleteUser =
			"delete from user "
			"where user.id = ? ";

		database << queryDeleteUser << input.id;

		return true;
	}

	return false;
}

bool AccountsManager::ChangePassword(std::shared_ptr<net::ClientConnection> client, data::User input , const std::string& newPassword)
{

	constexpr static const char* queryChangePassword =
		"update user "
		"set password = ? "
		"where user.id = ? ";

	database << queryChangePassword << newPassword << input.id;

	bool succesful;

	constexpr static const char* queryVerify =
		"select count(password) from user "
		"where user.password = ? and user.id = ? ";

	database << queryVerify << newPassword << input.id
		>> succesful;

	if (succesful)
	{
		return true;
	}

	return false;
}

bool AccountsManager::ChangeUserName(std::shared_ptr<net::ClientConnection> client, data::User input, const std::string& newName)
{
	bool found;

	constexpr static const char* queryVerifyIfAlreadyExists =
		"select count(name) "
		"from user "
		"where user.name = ? ";

	database << queryVerifyIfAlreadyExists << newName
		>> found;

	if (!found)
	{
		constexpr static const char* queryChangeUserName =
			"update user "
			"set name = ? "
			"where user.id = ? ";

		database << queryChangeUserName << newName << input.id;

		bool succesful;

		constexpr static const char* queryVerify =
			"select count(name) from user "
			"where user.name = ? and user.id = ?";

		database << queryVerify << newName << input.id
			>> succesful;

		if (succesful)
		{
			return true;
		}
	}
	return false;
}
