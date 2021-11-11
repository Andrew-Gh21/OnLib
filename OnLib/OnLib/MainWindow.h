#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "User.h"

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

private:
	Ui::MainWindow *ui;
	User user;
};
