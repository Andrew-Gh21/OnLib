#include "Book.h"

void data::Book::Serialize(net::Message& msg, Book book)
{
	msg << book.id << book.isbn << book.title << book.coverUrl << book.mainCategory << book.otherCategories << book.authors << book.rating;
}

data::Book data::Book::Deserialize(net::Message& msg)
{
	Book book;
	msg >> book.rating >> book.authors >> book.otherCategories >> book.mainCategory >> book.coverUrl >> book.title >> book.isbn << book.id;

	return book;
}