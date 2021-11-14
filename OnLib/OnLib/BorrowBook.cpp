#include "BorrowBook.h"

BorrowBook::BorrowBook(User user, Book book):book(book),user(user)
{
	//time_t now = time(0);
	//borrowDate = gmtime(&now);
	//char* dt = asctime(borrowDate);
	//returnDate = borrowDate;
	//returnDate->tm_mday += 14;
}

void BorrowBook::SetUser(User user)
{
	this->user = user;
}

void BorrowBook::SetBook(Book book)
{
	this->book = book;
}

void BorrowBook::SetReturnDate(tm* time)
{
	returnDate = time;
}

User BorrowBook::GetUser()
{
	return user;
}

Book BorrowBook::GetBook()
{
	return book;
}

tm* BorrowBook::GetBorrowDate()
{
	return borrowDate;
}

tm* BorrowBook::GetReturnDate()
{
	return returnDate;
}
