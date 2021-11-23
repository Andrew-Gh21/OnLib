#pragma once

#include "User.h"
#include "Book.h"
#include <vector>

class MockBooksAndUsers
{
private:
	std::vector<User> users;// = { new User("admin","admin"),new User("test","test") };
	std::vector<Book> books;// = { new Book("book1","author1","descr1"),new Book("book2","author2","descr2") ,new Book("book3","author3","descr3") ,new Book("book4","author4","descr4") };

public:
	MockBooksAndUsers();

	std::vector<User> GetUsers();
	std::vector<Book> GetBooks();

	void AddUser(User user);
	void AddBook(Book book);

	~MockBooksAndUsers() = default;
};