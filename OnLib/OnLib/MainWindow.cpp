#include "MainWindow.h"
#include <iostream>
#include "LoginWindow.h"
#include <QtWidgets>
#include "Book.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "FileDownloader.h"

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
		ui->genreVerticalLayout->addWidget(widget);
	}

	ui->toolbar1->setFixedHeight(70);

	BookSection* myListBookSection=new BookSection("My list",this);
	ui->myListGridLayout->addWidget(myListBookSection);

	connect(ui->actionLogOut, SIGNAL(triggered()), this, SLOT(HandleLogOutButton()));
	connect(ui->actionDeleteAccount, SIGNAL(triggered()), this, SLOT(HandleDeleteAccountButton()));
	connect(ui->actionHome, SIGNAL(triggered()), this, SLOT(HandleHomeButton()));
	connect(ui->actionMyList, SIGNAL(triggered()), this, SLOT(HandleMyListButton()));

	ui->actionSearchButton->setVisible(false);

	searchLineEdit = new QLineEdit();
	searchLineEditWidgetAction = ui->toolbar2->addWidget(searchLineEdit);
	searchLineEditWidgetAction->setVisible(false);

	StyleSheets();

	connect(ui->actionSearchIcon, SIGNAL(triggered()), this, SLOT(HandleSearchIconButton()));

	this->setMinimumHeight(600);
	this->setMinimumWidth(700);
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
	ui->stackedWidget->setCurrentIndex(1);
	ui->actionSearchIcon->setVisible(false);

	ui->actionSearchButton->setVisible(true);
	searchLineEditWidgetAction->setVisible(true);
}

void MainWindow::HandleHomeButton()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->actionSearchIcon->setVisible(true);

	ui->actionSearchButton->setVisible(false);
	searchLineEditWidgetAction->setVisible(false);

	searchLineEdit->setText("");
	HandleSearchBooksButton("");
}

void MainWindow::HandleMyListButton()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->actionSearchIcon->setVisible(true);

	ui->actionSearchButton->setVisible(false);
	searchLineEditWidgetAction->setVisible(false);

	searchLineEdit->setText("");
	HandleSearchBooksButton("");
}

void MainWindow::HandleSearchBooksButton(std::string s)
{
	if (s != "")
	{
	}
}

void MainWindow::StyleSheets()
{
	searchLineEdit->setFixedSize(300, 30);

	searchLineEdit->setStyleSheet("color: black;"
		"background-color: white;"
		"selection-color: white;"
		"border: 2px solid #004d4d;"
		"selection-background-color: black;"
		"border-top-left-radius: 7px;"
		"border-top-right-radius: 7px;"
		"border-bottom-left-radius: 7px;"
		"border-bottom-right-radius: 7px;");
}

void MainWindow::AddBooksToSection(const std::vector<data::Book>& books)
{
	for (const auto& book : books)
	{
		BookSection* section = categories[book.mainCategory];

		QSize  currentSize = this->normalGeometry().size();
		section->resize(currentSize / 2);

		BookPreview* bookPreview = new BookPreview(book, section);

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, bookPreview]() {
			bookPreview->BookCoverRecieved(coverDownloader->GetData());
			});

		section->AddBook(bookPreview);
	}
}