#pragma once
#include <string>
#include <unordered_map>
#include <Message.h>
#include "Enums.h"

namespace data
{
	struct LendBook
	{
		uint64_t bookId;
		std::string lendDate;
		std::string returnDate;
		std::string limitDate;
		std::string title;
		std::string description;
		std::string coverUrl;
		std::vector<std::string>authors;
		bool isAvailable;

		LendBook(uint64_t bookId, 
			const std::string& lendDate, 
			const std::string& returnDate,
			const std::string& limitDate,
			const std::string& title, 
			const std::string& description, 
			const std::string& coverUrl) 
			: bookId(bookId), lendDate(lendDate), returnDate(returnDate), limitDate(limitDate), title(title), description(description), coverUrl(coverUrl), isAvailable(false) {}
		
		LendBook() = default;

		LendBook(const LendBook& aux) = default;
		LendBook& operator=(const LendBook& aux) = default;

		LendBook(LendBook&& aux) = default;
		LendBook& operator=(LendBook&& aux) = default;

		~LendBook() = default;
	};

	struct Book
	{
		uint64_t id;
		std::string isbn;
		std::string title;
		std::string description;
		std::string coverUrl;
		BookCategory mainCategory;
		std::vector<BookCategory> otherCategories;
		std::vector<std::string>authors;
		float rating;

		Book() = default;

		Book(uint64_t id, 
			const std::string& isbn, 
			const std::string& title, 
			const std::string& coverUrl, 
			BookCategory mainCategory, 
			const std::vector<BookCategory>& otherCategories, 
			const std::vector<std::string>& authors, 
			float rating);

		Book(uint64_t id, 
			const std::string& isbn, 
			const std::string& title, 
			const std::string& description,
			const std::string& coverUrl, 
			BookCategory mainCategory, 
			float rating);

		Book(const Book& aux) = default;
		Book& operator=(const Book& aux) = default;

		Book(Book&& aux) = default;
		Book& operator=(Book&& aux) = default;

		~Book() = default;
	};

}

namespace net
{
	template<>
	inline void Serialize(Message& message, const data::Book& book)
	{
		Serialize(message, book.id);
		Serialize(message, std::cbegin(book.isbn), std::cend(book.isbn));
		Serialize(message, std::cbegin(book.title), std::cend(book.title));
		Serialize(message, std::cbegin(book.description), std::cend(book.description));
		Serialize(message, std::cbegin(book.coverUrl), std::cend(book.coverUrl));
		Serialize(message, book.mainCategory);
		Serialize(message, std::cbegin(book.otherCategories), std::cend(book.otherCategories));

		for (const auto& author : book.authors)
		{
			Serialize(message, std::cbegin(author), std::cend(author));
		}

		Serialize(message, book.authors.size());
		Serialize(message, book.rating);
	}

	template<>
	inline void Deserialize(Message& message, data::Book& book)
	{
		Deserialize(message, book.rating);
		
		std::size_t authorsCount;
		Deserialize(message, authorsCount);
		auto inserter = std::back_inserter(book.authors);

		for (size_t i = 0; i < authorsCount; i++)
		{
			std::string author;
			Deserialize(message, author, true);
			*inserter++ = author;
		}

		Deserialize(message, book.otherCategories, true);
		Deserialize(message, book.mainCategory);
		Deserialize(message, book.coverUrl, true);
		Deserialize(message, book.description, true);
		Deserialize(message, book.title, true);
		Deserialize(message, book.isbn, true);
		Deserialize(message, book.id);
	}

	template<>
	inline void Serialize(Message& message, const data::LendBook& lendBook)
	{
		Serialize(message, lendBook.bookId);
		Serialize(message, lendBook.isAvailable);
		Serialize(message, std::cbegin(lendBook.title), std::cend(lendBook.title));
		Serialize(message, std::cbegin(lendBook.description), std::cend(lendBook.description));
		Serialize(message, std::cbegin(lendBook.coverUrl), std::cend(lendBook.coverUrl));
		Serialize(message, std::cbegin(lendBook.lendDate), std::cend(lendBook.lendDate));
		Serialize(message, std::cbegin(lendBook.limitDate), std::cend(lendBook.limitDate));
		Serialize(message, std::cbegin(lendBook.returnDate), std::cend(lendBook.returnDate));

		for (const auto& author : lendBook.authors)
		{
			Serialize(message, std::cbegin(author), std::cend(author));
		}

		Serialize(message, lendBook.authors.size());
	}

	template<>
	inline void Deserialize(Message& message, data::LendBook& lendBook)
	{
		std::size_t authorsCount;
		Deserialize(message, authorsCount);
		auto inserter = std::back_inserter(lendBook.authors);

		for (size_t i = 0; i < authorsCount; i++)
		{
			std::string author;
			Deserialize(message, author, true);
			*inserter++ = author;
		}

		Deserialize(message, lendBook.returnDate, true);
		Deserialize(message, lendBook.limitDate, true);
		Deserialize(message, lendBook.lendDate, true);
		Deserialize(message, lendBook.coverUrl, true);
		Deserialize(message, lendBook.description, true);
		Deserialize(message, lendBook.title, true);
		Deserialize(message, lendBook.isAvailable);
		Deserialize(message, lendBook.bookId);
	}
}