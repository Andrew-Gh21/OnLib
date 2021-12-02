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

	//BorrowBook bb(User user(), Book book);

	AddBooksToScrollArea();
}

void MainWindow::AddBooksToScrollArea()
{
	/*QWidget* wgtMainSF = new QWidget();
	QWidget* wgtMainAction = new QWidget();
	QWidget* wgtMainComedy = new QWidget();
	QWidget* wgtMainRomance = new QWidget();
	QWidget* wgtMainDrama = new QWidget();

	QHBoxLayout* hboxMainSF = new QHBoxLayout(wgtMainSF);
	QHBoxLayout* hboxMainAction = new QHBoxLayout(wgtMainAction);
	QHBoxLayout* hboxMainComedy = new QHBoxLayout(wgtMainComedy);
	QHBoxLayout* hboxMainRomance = new QHBoxLayout(wgtMainRomance);
	QHBoxLayout* hboxMainDrama = new QHBoxLayout(wgtMainDrama);

	QWidget* wgtSubSF;
	QWidget* wgtSubAction;
	QWidget* wgtSubComedy;
	QWidget* wgtSubRomance;
	QWidget* wgtSubDrama;

	QVBoxLayout* vboxSubSF;
	QVBoxLayout* vboxSubAction;
	QVBoxLayout* vboxSubComedy;
	QVBoxLayout* vboxSubRomance;
	QVBoxLayout* vboxSubDrama;

	QPushButton* button;
	QPixmap pix("Images/defaultImage.png");

	QLabel* label;

	for (auto it : mock.GetBooks())
	{

		switch (it.GetType())
		{
		case TypeOfBook::SF:
			wgtSubSF = new QWidget();
			vboxSubSF = new QVBoxLayout(wgtSubSF);
			label = new QLabel();
			label->setMaximumHeight(100);
			label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			vboxSubSF->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubSF->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubSF->addWidget(label);
			button = new QPushButton(this);
			button->setText("Borrow book");
			connect(button, &QPushButton::released, this, [=]() { HandleAddToMyListButton(it); });
			vboxSubSF->addWidget(button);
			button->show();
			hboxMainSF->addWidget(wgtSubSF);
			ui->sfGenreScrollArea->setWidget(wgtMainSF);
			break;

		case TypeOfBook::Action:
			wgtSubAction = new QWidget();
			vboxSubAction = new QVBoxLayout(wgtSubAction);
			label = new QLabel();
			label->setMaximumHeight(100);
			label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			vboxSubAction->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubAction->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubAction->addWidget(label);
			button = new QPushButton(this);
			button->setText("Borrow book");
			connect(button, &QPushButton::released, this, [=]() { HandleAddToMyListButton(it); });
			vboxSubAction->addWidget(button);
			button->show();
			hboxMainAction->addWidget(wgtSubAction);
			ui->actionGenreScrollArea->setWidget(wgtMainAction);
			break;

		case TypeOfBook::Comedy:
			wgtSubComedy = new QWidget();
			vboxSubComedy = new QVBoxLayout(wgtSubComedy);
			label = new QLabel();
			label->setMaximumHeight(100);
			label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			vboxSubComedy->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubComedy->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubComedy->addWidget(label);
			button = new QPushButton(this);
			button->setText("Borrow book");
			connect(button, &QPushButton::released, this, [=]() { HandleAddToMyListButton(it); });
			vboxSubComedy->addWidget(button);
			button->show();
			hboxMainComedy->addWidget(wgtSubComedy);
			ui->comedyGenreScrollArea->setWidget(wgtMainComedy);
			break;

		case TypeOfBook::Romance:
			wgtSubRomance = new QWidget();
			vboxSubRomance = new QVBoxLayout(wgtSubRomance);
			label = new QLabel();
			label->setMaximumHeight(100);
			label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			vboxSubRomance->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubRomance->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubRomance->addWidget(label);
			button = new QPushButton(this);
			button->setText("Borrow book");
			connect(button, &QPushButton::released, this, [=]() { HandleAddToMyListButton(it); });
			vboxSubRomance->addWidget(button);
			button->show();
			hboxMainRomance->addWidget(wgtSubRomance);
			ui->romanceGenreScrollArea->setWidget(wgtMainRomance);
			break;

		case TypeOfBook::Drama:
			wgtSubDrama = new QWidget();
			vboxSubDrama = new QVBoxLayout(wgtSubDrama);
			label = new QLabel();
			label->setMaximumHeight(100);
			label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			vboxSubDrama->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubDrama->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubDrama->addWidget(label);
			button = new QPushButton(this);
			button->setText("Borrow book");
			connect(button, &QPushButton::released, this, [=]() { HandleAddToMyListButton(it); });
			vboxSubDrama->addWidget(button);
			button->show();
			hboxMainDrama->addWidget(wgtSubDrama);
			ui->dramaGenreScrollArea->setWidget(wgtMainDrama);
			break;

		default:
			break;
		}
	}*/
}

void MainWindow::AddBooksToMyList()
{
	QWidget* wgtMainMyList = new QWidget();
	QHBoxLayout* hboxMainMyList = new QHBoxLayout(wgtMainMyList);
	QWidget* wgtSubMyList;
	QVBoxLayout* vboxSubMyList;
	QPushButton* button;
	QPixmap pix("Images/defaultImage.png");
	QLabel* label;

	for (auto it : user.GetBorrowedBooks())
	{
		wgtSubMyList = new QWidget();
		vboxSubMyList = new QVBoxLayout(wgtSubMyList);
		label = new QLabel();
		label->setMaximumHeight(100);
		label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		vboxSubMyList->addWidget(label);
		label = new QLabel(QString::fromStdString(it.GetBook().GetName()));
		vboxSubMyList->addWidget(label);
		label = new QLabel(QString::fromStdString(it.GetBook().GetAuthor()));
		vboxSubMyList->addWidget(label);
		button = new QPushButton(this);
		button->setText("Return book");
		Book bk = it.GetBook();
		connect(button, &QPushButton::released, this, [=]() { HandleReturnBookButton(bk); });
		vboxSubMyList->addWidget(button);
		button->show();
		button = new QPushButton(this);
		button->setText("Extend return date");
		connect(button, &QPushButton::released, this, [=]() { HandleExtendDateButton(it); });
		vboxSubMyList->addWidget(button);
		button->show();
		hboxMainMyList->addWidget(wgtSubMyList);
	}
	ui->myListScrollArea->setWidget(wgtMainMyList);
}

void MainWindow::HandleAddToMyListButton(Book book)
{
	QMessageBox msgBox;
	//msgBox.setWindowTitle("Added book");
	//msgBox.setText(QString::fromStdString("Book  " + book.GetName() + " added to your list"));
	//msgBox.exec();

	if (user.GetBorrowLimit() > 0)
	{
		bool foundedBook = false;
		for (auto it : user.GetBorrowedBooks())
		{
			if (it.GetBook().GetName() == book.GetName() && it.GetBook().GetAuthor() == book.GetAuthor())
			{
				msgBox.setWindowTitle("Failed adding book");
				msgBox.setText(QString::fromStdString("Book  " + book.GetName()) + " already exists in your list");
				msgBox.exec();
				foundedBook = true;
				break;
			}
		}
		if (foundedBook == false)
		{
			BorrowBook borrowBook(book);
			user.AddBorrowBook(borrowBook);
			msgBox.setWindowTitle("Added book");
			msgBox.setText(QString::fromStdString("Book  " + borrowBook.GetBook().GetName()) + " added to your list");
			msgBox.exec();
			user.DecrementBorrowLimit();
		}
	}
	else
	{
		msgBox.setWindowTitle("Failed adding book");
		msgBox.setText(QString::fromStdString("You can't borrow any books, first you have to return a book."));
		msgBox.exec();
	}
	AddBooksToMyList();
}

void MainWindow::HandleReturnBookButton(Book book)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Return book confirmation");
	msgBox.setText(QString::fromStdString("Hello, " + user.GetUsername() + ", are you sure you want to return " + book.GetName()) + "?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);

	int result = msgBox.exec();
	switch (result) {
	case QMessageBox::Yes:
		for (auto it : user.GetBorrowedBooks())
		{
			if (it.GetBook().GetName() == book.GetName() && it.GetBook().GetAuthor() == book.GetAuthor())
			{
				user.RemoveBorrowedBook(book);
				user.IncrementBorrowLimit();
				break;
			}
		}
		AddBooksToMyList();
		break;
	}
}
void MainWindow::HandleExtendDateButton(BorrowBook book)
{
	book.ExtendReturnDate();
}

void MainWindow::LoginButtonClicked(User user)
{
	this->user = user;
}

void MainWindow::HandleLogOutButton()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Log out confirmation");
	msgBox.setText(QString::fromStdString("Hello, " + user.GetUsername()) + ", are you sure you want to log out?");
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
	if (QInputDialog::getText(this, "Confirm password", "Password:", QLineEdit::Password, "", &ok) == QString::fromStdString(user.GetPassword()))
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

	ui->recommendedBooksScrollArea->hide();
	ui->recommendedBooksLabel->hide();

	ui->actionGenreScrollArea->hide();
	ui->actionGenreLabel->hide();

	ui->sfGenreScrollArea->hide();
	ui->sfGenreLabel->hide();

	ui->comedyGenreScrollArea->hide();
	ui->comedyGenreLabel->hide();

	ui->romanceGenreScrollArea->hide();
	ui->romanceGenreLabel->hide();

	ui->dramaGenreScrollArea->hide();
	ui->dramaGenreLabel->hide();

	ui->searchBooksScrollArea->show();
	ui->actionSearchIcon->setVisible(false);
}

void MainWindow::HandleBackToMenuButton()
{
	ui->backToMenuButton->hide();
	ui->searchLineEdit->hide();
	ui->searchBooksButton->hide();

	ui->recommendedBooksScrollArea->show();
	ui->recommendedBooksLabel->show();

	ui->actionGenreScrollArea->show();
	ui->actionGenreLabel->show();

	ui->sfGenreScrollArea->show();
	ui->sfGenreLabel->show();

	ui->comedyGenreScrollArea->show();
	ui->comedyGenreLabel->show();

	ui->romanceGenreScrollArea->show();
	ui->romanceGenreLabel->show();

	ui->dramaGenreScrollArea->show();
	ui->dramaGenreLabel->show();

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
		for (auto it : mock.GetBooks())
		{
			if (it.GetName().find(s) != std::string::npos || it.GetAuthor().find(s) != std::string::npos)
			{
				wgtSubMyList = new QWidget();
				vboxSubMyList = new QVBoxLayout(wgtSubMyList);
				label = new QLabel();
				label->setMaximumHeight(100);
				label->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
				vboxSubMyList->addWidget(label);
				label = new QLabel(QString::fromStdString(it.GetName()));
				vboxSubMyList->addWidget(label);
				label = new QLabel(QString::fromStdString(it.GetAuthor()));
				vboxSubMyList->addWidget(label);
				hboxMainMyList->addWidget(wgtSubMyList);
			}
		}
	}
	ui->searchBooksScrollArea->setWidget(wgtMainMyList);
}

MainWindow::~MainWindow()
{
	delete ui;
}