#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->helloLineEdit->setText(QString::fromStdString(username));
}

MainWindow::~MainWindow()
{
}

void MainWindow::loginButtonClicked(std::string val)
{
	this->username = val;
	ui->helloLineEdit->setText(QString::fromStdString("Hello "+ username));
	
}
