#include "MainWindow.h"
#include <iostream>
#include "LoginWindow.h"
#include <QtWidgets>
#include "Book.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "FileDownloader.h"
#include "BookDetails.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->stackedWidget->setCurrentIndex(0);

	QIcon icon("Images/online-library.png");
	MainWindow::setWindowIcon(icon);
	MainWindow::setWindowTitle("Online library");

	categories =
	{
		{data::BookCategory::SF, new BookSection("SF",this)},
		{data::BookCategory::Romance, new BookSection("Romance",this)},
		{data::BookCategory::Action, new BookSection("Action",this)},
		{data::BookCategory::Comedy, new BookSection("Comedy",this)}
	};

	for (const auto& [section, widget] : categories)
	{
		ui->genreVerticalLayout->addWidget(widget);
	}

	ui->toolbar1->setFixedHeight(70);

	myListBookSection = new BookSection("My list", this);
	ui->myListGridLayout->addWidget(myListBookSection);

	ui->actionSearchButton->setVisible(false);

	searchLineEdit = new QLineEdit();
	searchLineEditWidgetAction = ui->toolbar2->addWidget(searchLineEdit);
	searchLineEditWidgetAction->setVisible(false);

	connect(ui->actionLogOut, &QAction::triggered, [this]() {emit LogOutRequest(); });
	connect(ui->actionDeleteAccount, &QAction::triggered, [this]() {emit DeleteAccountRequest(QInputDialog::getText(this, "Confirm password", "Password:", QLineEdit::Password, "").toStdString()); });
	connect(ui->actionHome, SIGNAL(triggered()), this, SLOT(HandleHomeButton()));
	connect(ui->actionMyList, SIGNAL(triggered()), this, SLOT(HandleMyListButton()));
	connect(ui->actionSearchIcon, &QAction::triggered, [this]() {emit HandleSearchIconButton(); });
	connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(HandleRefreshButton()));
	connect(ui->actionSearchButton, &QAction::triggered, [this]() {emit SearchRequest(searchLineEdit->text().toStdString()); });

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

	searchSection = new BookSection("", this);
	searchSection->setMinimumHeight(this->height() - 300);
	searchSection->repaint();

	//emit SearchRequest(searchLineEditWidgetAction->text().toStdString());
}

void MainWindow::HandleHomeButton()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->actionSearchIcon->setVisible(true);

	ui->actionSearchButton->setVisible(false);
	searchLineEditWidgetAction->setVisible(false);

	searchLineEdit->setText("");
}

void MainWindow::HandleMyListButton()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->actionSearchIcon->setVisible(true);

	ui->actionSearchButton->setVisible(false);
	searchLineEditWidgetAction->setVisible(false);

	searchLineEdit->setText("");
}

void MainWindow::HandleRefreshButton()
{
	for (QWidget* book : visibleBooks)
	{
		book->hide();
		book->deleteLater();
	}

	visibleBooks.clear();

	emit RefreshButtonPressed();
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

		QSize currentSize = this->size();
		section->setMinimumSize(currentSize.width() - 100, currentSize.height() - 200);

		BookPreview* bookPreview = new BookPreview(book, section) ;
		section->AddBook(bookPreview) ;

		QSize size = bookPreview->size();
		bookPreview->resize(size.width(), size.height());

		visibleBooks.push_back(bookPreview);

		connect(bookPreview, &BookPreview::BorrowPressed, [this](uint64_t id) {
			emit BorrowBookRequest(id);
			});

		connect(bookPreview, &BookPreview::BookReviewPressed, [this](int rating, uint64_t bookId) {emit BookRated(rating, bookId); });

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, bookPreview]() {
			bookPreview->BookCoverRecieved(coverDownloader->GetData());
			});

		connect(bookPreview->ui.detailsButton, &QPushButton::clicked, [this, book, bookPreview] {SeeBookDetails(book, bookPreview->ui.cover->pixmap()); });
	}

	for (const auto& [category, widget] : categories)
	{
		widget->setMinimumHeight(this->height() - 200);
		widget->repaint();
	}
}

void MainWindow::SeeBookDetails(const data::Book &book, QPixmap cover)
{
	QLayoutItem* child;
	while (ui->bookDetailsGridLayout->count() != 0) {
		child = ui->bookDetailsGridLayout->takeAt(0);
		if (child->widget() != 0) {
			delete child->widget();
		}

		delete child;
	}

	ui->stackedWidget->setCurrentIndex(3);
	BookDetails* bookDetails = new BookDetails(book);
	bookDetails->ui.cover->setPixmap(cover.scaled(bookDetails->ui.cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
	ui->bookDetailsGridLayout->addWidget(bookDetails);
	connect(bookDetails->ui.backButton, &QPushButton::clicked, [this] {ui->stackedWidget->setCurrentIndex(0); });

	connect(bookDetails, &BookDetails::BorrowPressed, [this](uint64_t id) {
		emit BorrowBookRequest(id);
		});
}

void MainWindow::SeeBookText(const data::LendBook& book)
{
	QLayoutItem* child;
	while (ui->bookDetailsGridLayout->count() != 0) {
		child = ui->bookDetailsGridLayout->takeAt(0);
		if (child->widget() != 0) {
			delete child->widget();
		}

		delete child;
	}

	ui->stackedWidget->setCurrentIndex(4);
	ui->text->setText("This page will display book content.");
	connect(ui->backHomeButton, &QPushButton::clicked, [this] {ui->stackedWidget->setCurrentIndex(0); });
}

void MainWindow::AddBorrowedBooks(const std::vector<data::LendBook>& books)
{
	BookSection* myListSection = new BookSection("My list", this);
	myListSection->setMinimumHeight(this->height() - 400);
	myListSection->repaint();

	for (const auto& book : books)
	{
		MyListBookPreview* preview = new MyListBookPreview(book, myListSection);

		preview->setMinimumWidth(300);
		myListSection->AddBook(preview);
		myListBookSection->AddBook(preview);

		visibleBooks.push_back(preview);

		connect(preview, &MyListBookPreview::ReturnButtonPressed, [this](uint64_t id) {emit ReturnBookRequest(id); });

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, preview]() {
			preview->BookCoverRecieved(coverDownloader->GetData());
			});
		connect(preview->ui.readBookButton, &QPushButton::clicked, [this,book] {SeeBookText(book); });
	}
}

void MainWindow::AddSearchedBooks(const std::vector<data::Book>& books)
{
	for (QWidget* book : searchBooks)
	{
		book->hide();
		book->deleteLater();
	}

	searchBooks.clear();

	for (const auto& book : books)
	{
		BookPreview* bookPreview = new BookPreview(book, searchSection);

		bookPreview->setMinimumWidth(300);
		searchSection->AddBook(bookPreview);

		ui->searchPageGridLayout->addWidget(searchSection);

		searchBooks.push_back(bookPreview);

		connect(bookPreview, &BookPreview::BorrowPressed, [this](uint64_t id) {
			emit BorrowBookRequest(id);
			});

		connect(bookPreview, &BookPreview::BookReviewPressed, [this](int rating, uint64_t bookId) {emit BookRated(rating, bookId); });

		FileDownloader* coverDownloader = new FileDownloader(book.coverUrl, this);
		connect(coverDownloader, &FileDownloader::DownloadFinished, [coverDownloader, bookPreview]() {
			bookPreview->BookCoverRecieved(coverDownloader->GetData());
			});

		connect(bookPreview->ui.detailsButton, &QPushButton::clicked, [this, book, bookPreview] {SeeBookDetails(book, bookPreview->ui.cover->pixmap()); });
	}

	
}
