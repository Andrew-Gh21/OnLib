#pragma once
#include <QWidget>
#include "ui_MyListBookPreview.h"

#include "../OnLib.DataTransfer/Book.h"

class MyListBookPreview : public QWidget
{
	Q_OBJECT

public:
	MyListBookPreview(const data::LendBook& book, QWidget* parent = Q_NULLPTR);
	~MyListBookPreview();	
	Ui::MyListBookPreview ui;


	void BookCoverRecieved(QByteArray data);

signals:
	void ReturnButtonPressed(uint64_t bookId);

private:
	data::LendBook book;
};

