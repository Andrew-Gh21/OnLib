#pragma once
#include <QtWidgets>
#include <QWidget>
#include <QPushButton>
#include <QFormLayout>
#include "User.h"

class QLineEdit;

class LoginWindow : public QWidget
{
	Q_OBJECT

private:
	QLineEdit* username;
	QLineEdit* password;
	QLineEdit* confirmPassword;
	QPushButton* loginButton;
	QPushButton* signupButton;
	QFormLayout* mainLayout;

public:
	LoginWindow(QWidget* parent = nullptr);
	void HandleLoginButton();
	void HandleSignUpButton();
};