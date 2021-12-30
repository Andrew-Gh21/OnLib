#include "MyListBookPreview.h"
#include <sstream>

MyListBookPreview::MyListBookPreview(const data::LendBook& book, QWidget* parent)
	: QWidget(parent)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));

	this->setMinimumSize(this->sizeHint());
	this->adjustSize();
}

MyListBookPreview::~MyListBookPreview()
{
}

void MyListBookPreview::BookCoverRecieved(QByteArray data)
{
	QPixmap cover;
	cover.loadFromData(data);
	ui.cover->setPixmap(cover.scaled(ui.cover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
