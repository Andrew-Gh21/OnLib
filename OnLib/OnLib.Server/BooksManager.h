#pragma once
#include <unordered_map>
#include <cstdint>
#include <iomanip>
#include <regex>

#include "Connection.h"

#include "ILogger.h"
#include "Book.h"
#include "Enums.h"
#include "sqlite_modern_cpp.h"

class BooksManager
{
public:
	BooksManager(sqlite::database& db) :database(db) {};
	BooksManager() = default;

	BooksManager(const BooksManager& aux) = default;
	BooksManager& operator=(const BooksManager& aux) = default;

	BooksManager(BooksManager&& aux) = default;
	BooksManager& operator=(BooksManager&& aux) = default;

	~BooksManager() = default;
	
	bool SetupSearchExtension(std::string& errorMsg);

	void AddLendedBookToUser(uint64_t bookId, uint64_t userId);
	bool CanLendBook(uint64_t bookId, uint64_t userId);

	std::string AddFourteenDays(const std::string& date);
	
	void ReturnBook(uint64_t bookId, uint64_t userId);
	void ExtendDate(uint64_t bookId, uint64_t userId);
	void Rate(uint64_t bookId, uint64_t userId, int rating);
	std::vector<data::Book> Search(const std::string& keyword);
	
	bool CheckIfAvailable(const std::string& date);

	std::vector<data::Book> GetNewestFiveBooksFromEachCategory();
	std::vector<data::LendBook> GetLendedBooks(uint64_t userId);
	void GetAuthors(uint64_t bookId, std::vector<std::string>& authors);
	void GetCategories(data::Book& book);
	void GetRating(data::Book& book);

private:
	sqlite::database& database;
};

