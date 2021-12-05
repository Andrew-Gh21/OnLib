#pragma once
#include <string>
#include <unordered_map>
#include <Message.h>
#include "BookCategory.h"

namespace data
{
	struct Book
	{
		uint64_t id;
		std::string isbn;
		std::string title;
		std::string coverUrl;
		BookCategory mainCategory;
		std::vector<BookCategory> otherCategories;
		std::vector<std::string>authors;
		float rating;
		std::unordered_map<data::BookCategory, std::string> bookCategory;

		Book();
		Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, const std::vector<BookCategory>& otherCategories, const std::vector<std::string>& authors, float rating);
		Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, float rating);
		void InitCategories();

	};

	inline std::string ParseCategoryToString(Book& book)
	{
		return book.bookCategory[book.mainCategory];
	}
	inline data::BookCategory ParseCategoryType(const std::string& category)
	{
		if (category == "SF")
		{
			return BookCategory::SF;
		}
		if (category == "Romantic")
		{
			return BookCategory::Romantic;
		}
		if (category == "Action")
		{
			return BookCategory::Action;
		}
		if (category == "Comedy")
		{
			return BookCategory::Comedy;
		}
		throw std::exception{ "Unknown product type" };

	}
}

namespace net
{
	template<>
	inline void Serialize(Message& message, const data::Book& book)
	{
		Serialize(message, book.id);
		Serialize(message, std::cbegin(book.isbn), std::cend(book.isbn));
		Serialize(message, std::cbegin(book.title), std::cend(book.title));
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
		Deserialize(message, book.title, true);
		Deserialize(message, book.isbn, true);
	}
}