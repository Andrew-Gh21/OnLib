#pragma once

#include "Book.h"
#include <time.h>

class BorrowBook
{
private:
	Book book;
	tm* borrowDate;
	tm* returnDate;

public:
	BorrowBook(Book book);
	
	Book GetBook()const;
	tm* GetBorrowDate()const;
	tm* GetReturnDate()const;

	void SetBook(const Book &book);
	void SetReturnDate(tm* time);

	~BorrowBook() = default;
};