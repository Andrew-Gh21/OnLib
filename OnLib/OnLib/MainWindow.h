#pragma once

#include <QWidget>
#include "ui_MainWindow.h"
#include <QMainWindow>
#include<string>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private slots:
	void loginButtonClicked(std::string val);

private:
	Ui::MainWindow* ui;
	std::string username;
};
