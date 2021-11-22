#include "User.h"

std::string User::GetUsername()
{
	return m_username;
}

std::string User::GetPassword()
{
	return m_password;
}

std::vector<BorrowBook> User::GetBorrowedBooks()
{
	return m_borrowedBooks;
}

void User::AddBorrowBook(const BorrowBook &book)
{
	m_borrowedBooks.push_back(book);
}

void User::SetUsername(const std::string &username)
{
	this->m_username = username;
}

void User::SetPassword(const std::string &password)
{
	this->m_password = password;
}

void User::DecrementBorrowLimit()
{
	borrowLimit--;
}

void User::IncrementBorrowLimit()
{
	borrowLimit++;
}

void User::RemoveBorrowedBook(Book book)
{
	//to be efficientized


	//m_borrowedBooks.erase(std::remove_if(m_borrowedBooks.begin(), m_borrowedBooks.end(), [book](BorrowBook bw) {return bw.GetBook() = book; }));


	for (int i = 0; i < m_borrowedBooks.size(); i++)
	{
		Book bk = m_borrowedBooks[i].GetBook();
		if (bk.GetAuthor() == book.GetAuthor() && bk.GetName()==book.GetName())
		{
			m_borrowedBooks.erase(m_borrowedBooks.begin() + i);
		}
	}
}

unsigned int User::GetBorrowLimit()
{
	return borrowLimit;
}

User::User(std::string username, std::string password):m_username(username),m_password(password)
{
	borrowLimit = 5;
}