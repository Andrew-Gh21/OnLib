#pragma once

#include <QWidget>
#include "ui_BookSection.h"
#include "BookPreview.h"
#include "MyListBookPreview.h"

class BookSection : public QWidget
{
	Q_OBJECT

public:
	BookSection(const QString& section, QWidget *parent = Q_NULLPTR);
	~BookSection() = default;

	void AddBook(BookPreview* book);
	void AddBook(MyListBookPreview* book);

private:
	Ui::BookSection ui;
	QWidget* widget;
	QGridLayout* layout;
};
