#include "BookDetails.h"
#include <sstream>

BookDetails::BookDetails(const data::Book& book, QWidget* parent)
	: QWidget(parent)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.description->setText(QString::fromStdString(book.description));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));
	ui.lendButton->setText("Borrow book");

	this->setMinimumSize(this->sizeHint());
	this->adjustSize();
}

BookDetails::~BookDetails()
{
}

