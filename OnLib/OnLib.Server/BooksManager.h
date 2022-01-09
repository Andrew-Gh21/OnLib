#pragma once
#include <unordered_map>
#include <cstdint>
#include <iomanip>
#include <regex>

#include "Connection.h"

#include "ILogger.h"
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
	std::vector<data::LendBook>GetLendedBooks(uint64_t userId);
	void AddLendedBookToUser(uint64_t bookId, uint64_t userId);
	void GetAuthors(uint64_t bookId, std::vector<std::string>& authors);
	void GetCategories(data::Book& book);
	void GetRating(data::Book& book);
	void ReturnBook(uint64_t bookId, uint64_t userId);
	bool CheckIfAvailable(const std::string& date);
	void Rate(uint64_t bookId, uint64_t userId, int rating);
	void Search();
};

