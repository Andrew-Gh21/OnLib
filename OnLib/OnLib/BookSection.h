#pragma once

#include <QWidget>
#include "ui_BookSection.h"
#include "BookPreview.h"

class BookSection : public QWidget
{
	Q_OBJECT

public:
	BookSection(const QString& section, QWidget *parent = Q_NULLPTR);
	~BookSection();

	void AddBook(BookPreview* book);

private:
	Ui::BookSection ui;
	QHBoxLayout* layout;
};
