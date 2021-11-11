#include "LoginWindow.h"
#include "MainWindow.h"
#include"User.h"
#include"MockBooksAndUsers.h"
#include<QMessageBox>

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->confirmPasswordLabel->hide();
    ui->confirmPasswordLineEdit->hide();
    ui->backToLoginButton->hide();
    ui->signupButton->hide();

    mainWindow = new MainWindow;

    connect(ui->loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(ui->signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);
    connect(ui->backToLoginButton, &QPushButton::released, this, &LoginWindow::HandleBackToLoginButton);
    connect(ui->registerButton, &QPushButton::released, this, &LoginWindow::HandleRegisterButton);

    //connect(this, SIGNAL(loginButtonClicked(std::string)), mainWindow, SLOT(loginButtonClicked(std::string)));
    this->show();

}

void LoginWindow::HandleLoginButton()
{
    //auto attributeFinder = [](User user, const std::string& name)->bool {return user.GetUsername() == name; };

    //auto attr_iter = std::find_if(mock.GetUsers().begin(), 
    //    mock.GetUsers().end(), 
    //    std::bind(attributeFinder, ui.usernameLineEdit->text().toStdString()));
    bool succesfullyLogin = false;
    for (auto it : mock.GetUsers())
    {
        if (ui->usernameLineEdit->text().toStdString() == it.GetUsername() && ui->passwordLineEdit->text().toStdString() == it.GetPassword())
        {

            this->hide();
            //emit loginButtonClicked(ui->usernameLineEdit->text().toStdString());
            mainWindow->showMaximized();
            succesfullyLogin = true;
        }
    }
    if (succesfullyLogin == false)
    {
        QMessageBox::warning(this, "Login", "Username and password doesen't match.");
    }
}

void LoginWindow::HandleSignUpButton()
{
    if (ui->usernameLineEdit->text().size() >= 5 && ui->passwordLineEdit->text().size() >= 5 && ui->confirmPasswordLineEdit->text().size() >= 5)
    {
        if (ui->passwordLineEdit->text() == ui->confirmPasswordLineEdit->text())
        {
            /* QRegularExpression reg("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[a-zA-Z]).{8,}$");
             QValidator* validator = new QRegularExpressionValidator(reg, this);

             if (reg.match(ui->passwordLineEdit->text()).hasMatch())
             {
                 mock.AddUser(User(ui->usernameLineEdit->text().toStdString(), ui->passwordLineEdit->text().toStdString()));
                 QMessageBox::information(this, "Register", "Account created succesfully");
                 HandleBackToLoginButton();
             }*/
        }
        else
        {
            QMessageBox::warning(this, "Register", "Passwords don't match");
        }
    }
    else
    {
        QMessageBox::warning(this, "Register", "Username and password length have to be min 5 char");
    }
}

void LoginWindow::HandleRegisterButton()
{
    ui->loginButton->hide();
    ui->confirmPasswordLineEdit->show();
    ui->backToLoginButton->show();
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->show();
    ui->confirmPasswordLabel->show();
    ui->registerButton->hide();
    ui->registerButtonLabel->hide();
    ui->signupButton->show();
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
    ui->signupButton->hide();
    ui->registerButton->show();
    ui->registerButtonLabel->show();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}