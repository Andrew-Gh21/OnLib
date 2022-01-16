#include "AccountsManager.h"

bool AccountsManager::ValidateLogin(std::shared_ptr<net::ClientConnection> client, const data::User& input, std::vector<data::LoginErrors>& errors)
{
	constexpr static const char* queryMatchName = "select count(*) from user where name = ? ";
	constexpr static const char* queryMatchPassword = "select count(*) from user where name = ? and password = ? ";
	
	bool passwordMatch;
	bool userMatch;

	database << queryMatchName << input.name
		>> userMatch;

	database << queryMatchPassword << input.name << input.password
		>> passwordMatch;

	if (userMatch == false)
	{
		errors.push_back(data::LoginErrors::InvalidUser);
	}
	
	if (passwordMatch == false)
	{
		errors.push_back(data::LoginErrors::InvalidPassword);
	}

	if (users.find(client->GetId()) != users.end())
	{
		errors.push_back(data::LoginErrors::UserAlreadyConnected);
	}

	return errors.empty();
}

bool AccountsManager::ValidateRegister(std::shared_ptr<net::ClientConnection> client, const data::User& input, std::vector<data::RegisterErrors>& errors)
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

	constexpr static const char* queryUserExists = "select count(*) from user where name = ? ";

	int count;

	database << queryUserExists << input.name 
		>> count;

	if (count == 1)
	{
		errors.push_back(data::RegisterErrors::UsernameAlreadyExists);
	}
	
	return errors.empty();
}

void AccountsManager::SaveRegisterDetails(const data::User& user)
{
	constexpr static const char* queryInsertion = "insert into user (name,password) values ( ? , ? ) ";
	database << queryInsertion << user.name << user.password;
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
	constexpr static const char* queryUserMatch =
		"select count(*) from user "
		"where user.id = ? and user.password = ? ";

	constexpr static const char* queryDeleteUser =
		"delete from user "
		"where user.id = ? ";

	auto input = users[clientId];
	bool passwordMatch;

	database << queryUserMatch << input.id << password
		>> passwordMatch;

	if (passwordMatch)
	{
		database << queryDeleteUser << input.id;
		return true;
	}

	return false;
}
