#include "MockBooksAndUsers.h"
#include"User.h"

void MockBooksAndUsers::AddUser(User user)
{
    users.push_back(user);
}

void MockBooksAndUsers::AddBook(Book book)
{
    books.push_back(book);
}

std::vector<User> MockBooksAndUsers::GetUsers()
{
    return users;
}

MockBooksAndUsers::MockBooksAndUsers()
{
    users.push_back(User("admin", "admin"));
    users.push_back(User("test", "test"));
    books.push_back(Book("name1", "author1", "desc1"));
    books.push_back(Book("name2", "author2", "desc2"));
    books.push_back(Book("name3", "author3", "desc3"));
    books.push_back(Book("name4", "author4", "desc4"));
}




