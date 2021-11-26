#include "BorrowBook.h"

BorrowBook::BorrowBook(Book book) : book(book)
{
	//time_t now = time(0);
	//borrowDate = gmtime(&now);
	//char* dt = asctime(borrowDate);
	//returnDate = borrowDate;
	//returnDate->tm_mday += 14;
}

Book BorrowBook::GetBook() const
{
	return book;
}

tm* BorrowBook::GetBorrowDate() const
{
	return borrowDate;
}

tm* BorrowBook::GetReturnDate() const
{
	return returnDate;
}

void BorrowBook::SetBook(const Book &book)
{
	this->book = book;
}

void BorrowBook::SetReturnDate(tm* time)
{
	returnDate = time;
}