#include "BookPreview.h"

BookPreview::BookPreview(const data::Book& book, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.originalTitle));
	ui.authors->setText(QString::fromStdString(book.authors));
}

BookPreview::~BookPreview()
{
}
