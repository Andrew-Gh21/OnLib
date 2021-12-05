
#include "MainWindow.h"
#include <iostream>
#include "LoginWindow.h"
#include <QtWidgets>
#include <QPixmap>
#include "Book.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

	std::vector<data::Book> vec =
	{
		{0,"abc","abc","abc",data::BookCategory::SF,{},{"A","B"},3}
	};
	vec[0].mainCategory = data::BookCategory::SF;

	AddBooksToSection(vec);

	ui->iconToolBar->setFixedHeight(70);

	connect(ui->actionLogOut, SIGNAL(triggered()), this, SLOT(HandleLogOutButton()));
	connect(ui->actionDeleteAccount, SIGNAL(triggered()), this, SLOT(HandleDeleteAccountButton()));
	connect(ui->actionSearchIcon, SIGNAL(triggered()), this, SLOT(HandleSearchIconButton()));
	//connect(ui->backToMenuButton, &QPushButton::released, this, &MainWindow::HandleBackToMenuButton);
	//connect(ui->searchBooksButton, &QPushButton::released, this, [=]() { HandleSearchBooksButton(ui->searchLineEdit->text().toStdString()); });

	QPixmap pm = DownloadImageFrom("https://www.gimp.org/tutorials/Lite_Quickies/m51_hallas_big.jpg");
	QPixmap pm1 = DownloadImageFrom("https://cdn.pixabay.com/photo/2015/04/23/22/00/tree-736885__480.jpg");
	QPixmap pm2 = DownloadImageFrom("https://images.ctfassets.net/hrltx12pl8hq/7yQR5uJhwEkRfjwMFJ7bUK/dc52a0913e8ff8b5c276177890eb0129/offset_comp_772626-opt.jpg?fit=fill&w=800&h=300");
	QPixmap pm3 = DownloadImageFrom("https://images.unsplash.com/photo-1453728013993-6d66e9c9123a?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxzZWFyY2h8Mnx8dmlld3xlbnwwfHwwfHw%3D&w=1000&q=80");
	QPixmap pm4 = DownloadImageFrom("https://static.vecteezy.com/packs/media/components/global/search-explore-nav/img/vectors/term-bg-1-666de2d941529c25aa511dc18d727160.jpg");
	ui->labelImage->setPixmap(pm);
	ui->label->setPixmap(pm1);
	ui->label_2->setPixmap(pm2);
	ui->label_3->setPixmap(pm3);
	ui->label_4->setPixmap(pm4);
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
	ui->actionSearchIcon->setVisible(false);
}

void MainWindow::HandleBackToMenuButton()
{
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
	//ui->searchBooksScrollArea->setWidget(wgtMainMyList);
}

QPixmap MainWindow::DownloadImageFrom(const QString& url)
{
	QNetworkAccessManager nam;
	QEventLoop loop;
	QObject::connect(&nam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
	QNetworkReply* reply = nam.get(QNetworkRequest(url));
	loop.exec();

	QPixmap pm;
	pm.loadFromData(reply->readAll());

	delete reply;
	return pm;
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