#pragma once

#include <QWidget>
#include "ui_LoginWindow.h"

class LoginWindow : public QWidget
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = Q_NULLPTR);
	void HandleLoginButton();
	void HandleSignUpButton();
	void HandleBackToLoginButton();
	~LoginWindow();

private:
	Ui::LoginWindow ui;
	QWidget* window;
};
