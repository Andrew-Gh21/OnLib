#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "User.h"
#include"MockBooksAndUsers.h"

namespace Ui {
	class LoginWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
private slots:
	void LoginButtonClicked(User user);
	void HandleLogOutButton();
	void HandleDeleteAccountButton();
	void HandleSearchIconButton();


private:
	void AddBooksToScrollArea();
	Ui::MainWindow *ui;
	MockBooksAndUsers mock;
	User user;
};
