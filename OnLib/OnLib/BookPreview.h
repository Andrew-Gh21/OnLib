#pragma once

#include <QWidget>
#include "ui_BookPreview.h"

#include "../OnLib.DataTransfer/Book.h"

class BookPreview : public QWidget
{
	Q_OBJECT

public:
	BookPreview(const data::Book& book, QWidget *parent = Q_NULLPTR);
	~BookPreview();

private:
	Ui::BookPreview ui;
};
