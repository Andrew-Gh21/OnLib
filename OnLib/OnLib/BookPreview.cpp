#include "BookPreview.h"
#include <sstream>

BookPreview::BookPreview(const data::Book& book, QWidget *parent)
	: QWidget(parent),book(book)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));
	ui.lendButton->setText("Borrow book");

	connect(ui.lendButton, &QPushButton::pressed, [this,book]() {emit BorrowPressed(book.id); });
	connect(ui.detailsButton, &QPushButton::pressed, [this, book]() {emit BookDetailsPressed(book); });

	this->setMinimumSize(this->sizeHint());
	this->adjustSize();
}

void BookPreview::BookCoverRecieved(QByteArray data)
{
	QPixmap cover;
	cover.loadFromData(data);
	ui.cover->setPixmap(cover.scaled(ui.cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

