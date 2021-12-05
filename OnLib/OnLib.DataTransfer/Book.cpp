#include "Book.h"

data::Book::Book()
    :id(), isbn(), title(), coverUrl(), mainCategory(), otherCategories(), authors(), rating()
{
	InitCategories();
}

data::Book::Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, const std::vector<BookCategory>& otherCategories, const std::vector<std::string>& authors, float rating)
    :id(id), isbn(isbn), title(title), coverUrl(coverUrl), mainCategory(mainCategory), otherCategories(otherCategories), authors(authors), rating(rating)
{
	InitCategories();
}

data::Book::Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, float rating)
    : id(id), isbn(isbn), title(title), coverUrl(coverUrl), mainCategory(mainCategory), rating(rating)
{
	InitCategories();
}

void data::Book::InitCategories()
{
	bookCategory[BookCategory::SF] = "SF";
	bookCategory[BookCategory::Romantic] = "Romantic";
	bookCategory[BookCategory::Action] = "Action";
	bookCategory[BookCategory::Comedy] = "Comedy";
}

