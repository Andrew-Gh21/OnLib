#pragma once
#include "Book.h"
#include "User.h"
#include<time.h>
class BorrowBook
{
private:
	User user;
	Book book;
	tm* borrowDate;
	tm* returnDate;
public:
	BorrowBook(User, Book);
public:
	void SetUser(User user); 
	void SetBook(Book book); 
	void SetReturnDate(tm* time);
	User GetUser();
	Book GetBook();
	tm* GetBorrowDate();
	tm* GetReturnDate();
	
};

