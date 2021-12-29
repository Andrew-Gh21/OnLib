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
		GetAuthors(book);
		GetCategories(book);
		GetRating(book);
	}

	return books;
}

std::vector<data::LendBook> BooksManager::GetLendedBooks(uint64_t userId)
{
	std::vector<data::LendBook>lendedBooks;
	constexpr static const char* query =
		"select user_id, book_id, lend_date, return_date, b.title, b.cover_url "
		"from user_book "
		"inner join book b on b.id = book_id "
		"where user_id = ? ";

	auto output = [&lendedBooks](uint64_t userId, uint64_t bookId, std::string lendDate, std::optional<std::string> returnDate, std::string title, std::string coverUrl)
	{
		lendedBooks.push_back(data::LendBook(bookId, lendDate, returnDate ? *returnDate : "", title, coverUrl));
	};

	database << query << userId
		>> output;

	for (data::LendBook& book : lendedBooks)
	{
		GetAuthors(book);

		if (checkIfAvailable(book.lendDate))
		{
			book.isAvailable = true;
		}
	}

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
		"select a.name from author as a "
		"inner join book_author ba on a.id = ba.author_id "
		"where ba.book_id = ?";

	auto output = [&book](std::string name)
	{
		book.authors.push_back(name);
	};

	database << query << book.id
		>> output;
}

void BooksManager::GetAuthors(data::LendBook& book)
{
	constexpr static const char* query =
		"select a.name from author as a "
		"inner join book_author ba on a.id = ba.author_id "
		"where ba.book_id = ?";

	auto output = [&book](std::string name)
	{
		book.authors.push_back(name);
	};

	database << query << book.bookId
		>> output;
}

void BooksManager::GetCategories(data::Book& book)
{
	constexpr static const char* query =
		"select category_id from book_category "
		"where book_id = ? AND category_id NOT IN (?)";

	auto output = [&book](uint64_t categoryId)
	{
		book.otherCategories.push_back(static_cast<data::BookCategory>(categoryId - 1));
	};

	database << query << book.id << static_cast<uint64_t>(book.mainCategory)
		>> output;
}

void BooksManager::GetRating(data::Book& book)
{
	constexpr static const char* query =
		"select AVG(rating) from book_rating "
		"where book_id = ?";

	auto output = [&book](float rating)
	{
		book.rating = rating;
	};

	database << query << book.id
		>> output;
}

bool BooksManager::checkIfAvailable(const std::string& date)
{
	std::time_t currentTime = std::time(0);
	std::time_t rawtime;
	std::tm* timestamp = new std::tm;
	std::tm* time1 = new std::tm();
	localtime_s(timestamp, &currentTime);

	std::string day1(date.begin(), date.begin() + 2);
	std::string luna1(date.begin() + 3, date.begin() + 5);
	std::string an1(date.begin() + 6, date.end());
	time1->tm_year = stoi(an1) + 100;
	time1->tm_mon = stoi(luna1) - 1;
	time1->tm_mday = stoi(day1);
	rawtime = mktime(time1);

	if (difftime(currentTime, rawtime) / (60 * 60 * 24) <= 14)
		return true;

	return false;
}

