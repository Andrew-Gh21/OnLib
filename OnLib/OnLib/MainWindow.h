#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
private slots:
	//void HandleAddToMyListButton(Book book);
	//void LoginButtonClicked(User user);
	void HandleLogOutButton();
	void HandleDeleteAccountButton();
	void HandleSearchIconButton();
	void HandleBackToMenuButton();
	void HandleSearchBooksButton(std::string s);
	//void HandleReturnBookButton(Book book);
	//void HandleExtendDateButton(BorrowBook book);

private:
	void AddBooksToScrollArea();
	void AddBooksToMyList();
	Ui::MainWindow* ui;
};
