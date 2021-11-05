#pragma once
#include<string>
class User
{
private:
	std::string m_username;
	std::string m_password;
public:
	std::string GetUsername();
	std::string GetPassword();
	void SetUsername(std::string username);
	void SetPassword(std::string password);
	User()=default;
	User(std::string, std::string);
	~User()=default;
};