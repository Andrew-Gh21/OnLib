#pragma once
#include <QtWidgets>
#include <QWidget>
#include <QPushButton>
#include <QFormLayout>

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

public:
	LoginWindow(QWidget* parent = nullptr);
};