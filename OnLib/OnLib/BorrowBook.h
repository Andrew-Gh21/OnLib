#pragma once
#include "Book.h"
#include<time.h>
class BorrowBook
{
private:
	Book book;
	tm* borrowDate;
	tm* returnDate;
public:
	BorrowBook(Book);
public:
	void SetBook(Book book); 
	void SetReturnDate(tm* time);
	Book GetBook();
	tm* GetBorrowDate();
	tm* GetReturnDate();
	
};

