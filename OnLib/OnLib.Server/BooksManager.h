#pragma once
#include <unordered_map>
#include <cstdint>
#include <regex>

#include "Connection.h"

#include "Book.h"
#include "Errors.h"
#include "sqlite_modern_cpp.h"

class BooksManager
{
private:
	sqlite::database& database;
	void GetAuthorsAndOtherCategories(const std::vector<data::Book>& books);

public:
	BooksManager(sqlite::database& db) :database(db) {};
	std::vector<data::Book> GetNewestFiveBooksFromEachCategory();
	std::vector<data::LendBook>GetLendedBooks(uint64_t bookId);
	void AddLendedBookToUser(uint64_t bookId, uint64_t userId);
};

