#include "BorrowBook.h"


BorrowBook::BorrowBook(Book book):book(book)
{
	//time_t now = time(0);
	//borrowDate = gmtime(&now);
	//char* dt = asctime(borrowDate);
	//returnDate = borrowDate;
	//returnDate->tm_mday += 14;
}


void BorrowBook::SetBook(Book book)
{
	this->book = book;
}

void BorrowBook::SetReturnDate(tm* time)
{
	returnDate = time;
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
