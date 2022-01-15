#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include<unordered_map>
#include "../OnLib.DataTransfer/BookCategory.h"
#include "BookSection.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = Q_NULLPTR);
	BookSection* myListBookSection;
	~MainWindow() = default;
	void SeeBookDetails(const data::Book &book, QPixmap cover);
	void SeeBookText(const data::LendBook& book);
public slots:
	void AddBooksToSection(const std::vector<data::Book>& books);
	void AddBorrowedBooks(const std::vector<data::LendBook>& books);
	void AddSearchedBooks(const std::vector<data::Book>& books);
signals:
	void BorrowBookRequest(uint64_t id);
	void ReturnBookRequest(uint64_t bookId);
	void LogOutRequest();
	void DeleteAccountRequest(const std::string& password);
	void SearchRequest(const std::string& text);
	void RefreshButtonPressed();
	void BookRated(int rating, uint64_t bookId);

private slots:
	void HandleSearchIconButton();
	void HandleHomeButton();
	void HandleMyListButton();
	void HandleSearchBooksButton(std::string s);
	void HandleRefreshButton();
	void StyleSheets();

	
private:
	std::unordered_map<data::BookCategory, BookSection*> categories;
	std::vector<QWidget*> visibleBooks;
	std::vector<QWidget*> searchBooks;
	Ui::MainWindow* ui;
	QAction* searchButtonWidgetAction;
	QAction* searchLineEditWidgetAction;
	QLineEdit* searchLineEdit;

};
