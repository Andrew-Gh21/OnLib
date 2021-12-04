
#include "MainWindow.h"
#include <iostream>
#include "LoginWindow.h"
#include <QtWidgets>
#include <QPixmap>
#include "Book.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QIcon icon("Images/online-library.png");
	MainWindow::setWindowIcon(icon);
	MainWindow::setWindowTitle("Online library");

	categories =
	{
		{data::BookCategory::SF, new BookSection("SF",this)},
		{data::BookCategory::Romantic, new BookSection("Romantic",this)},
		{data::BookCategory::Action, new BookSection("Action",this)},
		{data::BookCategory::Comedy, new BookSection("Comedy",this)}
	};

	for (const auto& [section, widget] : categories)
	{
		ui->verticalLayout->addWidget(widget);
	}

	std::vector<data::Book> vec =
	{
		{0,"abc","abc","abc",data::BookCategory::SF,{},{"A","B"},3}
	};
	vec[0].mainCategory = data::BookCategory::SF;

	AddBooksToSection(vec);

	ui->searchBooksButton->hide();
	ui->searchLineEdit->hide();
	ui->backToMenuButton->hide();
	ui->searchBooksScrollArea->hide();
	
	ui->mainToolBar->setFixedHeight(70);


	connect(ui->actionLogOut, SIGNAL(triggered()), this, SLOT(HandleLogOutButton()));
	connect(ui->actionDeleteAccount, SIGNAL(triggered()), this, SLOT(HandleDeleteAccountButton()));
	connect(ui->actionSearchIcon, SIGNAL(triggered()), this, SLOT(HandleSearchIconButton()));
	connect(ui->backToMenuButton, &QPushButton::released, this, &MainWindow::HandleBackToMenuButton);
	connect(ui->searchBooksButton, &QPushButton::released, this, [=]() { HandleSearchBooksButton(ui->searchLineEdit->text().toStdString()); });





}


void MainWindow::HandleLogOutButton()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Log out confirmation");
	msgBox.setText(QString::fromStdString("Hello, , are you sure you want to log out?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);

	int result = msgBox.exec();
	switch (result) {
	case QMessageBox::Yes:
		this->close();
		LoginWindow* window = new LoginWindow;
		window->show();
		break;
	}
}

void MainWindow::HandleDeleteAccountButton()
{
	bool ok;
	if (QInputDialog::getText(this, "Confirm password", "Password:", QLineEdit::Password, "", &ok) == QString::fromStdString("password"))
	{
		//delete user...
		if (ok)
		{
			this->close();
			LoginWindow* window = new LoginWindow;
			window->show();
			QMessageBox msgBox;
			msgBox.setText("Your account was deleted succesfully.");
			msgBox.exec();
		}
	}
	else
	{
		if (ok)
		{
			QMessageBox msgBox;
			msgBox.setText("Incorrect password, try again later.");
			msgBox.exec();
		}
	}
}

void MainWindow::HandleSearchIconButton()
{
	ui->tabWidget->setCurrentIndex(0);

	ui->backToMenuButton->show();
	ui->searchLineEdit->show();
	ui->searchBooksButton->show();




	ui->searchBooksScrollArea->show();
	ui->actionSearchIcon->setVisible(false);
}

void MainWindow::HandleBackToMenuButton()
{
	ui->backToMenuButton->hide();
	ui->searchLineEdit->hide();
	ui->searchBooksButton->hide();

	ui->searchBooksScrollArea->hide();
	ui->backToMenuButton->hide();

	ui->actionSearchIcon->setVisible(true);

	ui->searchLineEdit->setText("");
	HandleSearchBooksButton("");
}

void MainWindow::HandleSearchBooksButton(std::string s)
{

	QWidget* wgtMainMyList = new QWidget();
	QHBoxLayout* hboxMainMyList = new QHBoxLayout(wgtMainMyList);
	QWidget* wgtSubMyList;
	QVBoxLayout* vboxSubMyList;
	QPixmap pix("Images/defaultImage.png");
	QLabel* label;

	if (s != "")
	{
		
	}
	ui->searchBooksScrollArea->setWidget(wgtMainMyList);
}

void MainWindow::AddBooksToSection(const std::vector<data::Book>& books)
{

	for (const auto& book : books)
	{
		BookSection* section = categories[book.mainCategory];
		BookPreview* bookPreview = new BookPreview(book, section);
		section->AddBook(bookPreview);
	}

}

MainWindow::~MainWindow()
{
	delete ui;
}