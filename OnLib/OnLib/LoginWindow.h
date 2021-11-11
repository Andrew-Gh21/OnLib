#pragma once

#include <QMainwindow>
#include "ui_LoginWindow.h"
#include"MockBooksAndUsers.h"
#include"MainWindow.h"
#include"User.h"

namespace Ui {
	class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LoginWindow(QWidget* parent = Q_NULLPTR);
	void HandleLoginButton();
	void HandleSignUpButton();
	void HandleRegisterButton();
	void HandleBackToLoginButton();
	~LoginWindow();

signals:
	void LoginButtonClicked(User user);

private:
	Ui::LoginWindow* ui;
	MainWindow* mainWindow;
	MockBooksAndUsers mock;
};