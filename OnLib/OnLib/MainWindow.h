#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

namespace Ui {
	class LoginWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	Ui::MainWindow *ui;
};
