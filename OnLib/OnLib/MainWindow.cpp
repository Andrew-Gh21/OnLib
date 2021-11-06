#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) //, ui(new Ui::MainWindow)
{
	ui.setupUi(this);

	MainWindow* mainWindow = new MainWindow;

	//connect(ui->searchGenreButton, &QPushButton::released, this, &MainWindow::HandleSearchGenreButton);
}

void MainWindow::HandleSearchGenreButton()
{
    
}

MainWindow::~MainWindow()
{
}
