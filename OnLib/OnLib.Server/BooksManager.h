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

public:
	BooksManager(sqlite::database& db) :database(db) {};

	void AddBook(data::Book book);
	bool GetBookByOriginalTitle(std::string title,data::Book& book);
	bool GetBookByIsbn(std::string isbn, data::Book& book);
	bool GetBookByIsbn13(std::string isbn13, data::Book& book);
	bool GetAllBooksByAuthors(std::string authors, std::vector<data::Book>& books);
};

