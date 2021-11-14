#include "User.h"

std::string User::GetUsername()
{
	return m_username;
}

std::string User::GetPassword()
{
	return m_password;
}

void User::SetUsername(std::string username)
{
	this->m_username = username;
}

void User::SetPassword(std::string password)
{
	this->m_password = password;
}

unsigned int User::GetBorrowLimit()
{
	return borrowLimit;
}

User::User(std::string username, std::string password):m_username(username),m_password(password)
{
	borrowLimit = 3;
}