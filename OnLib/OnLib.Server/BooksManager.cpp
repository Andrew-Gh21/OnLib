#include "BooksManager.h"


std::vector<data::Book> BooksManager::GetNewestFiveBooksFromEachCategory()
{
	constexpr static const char* query =
		"SELECT b.id, b.isbn, b.title, b.cover_url, b.added_date, c.id as category_id FROM book as b "
		"INNER JOIN category c ON b.main_category_id = c.id "
		"WHERE b.id in( "
		"	SELECT b1.id FROM book as b1 "
		"	WHERE b.main_category_id = b1.main_category_id "
		"	ORDER BY added_date LIMIT 0, 5)";

	std::vector<data::Book> books;

	auto output = [&books](uint64_t id, std::string isbn, std::string title,
		std::string coverUrl, std::string addedDate, uint64_t categoryId)
	{
		data::Book book(id, isbn, title, coverUrl, static_cast<data::BookCategory>(categoryId - 1), 0);
		books.push_back(std::move(book));
	};

	database << query
		>> output;

	for (auto& book : books)
	{
		// Fill authors, categories, rating
	}

	return books;
}

std::vector<data::LendBook> BooksManager::GetLendedBooks(uint64_t userId)
{
	std::vector<data::LendBook>lendedBooks;
	constexpr static const char* query = "select * from user_book where user_id=?";

	auto output = [&lendedBooks](uint64_t userId, uint64_t bookId, std::string lendDate, std::optional<std::string> returnDate)
	{
		lendedBooks.push_back(data::LendBook(bookId, lendDate, returnDate ? *returnDate : ""));
	};

	database << query << userId
		>> output;

	return lendedBooks;
}

void BooksManager::AddLendedBookToUser(uint64_t bookId, uint64_t userId)
{
	constexpr static const char* query = "insert into user_book values (?,?,?,?)";
	database << query << userId << bookId << std::string() << std::string();
}

void BooksManager::GetAuthors(data::Book& book)
{
	constexpr static const char* query =
		"select a.name from author "
		"inner join book_author ba on a.id=ba.author_id "
		"where ba.book_id = ?";

	auto output = [&book](std::string name)
	{
		book.authors.push_back(name);
	};

	database << query << book.id
		>> output;
}