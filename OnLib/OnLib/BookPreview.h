#pragma once

#include <QWidget>
#include "ui_BookPreview.h"

#include "../OnLib.DataTransfer/Book.h"

class BookPreview : public QWidget
{
	Q_OBJECT

public:
	BookPreview(const data::Book& book, QWidget *parent = Q_NULLPTR);
	~BookPreview() = default;

	void BookCoverRecieved(QByteArray data);

private:
	Ui::BookPreview ui;
	data::Book book;
signals:
	void BorrowPressed(uint64_t id);
	void BookDetailsPressed(const data::Book book);
	void BookReviewPressed(QRadioButton* rating,uint64_t id);
};
