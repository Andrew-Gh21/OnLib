#pragma once
#include <string>
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

		Book() :id(), isbn(), title(), coverUrl(), mainCategory(), otherCategories(), authors(), rating() {}
		Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, const std::vector<BookCategory>& otherCategories, const std::vector<std::string>& authors, float rating) :id(id), isbn(isbn), title(title), coverUrl(coverUrl), mainCategory(mainCategory), otherCategories(otherCategories), authors(authors), rating(rating) {}
		static void Serialize(net::Message& msg, Book book);
		static Book Deserialize(net::Message& msg);
	};
}