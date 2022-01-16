#include "BooksManager.h"

#include <sstream>

std::vector<data::Book> BooksManager::GetNewestFiveBooksFromEachCategory()
{
	constexpr static const char* query =
		"SELECT b.id, b.isbn, b.title, b.description, b.cover_url, b.added_date, c.id as category_id FROM book as b "
		"INNER JOIN category c ON b.main_category_id = c.id "
		"WHERE b.id in( "
		"	SELECT b1.id FROM book as b1 "
		"	WHERE b.main_category_id = b1.main_category_id "
		"	ORDER BY added_date LIMIT 0, 5)";

	std::vector<data::Book> books;

	auto output = [&books](uint64_t id, std::string isbn, std::string title,
		std::string description, std::string coverUrl, std::string addedDate, uint64_t categoryId)
	{
		data::Book book(id, isbn, title, description, coverUrl, static_cast<data::BookCategory>(categoryId), 0);
		books.push_back(std::move(book));
	};

	database << query
		>> output;

	for (auto& book : books)
	{
		GetAuthors(book.id, book.authors);
		GetCategories(book);
		GetRating(book);
	}

	return books;
}

std::vector<data::LendBook> BooksManager::GetLendedBooks(uint64_t userId)
{
	constexpr static double parserToDays = 60.0 * 60.0 * 24.0;

	constexpr static const char* query =
		"select user_id, book_id, lend_date, return_date, b.title, b.description, b.cover_url "
		"from user_book "
		"inner join book b on b.id = book_id "
		"where user_id = ? and return_date is null";

	std::vector<data::LendBook>lendedBooks;

	auto output = [&lendedBooks](uint64_t userId, uint64_t bookId, std::string lendDate, /*std::string limitDate,*/ std::optional<std::string> returnDate, std::string title, std::string description, std::string coverUrl)
	{
		lendedBooks.push_back(data::LendBook(bookId, lendDate, /*limitDate,*/ returnDate ? *returnDate : "", title, description, coverUrl));
	};

	database << query
		<< userId
		>> output;

	for (data::LendBook& book : lendedBooks)
	{
		GetAuthors(book.bookId, book.authors);
		book.isAvailable = CheckIfAvailable(book.lendDate);
	}

	return lendedBooks;
}

void BooksManager::AddLendedBookToUser(uint64_t bookId, uint64_t userId)
{
	constexpr static const char* queryInsertBook =
		"insert into user_book (user_id, book_id, lend_date) "
		"values (?,?,?) ";

	if (!CanLendBook(bookId, userId))
		return;

	std::string time = LogMessage::GetTime();

	database << queryInsertBook
		<< userId << bookId << time /*<< AddFourteenDays(time)*/;
}

bool BooksManager::CanLendBook(uint64_t bookId, uint64_t userId)
{
	constexpr static const char* checkIfBorrowed =
		"select count(*) from user_book "
		"where user_id = ? and book_id = ? and return_date is null ";

	constexpr static int MAX_BOOKS_PER_USER = 5;

	bool alreadyBorrowed = false;

	database << checkIfBorrowed
		<< userId << bookId
		>> alreadyBorrowed;

	if (alreadyBorrowed)
		return false;

	const std::vector<data::LendBook> userBorrowedBooks = GetLendedBooks(userId);

	if (userBorrowedBooks.size() > MAX_BOOKS_PER_USER)
		return false;

	bool hasLateBooks = std::any_of(std::cbegin(userBorrowedBooks), std::cend(userBorrowedBooks), [](const data::LendBook& book) {
		return book.isAvailable == false;
		});

	return !hasLateBooks;
}

void BooksManager::GetAuthors(uint64_t bookId, std::vector<std::string>& authors)
{
	constexpr static const char* query =
		"select a.name from author as a "
		"inner join book_author ba on a.id = ba.author_id "
		"where ba.book_id = ?";

	auto output = [&authors](std::string name)
	{
		authors.push_back(name);
	};

	database << query
		<< bookId
		>> output;
}

void BooksManager::GetCategories(data::Book& book)
{
	constexpr static const char* query =
		"select category_id from book_category "
		"where book_id = ? AND category_id NOT IN (?)";

	auto output = [&book](uint64_t categoryId)
	{
		book.otherCategories.push_back(static_cast<data::BookCategory>(categoryId));
	};

	database << query
		<< book.id << data::EnumToNumber(book.mainCategory)
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

	database << query
		<< book.id
		>> output;
}

void BooksManager::ReturnBook(uint64_t bookId, uint64_t userId)
{
	constexpr static const char* query =
		"update user_book "
		"set return_date = ? "
		"where user_id = ? and book_id = ? ";

	database << query
		<< LogMessage::GetTime() << userId << bookId;
}

bool BooksManager::CheckIfAvailable(const std::string& date)
{
	constexpr static double parserToDays = 60.0 * 60.0 * 24.0;

	std::time_t currentTime = std::time(0);
	std::time_t rawtime = std::time(0);
	std::tm* timestamp = new std::tm;
	std::tm* time = new std::tm();
	localtime_s(timestamp, &currentTime);

	std::string day(date.begin(), date.begin() + 2);
	std::string month(date.begin() + 3, date.begin() + 5);
	std::string year(date.begin() + 6, date.end());
	time->tm_year = std::stoi(year) + 100;
	time->tm_mon = std::stoi(month) - 1;
	time->tm_mday = std::stoi(day);

	if (std::difftime(currentTime, mktime(time)) / (parserToDays) <= 14)
		return true;

	return false;
}

std::string BooksManager::AddFourteenDays(const std::string& date)
{
	constexpr static double fourteenDays = 14.0 * 24.0 * 60.0 * 60.0;
	std::tm* time = new std::tm();
	std::string day(date.begin(), date.begin() + 2);
	std::string month(date.begin() + 3, date.begin() + 5);
	std::string year(date.begin() + 6, date.end());
	time->tm_year = std::stoi(year) + 100;
	time->tm_mon = std::stoi(month) - 1;
	time->tm_mday = std::stoi(day);

	std::tm* finalTime{};
	time_t t = mktime(time) + fourteenDays;
	localtime_s(finalTime, &t);

	char buffer[40];
	strftime(buffer, 40, "%d/%m/%g %T", finalTime);
	return std::string(buffer);
}

void BooksManager::Rate(uint64_t bookId, uint64_t userId, int rating)
{
	constexpr static const char* select = "select count(*) from book_rating where book_id = ? and user_id = ? ";
	constexpr static const char* insert = "insert into book_rating(book_id, user_id, rating) values (?, ?, ?) ";
	constexpr static const char* update = "update book_rating set rating = ? where book_id = ? and user_id = ? ";

	bool rated;

	database << select
		<< bookId << userId
		>> rated;

	if (rated)
	{
		database << update
			<< rating << bookId << userId;
		return;
	}

	database << insert
		<< bookId << userId << rating;
}

bool BooksManager::SetupSearchExtension(std::string& errorMsg)
{
	if (sqlite3_enable_load_extension(database.connection().get(), 1) != SQLITE_OK)
	{
		errorMsg = "Error in enabling extension loading";
		return false;
	}

	if (sqlite3_load_extension(database.connection().get(), "spellfix.dll", "sqlite3_spellfix_init", NULL) != SQLITE_OK)
	{
		errorMsg = "Error in loading extension";
		return false;
	}

	if (sqlite3_enable_load_extension(database.connection().get(), 0) != SQLITE_OK)
	{
		errorMsg = "Error in disabling extension loading";
		return false;
	}

	database << "CREATE VIRTUAL TABLE IF NOT EXISTS demo USING spellfix1;";
	database << "insert into demo(word, rank) select title, id from book;";

	return true;
}

void BooksManager::ExtendDate(uint64_t bookId, uint64_t userId)
{
	constexpr static const char* getDate =
		"select id , limit_date from user_book "
		"where user_id = ? and book_id = ? and return_date is null ";

	constexpr static const char* updateDate =
		"update user_book "
		"set limit_date = ? "
		"where id = ? ";

	uint64_t id;
	std::string date;

	auto output = [&id, &date](uint64_t idAux, std::string dateAux)
	{
		id = idAux;
		date = dateAux;
	};

	database << getDate
		<< userId << bookId
		>> output;


	database << updateDate
		<< date << id;
}

std::vector<data::Book> BooksManager::Search(const std::string& keyword)
{
	std::stringstream stream;
	std::vector<data::Book> books;

	auto output = [&books](uint64_t id, std::string isbn, std::string title,
		std::string description, std::string coverUrl, std::string addedDate, uint64_t categoryId)
	{
		data::Book book(id, isbn, title, description, coverUrl, static_cast<data::BookCategory>(categoryId), 0);
		books.push_back(std::move(book));
	};

	stream << "SELECT b.id, b.isbn, b.title, b.description, b.cover_url, b.added_date, c.id AS category_id FROM book AS b"
		" INNER JOIN category AS c ON b.main_category_id = c.id"
		" WHERE b.id IN (SELECT d.rank FROM demo AS d WHERE editdist3(d.word, '" << keyword << "') / 100 < 10)";

	database << stream.str() >> output;

	for (auto& book : books)
	{
		GetAuthors(book.id, book.authors);
		GetCategories(book);
		GetRating(book);
	}

	return books;
}