#include "Book.h"

data::Book::Book()
    :id(), isbn(), title(), coverUrl(), mainCategory(), otherCategories(), authors(), rating()
{
}

data::Book::Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, const std::vector<BookCategory>& otherCategories, const std::vector<std::string>& authors, float rating)
    :id(id), isbn(isbn), title(title), coverUrl(coverUrl), mainCategory(mainCategory), otherCategories(otherCategories), authors(authors), rating(rating)
{
}

data::Book::Book(uint64_t id, const std::string& isbn, const std::string& title, const std::string& coverUrl, BookCategory mainCategory, float rating)
    : id(id), isbn(isbn), title(title), coverUrl(coverUrl), mainCategory(mainCategory), rating(rating)
{
}

