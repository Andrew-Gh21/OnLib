#include "BookSection.h"

BookSection::BookSection(const QString& section, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.sectionLabel->setText(section);
}

BookSection::~BookSection()
{
}

void BookSection::AddBook(BookPreview* book)
{
	ui.sectionContent->layout()->addWidget(book);
}
