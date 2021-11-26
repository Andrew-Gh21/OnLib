#pragma once

#include <string>
#include "Book.h"
#include "BorrowBook.h"
#include <set>
#include <cstdint>
#include <vector>

class User
{
private:
	std::string m_username;
	std::string m_password;
	std::vector<BorrowBook> m_borrowedBooks;
	unsigned int borrowLimit = 5;

public:
	std::string GetUsername()const;
	std::string GetPassword()const;
	std::vector<BorrowBook> GetBorrowedBooks()const;
	void AddBorrowBook(const BorrowBook& book);
	void SetUsername(const std::string& username);
	void SetPassword(const std::string& password);
	void DecrementBorrowLimit();
	void IncrementBorrowLimit();
	void RemoveBorrowedBook(Book book);
	unsigned int GetBorrowLimit()const;
	User() = default;
	User(std::string, std::string);
	~User() = default;
};