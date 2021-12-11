#include "BooksManager.h"

void BooksManager::GetAuthorsAndOtherCategories(const std::vector<data::Book>& books)
{	
	for (data::Book book : books)
	{
		std::optional<std::vector<std::string>> optionalAuthors;
		std::optional<std::vector<data::BookCategory>> optionalCategories;
		database << "select a.name from author a inner join book_author ba on ba.book_id = ? where a.id= ba.author_id"
				 << book.id;
		//	select a.name from author a
		//	inner join book_author ba on ba.book_id = ?
		//	where a.id= ba.author_id
		[&optionalAuthors](std::string author)
		{
			optionalAuthors->push_back(author);
		};
		if (optionalAuthors.has_value())
		{
			book.authors = *optionalAuthors;
		}

		database << "select c.name from category c inner join book_category bc on bc.book_id = ? where c.id= bc.category_id and c.name not in (?)"
			<< book.id
			<< data::ParseCategoryToString(book);
		//	select c.name from category c
		//	inner join book_category bc on bc.book_id = ?
		//	where c.id= bc.category_id and c.name not in (?)
		[&optionalCategories](std::string category)
		{
			optionalCategories->push_back(data::ParseCategoryType(category));
		};
		if (optionalCategories.has_value())
		{
			book.otherCategories = *optionalCategories;
		}
	}
}

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
	std::optional<std::vector<data::LendBook>> optionalLendedBooks;
	database << "select * from user_book where user_id=?" << userId;
	[&optionalLendedBooks](uint64_t userId , uint64_t bookId, std::string lendDate, std::string returnDate)
	{
		optionalLendedBooks->push_back(data::LendBook(bookId, lendDate, returnDate));
	};
	if (optionalLendedBooks.has_value())
	{
		return *optionalLendedBooks;
	}
}

void BooksManager::AddLendedBookToUser(uint64_t bookId, uint64_t userId)
{
	
	database << "insert into user_book values (?,?,?,?)"
		<< userId << bookId << std::string() << std::string();
}
