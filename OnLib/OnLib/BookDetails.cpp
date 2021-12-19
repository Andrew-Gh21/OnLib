#include "BookDetails.h"
#include <sstream>

BookDetails::BookDetails(const data::Book& book, QWidget* parent)
	: QWidget(parent)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));
	ui.lendButton->setText("Borrow book");

	this->setMinimumSize(this->sizeHint());
	this->adjustSize();
}

BookDetails::~BookDetails()
{
}

void BookDetails::BookCoverRecieved(QByteArray data)
{
	QPixmap cover;
	cover.loadFromData(data);
	ui.cover->setPixmap(cover.scaled(ui.cover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}