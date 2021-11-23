#include "MockBooksAndUsers.h"
#include "User.h"

MockBooksAndUsers::MockBooksAndUsers()
{
	users.push_back(User("admin", "admin"));
	users.push_back(User("test", "test"));
	books.push_back(Book("name1", "author1", "1", "desc1", TypeOfBook::SF));
	books.push_back(Book("name2", "author2", "2", "desc2", TypeOfBook::Action));
	books.push_back(Book("name3", "author3", "3", "desc3", TypeOfBook::Comedy));
	books.push_back(Book("name4", "author4", "4", "desc4", TypeOfBook::Drama));
	books.push_back(Book("name5", "author5", "5", "desc5", TypeOfBook::Romance));
	books.push_back(Book("name6", "author6", "6", "desc6", TypeOfBook::SF));
	books.push_back(Book("name7", "author7", "7", "desc7", TypeOfBook::SF));
	books.push_back(Book("name8", "author8", "8","desc8", TypeOfBook::SF));
	books.push_back(Book("name9", "author9", "9", "desc9", TypeOfBook::Action));
	books.push_back(Book("name10", "author10", "10", "desc10", TypeOfBook::Action));
	books.push_back(Book("name11", "author11", "11", "desc11", TypeOfBook::Action));
	books.push_back(Book("name12", "author12", "12","desc12", TypeOfBook::Action));
}

std::vector<User> MockBooksAndUsers::GetUsers()
{
	return users;
}

std::vector<Book> MockBooksAndUsers::GetBooks()
{
	return books;
}

void MockBooksAndUsers::AddUser(User user)
{
	users.push_back(user);
}

void MockBooksAndUsers::AddBook(Book book)
{
	books.push_back(book);
}