#pragma once

#include <QMainwindow>
#include "ui_LoginWindow.h"
#include"MockBooksAndUsers.h"
#include"MainWindow.h"

#include "../OnLib.DataTransfer/User.h"
#include <vector>
#include "Errors.h"

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
	void LoginButtonClicked(data::User user);

public slots:
	void OnLoginFailure(const std::vector<data::LogginErrors>& errors);
	void OnLoginSucces();
	void OnRegisterSuccess();
	void OnRegisterFailure(const std::vector<data::RegisterErrors>& errors);

private:
	Ui::LoginWindow* ui;
	MainWindow* mainWindow;
	MockBooksAndUsers mock;
};