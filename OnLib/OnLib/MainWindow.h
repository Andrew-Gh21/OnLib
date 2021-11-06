#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	void HandleSearchGenreButton();
	~MainWindow();

private:
	Ui::MainWindow ui;
};