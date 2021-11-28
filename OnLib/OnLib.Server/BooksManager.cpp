#include "BooksManager.h"

void BooksManager::AddBook(data::Book book)
{
	database << "insert into book (bookId,bestBookId,workId,booksCount,isbn,isbn13,authors,originalPublicationYear,originalTitle) values (?,?,?,?,?,?,?,?,?)"
		<< book.bookId << book.bestBookId << book.workId << book.booksCount << book.isbn << book.isbn13 << book.authors << book.originalPublicationYear << book.originalTitle;
}

bool BooksManager::GetBookByOriginalTitle(std::string title, data::Book& book)
{
	database << "select * from book where original_title = ?"
		<< title
		>> [&book](uint64_t id,uint64_t bookId,uint64_t bestBookId,uint64_t workId,uint32_t booksCount,std::string isbn,std::string isbn13,std::string authors,int32_t originalPublicationYear,std::string originalTitle)
	{
		if (id != NULL)
		{
			book.id = id;
			book.bookId = bookId;
			book.bestBookId = bestBookId;
			book.workId = workId;
			book.booksCount = booksCount;
			book.isbn = isbn;
			book.isbn13 = isbn13;
			book.authors = authors;
			book.originalPublicationYear = originalPublicationYear;
			book.originalTitle = originalTitle;
			return true;
		}
	};
    return false;
}

bool BooksManager::GetBookByIsbn(std::string isbn, data::Book& book)
{
	database << "select * from book where isbn = ?"
		<< isbn
		>> [&book](uint64_t id, uint64_t bookId, uint64_t bestBookId, uint64_t workId, uint32_t booksCount, std::string isbn, std::string isbn13, std::string authors, int32_t originalPublicationYear, std::string originalTitle)
	{
		if (id != NULL)
		{
			book.id = id;
			book.bookId = bookId;
			book.bestBookId = bestBookId;
			book.workId = workId;
			book.booksCount = booksCount;
			book.isbn = isbn;
			book.isbn13 = isbn13;
			book.authors = authors;
			book.originalPublicationYear = originalPublicationYear;
			book.originalTitle = originalTitle;
			return true;
		}
	};
	return false;
}

bool BooksManager::GetBookByIsbn13(std::string isbn13, data::Book& book)
{
	database << "select * from book where isbn = ?"
		<< isbn13
		>> [&book](uint64_t id, uint64_t bookId, uint64_t bestBookId, uint64_t workId, uint32_t booksCount, std::string isbn, std::string isbn13, std::string authors, int32_t originalPublicationYear, std::string originalTitle)
	{
		if (id != NULL)
		{
			book.id = id;
			book.bookId = bookId;
			book.bestBookId = bestBookId;
			book.workId = workId;
			book.booksCount = booksCount;
			book.isbn = isbn;
			book.isbn13 = isbn13;
			book.authors = authors;
			book.originalPublicationYear = originalPublicationYear;
			book.originalTitle = originalTitle;
			return true;
		}
	};
	return false;
}

bool BooksManager::GetAllBooksByAuthors(std::string authors, std::vector<data::Book>& books)
{
	return false;
}
