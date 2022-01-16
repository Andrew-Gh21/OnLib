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

signals:
	void BorrowPressed(uint64_t id);
	void BookReviewPressed(int rating, uint64_t id);

public:
	Ui::BookPreview ui;

private:
	data::Book book;
};
