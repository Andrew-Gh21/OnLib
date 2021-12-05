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
	std::optional<std::vector<data::Book>> optionalTopFiveBooks;
	database << "select b.id, b.isbn, b.title, b.cover_url, b.added_date, c.name, r.rating from book b inner join category c on c.id = b.main_category_id  inner join book_rating r on r.book_id = b.id group by c.name order by added_date desc limit 5";
	//   select b.id, b.isbn, b.title, b.cover_url, b.added_date, c.name, r.rating from book b
	//   inner join category c on c.id = b.main_category_id
	//   inner join book_rating r on r.book_id = b.id
	//	 group by c.name
	//	 order by added_date desc
	//	 limit 5
	[&optionalTopFiveBooks](uint64_t id,std::string isbn,std::string title,std::string coverUrl,data::BookCategory mainCategory,float rating)
	{
		optionalTopFiveBooks->push_back(data::Book(id,isbn,title,coverUrl,mainCategory,rating));
	};

	if (optionalTopFiveBooks.has_value())
	{
		GetAuthorsAndOtherCategories(*optionalTopFiveBooks);
	}

	return *optionalTopFiveBooks;
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
		<< userId << bookId << new std::string() << new std::string();
}
