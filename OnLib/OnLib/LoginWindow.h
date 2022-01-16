#pragma once

#include <QMainwindow>
#include "ui_LoginWindow.h"
#include"MainWindow.h"

#include "../OnLib.DataTransfer/User.h"
#include <vector>
#include "Enums.h"

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
	void RegisterButtonClicked(data::User user);

public slots:
	void OnLoginFailure(const std::vector<data::LoginErrors>& errors);
	void OnRegisterSuccess();
	void OnRegisterFailure(const std::vector<data::RegisterErrors>& errors);

private:
	Ui::LoginWindow* ui;
	static std::unordered_map<data::LoginErrors, std::string> loginErrors;
	static std::unordered_map<data::RegisterErrors, std::string> registerErrors;
};