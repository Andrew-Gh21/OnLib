#include "MyListBookPreview.h"
#include <sstream>

MyListBookPreview::MyListBookPreview(const data::LendBook& _book, QWidget* parent)
	: QWidget(parent), book(_book)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));
	
	ui.returnDate->setText(QString::fromStdString(book.isAvailable ?
		"Borrowed at:" + book.lendDate :
		"Borrowed at:" + book.lendDate + " you didn't returned it on time"
	));

	this->setMinimumSize(this->sizeHint());
	this->adjustSize();

	connect(ui.returnBookButton, &QAbstractButton::pressed, [this]() {emit ReturnButtonPressed(book.bookId); });
}

void MyListBookPreview::BookCoverRecieved(QByteArray data)
{
	QPixmap cover;
	cover.loadFromData(data);
	ui.cover->setPixmap(cover.scaled(ui.cover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

MyListBookPreview::~MyListBookPreview()
{
}
