#pragma once
#include <QWidget>
#include "ui_BookDetails.h"

#include "../OnLib.DataTransfer/Book.h"
class BookDetails : public QWidget
{
	Q_OBJECT

public:
	BookDetails(const data::Book& book, QWidget* parent = Q_NULLPTR);
	~BookDetails();
	Ui::BookDetails ui;


};

