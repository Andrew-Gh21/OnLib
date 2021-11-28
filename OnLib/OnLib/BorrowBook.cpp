#include "BorrowBook.h"

BorrowBook::BorrowBook(Book book) : book(book)
{
	time_t now = time(0);
	localtime_s(&borrowDate, &now);
	localtime_s(&returnDate, &now);
	returnDate.tm_mday += 15;
	mktime(&returnDate);

	


}

Book BorrowBook::GetBook() const
{
	return book;
}

std::tm BorrowBook::GetBorrowDate() const
{
	return borrowDate;
}

std::tm BorrowBook::GetReturnDate() const
{
	return returnDate;
}



void BorrowBook::ExtendReturnDate()
{
	returnDate.tm_mday += 15;
	mktime(&returnDate);
}

void BorrowBook::SetBook(const Book &book)
{
	this->book = book;
}

