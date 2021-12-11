#include "BookSection.h"

BookSection::BookSection(const QString& section, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.sectionLabel->setText(section);
	widget = new QWidget();
	layout = new QGridLayout(widget);
	widget->setLayout(layout);
	ui.sectionContent->setWidget(widget);

}

BookSection::~BookSection()
{
}

void BookSection::AddBook(BookPreview* book)
{
	layout->addWidget(book,0,layout->columnCount()+1);
}
