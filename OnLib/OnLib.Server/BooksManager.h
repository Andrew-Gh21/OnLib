#pragma once
#include <unordered_map>
#include <cstdint>
#include <iomanip>
#include <regex>

#include "Connection.h"

#include "Book.h"
#include "Errors.h"
#include "sqlite_modern_cpp.h"

class BooksManager
{
private:
	sqlite::database& database;
public:
	BooksManager(sqlite::database& db) :database(db) {};
	std::vector<data::Book> GetNewestFiveBooksFromEachCategory();
	std::vector<data::LendBook>GetLendedBooks(uint64_t bookId);
	void AddLendedBookToUser(uint64_t bookId, uint64_t userId);
	void GetAuthors(data::Book& book);
	void GetAuthors(data::LendBook& book);
	void GetCategories(data::Book& book);
	void GetRating(data::Book& book);
	bool CheckIfAvailable(const std::string& date);
	void Rate(uint64_t bookId, uint64_t userId, int rating);
};

