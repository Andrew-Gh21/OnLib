#pragma once

#include <QMainwindow>
#include "ui_LoginWindow.h"
#include"MockBooksAndUsers.h"
#include"MainWindow.h"

#include "../OnLib.DataTransfer/User.h"

namespace Ui {
	class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

signals:
	void LoginButtonClicked(data::User user);

public:
	explicit LoginWindow(QWidget* parent = Q_NULLPTR);
	void HandleLoginButton();
	void HandleSignUpButton();
	void HandleRegisterButton();
	void HandleBackToLoginButton();
	~LoginWindow();

private:
	Ui::LoginWindow* ui;
	MainWindow* mainWindow;
	MockBooksAndUsers mock;
};