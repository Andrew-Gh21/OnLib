#include "LoginWindow.h"
#include "MainWindow.h"
#include"User.h"
#include"MockBooksAndUsers.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent),ui(new Ui::LoginWindow)
{
	ui->setupUi(this);


    ui->confirmPasswordLabel->hide();
    ui->confirmPasswordLineEdit->hide();
    ui->backToLoginButton->hide();

    mainWindow = new MainWindow;

    connect(ui->loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(ui->signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);
    connect(ui->backToLoginButton, &QPushButton::released, this, &LoginWindow::HandleBackToLoginButton);

    connect(this, SIGNAL(loginButtonClicked(std::string)), mainWindow, SLOT(loginButtonClicked(std::string)));
    this->show();



}

void LoginWindow::HandleLoginButton()
{
    //auto attributeFinder = [](User user, const std::string& name)->bool {return user.GetUsername() == name; };

    //auto attr_iter = std::find_if(mock.GetUsers().begin(), 
    //    mock.GetUsers().end(), 
    //    std::bind(attributeFinder, ui.usernameLineEdit->text().toStdString()));

    for (auto it : mock.GetUsers())
    {
        if (ui->usernameLineEdit->text().toStdString() == it.GetUsername() && ui->passwordLineEdit->text().toStdString()==it.GetPassword())
        {

                this->hide();
                emit loginButtonClicked(ui->usernameLineEdit->text().toStdString());
                mainWindow->show();

          
        }
    }

}

void LoginWindow::HandleSignUpButton()
{
    ui->loginButton->hide();
    ui->confirmPasswordLineEdit->show();
    ui->backToLoginButton->show();
    ui->mainLayout->labelForField(ui->confirmPasswordLineEdit)->show();
}

void LoginWindow::HandleBackToLoginButton()
{
    ui->confirmPasswordLineEdit->hide();
    ui->loginButton->show();
    ui->backToLoginButton->hide();
    ui->confirmPasswordLabel->hide();
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
