#include "BookPreview.h"
#include <sstream>

BookPreview::BookPreview(const data::Book& book, QWidget *parent)
	: QWidget(parent)
{
	std::ostringstream authorsStream;
	std::copy(std::cbegin(book.authors), std::cend(book.authors), std::ostream_iterator<std::string>(authorsStream));

	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(authorsStream.str()));
}

BookPreview::~BookPreview()
{
}

void BookPreview::BookCoverRecieved(QByteArray data)
{
	QPixmap cover;
	cover.loadFromData(data);
	ui.cover->setPixmap(cover);
}
