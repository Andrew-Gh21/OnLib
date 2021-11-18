#include "Book.h"

void data::Book::Serialize(net::Message& msg, Book book)
{
	msg << book.id << book.bookId << book.bestBookId << book.workId << book.booksCount << book.isbn << book.isbn13 << book.authors << book.originalPublicationYear << book.originalTitle;
}

data::Book data::Book::Deserialize(net::Message& msg)
{
	Book book;
	msg >> book.originalTitle >> book.originalPublicationYear >> book.authors >> book.isbn13 >> book.isbn >> book.booksCount >> book.workId >> book.bestBookId >> book.bookId >> book.id;

	return book;
}