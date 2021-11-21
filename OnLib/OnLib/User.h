#pragma once
#include<string>
#include"Book.h"
#include<set>
#include<cstdint>
#include<vector>
class User
{
private:
	std::string m_username;
	std::string m_password;
	unsigned int borrowLimit;
public:
	std::string GetUsername();
	std::string GetPassword();
	void SetUsername(std::string username);
	void SetPassword(std::string password);
	void DecrementBorrowLimit();
	void IncrementBorrowLimit();
	unsigned int GetBorrowLimit();
	User()=default;
	User(std::string, std::string);
	~User()=default;
};