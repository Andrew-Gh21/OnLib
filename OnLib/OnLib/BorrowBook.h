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
	
	Book GetBook();
	tm* GetBorrowDate();
	tm* GetReturnDate();

	void SetBook(Book book);
	void SetReturnDate(tm* time);

	~BorrowBook() = default;
};