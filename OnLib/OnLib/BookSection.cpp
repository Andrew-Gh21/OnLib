#include "BookSection.h"

BookSection::BookSection(const QString& section, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.sectionLabel->setText(section);
	layout = new QHBoxLayout;
	ui.sectionContent->setLayout(layout);
}

BookSection::~BookSection()
{
}

void BookSection::AddBook(BookPreview* book)
{
	layout->addWidget(book);
}
