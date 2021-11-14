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

		static void Serialize(net::Message& msg, Book book);
		static void Deserialize(net::Message& msg, Book book);
	};
};

