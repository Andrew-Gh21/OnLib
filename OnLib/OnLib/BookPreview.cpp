#include "BookPreview.h"

BookPreview::BookPreview(const data::Book& book, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.title->setText(QString::fromStdString(book.title));
	ui.authors->setText(QString::fromStdString(book.authors[0]));
}

BookPreview::~BookPreview()
{
}
