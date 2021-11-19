#include "MainWindow.h"
#include<iostream>
#include "LoginWindow.h"
#include "BorrowBook.h"
#include <QtWidgets>
#include<QPixmap>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionLogOut, SIGNAL(triggered()), this, SLOT(HandleLogOutButton()));
	connect(ui->actionDeleteAccount, SIGNAL(triggered()), this, SLOT(HandleDeleteAccountButton()));

	//BorrowBook bb(User user(), Book book);

	AddBooksToScrollArea();
}

void MainWindow::AddBooksToScrollArea()
{
	QWidget* wgtMainSF = new QWidget();
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

	QLabel* label;

	for (auto it : mock.GetBooks())
	{

		switch (it.GetType())
		{
		case TypeOfBook::SF:
			wgtSubSF = new QWidget();
			vboxSubSF = new QVBoxLayout(wgtSubSF);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubSF->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubSF->addWidget(label);
			hboxMainSF->addWidget(wgtSubSF);
			ui->sfGenreScrollArea->setWidget(wgtMainSF);
			break;
		case TypeOfBook::Action:
			wgtSubAction = new QWidget();
			vboxSubAction = new QVBoxLayout(wgtSubAction);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubAction->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubAction->addWidget(label);
			hboxMainAction->addWidget(wgtSubAction);
			ui->actionGenreScrollArea->setWidget(wgtMainAction);
			break;
		case TypeOfBook::Comedy:
			wgtSubComedy = new QWidget();
			vboxSubComedy = new QVBoxLayout(wgtSubComedy);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubComedy->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubComedy->addWidget(label);
			hboxMainComedy->addWidget(wgtSubComedy);
			ui->comedyGenreScrollArea->setWidget(wgtMainComedy);
			break;
		case TypeOfBook::Romance:
			wgtSubRomance = new QWidget();
			vboxSubRomance = new QVBoxLayout(wgtSubRomance);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubRomance->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubRomance->addWidget(label);
			hboxMainRomance->addWidget(wgtSubRomance);
			ui->romanceGenreScrollArea->setWidget(wgtMainRomance);
			break;
		case TypeOfBook::Drama:
			wgtSubDrama = new QWidget();
			vboxSubDrama = new QVBoxLayout(wgtSubDrama);
			label = new QLabel(QString::fromStdString(it.GetName()));
			vboxSubDrama->addWidget(label);
			label = new QLabel(QString::fromStdString(it.GetAuthor()));
			vboxSubDrama->addWidget(label);
			hboxMainDrama->addWidget(wgtSubDrama);
			ui->dramaGenreScrollArea->setWidget(wgtMainDrama);
			break;
		default:
			break;
		}
	}
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

MainWindow::~MainWindow()
{
	delete ui;
}
