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

	BookSection* myListBookSection = new BookSection("My list", this);
	ui->myListGridLayout->addWidget(myListBookSection);

	connect(ui->actionLogOut, &QAction::triggered, [this]() {emit LogOutRequest(); });
	connect(ui->actionDeleteAccount, &QAction::triggered, [this]() {emit DeleteAccountRequest(QInputDialog::getText(this, "Confirm password", "Password:", QLineEdit::Password, "").toStdString()); });
	connect(ui->actionHome, SIGNAL(triggered()), this, SLOT(HandleHomeButton()));
	connect(ui->actionMyList, SIGNAL(triggered()), this, SLOT(HandleMyListButton()));
	connect(ui->actionSearchIcon, &QAction::triggered, [this]() {emit HandleSearchIconButton(); });
	connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(HandleRefreshButton()));

	ui->actionSearchButton->setVisible(false);

	searchLineEdit = new QLineEdit();
	searchLineEditWidgetAction = ui->toolbar2->addWidget(searchLineEdit);
	searchLineEditWidgetAction->setVisible(false);

	StyleSheets();

	this->setMinimumHeight(600);
	this->setMinimumWidth(700);
}

void MainWindow::HandleSearchIconButton()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->actionSearchIcon->setVisible(false);

	ui->actionSearchButton->setVisible(true);
	searchLineEditWidgetAction->setVisible(true);

	emit SearchRequest(searchLineEditWidgetAction->text().toStdString());
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

void MainWindow::HandleRefreshButton()
{
	this->repaint();
	this->centralWidget()->repaint();
	ui->stackedWidget->repaint();
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

		QSize currentSize = this->normalGeometry().size();
		section->resize(currentSize / 2);

		BookPreview* bookPreview = new BookPreview(book, section);
		section->AddBook(bookPreview);

		connect(bookPreview, &BookPreview::BorrowPressed, [this](uint64_t id) {
			emit BorrowBookRequest(id);
			});
		connect(bookPreview, &BookPreview::BookDetailsPressed, [this](const data::Book book) {
			emit BookDetailsRequest(book);
			});

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, bookPreview]() {
			bookPreview->BookCoverRecieved(coverDownloader->GetData());
			});
	}
}

void MainWindow::AddBorrowedBooks(const std::vector<data::LendBook>& books)
{
	for (const auto& book : books)
	{
		MyListBookPreview* preview = new MyListBookPreview(book, this);

		ui->myListGridLayout->addWidget(preview);

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, preview]() {
			preview->BookCoverRecieved(coverDownloader->GetData());
			});
	}
}