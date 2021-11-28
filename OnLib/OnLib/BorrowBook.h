#pragma once

#include "Book.h"
#include <ctime>

class BorrowBook
{
private:
	Book book;
	std::tm borrowDate;
	std::tm returnDate;

public:
	BorrowBook(Book book);
	
	Book GetBook()const;
	std::tm GetBorrowDate() const;
	std::tm GetReturnDate() const;
	

	void ExtendReturnDate();

	void SetBook(const Book &book);

	~BorrowBook() = default;
};