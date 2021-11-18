#pragma once
#include <string>
#include <Message.h>

namespace data
{
	struct Book
	{
		uint64_t id;
		uint64_t bookId;
		uint64_t bestBookId;
		uint64_t workId;
		uint32_t booksCount;
		std::string isbn;
		std::string isbn13;
		std::string authors;
		int32_t originalPublicationYear;
		std::string originalTitle;

		Book() :id(), bookId(), bestBookId(), workId(), booksCount(), isbn(), isbn13(), authors(), originalPublicationYear(), originalTitle() {}
		Book(uint64_t id,uint64_t bookId,uint64_t bestBookId,uint64_t workId,uint32_t booksCount,const std::string& isbn,const std::string& isbn13,const std::string& authors,int32_t originalPublicationYear,const std::string& originalTitle) :id(id), bookId(bookId), bestBookId(bestBookId), workId(workId), booksCount(booksCount), isbn(isbn), isbn13(isbn13), authors(authors), originalPublicationYear(originalPublicationYear), originalTitle(originalTitle) {}

		static void Serialize(net::Message& msg, Book book);
		static Book Deserialize(net::Message& msg);
	};
};