#include "LoginWindow.h"
#include "MainWindow.h"
#include "User.h"
#include "MockBooksAndUsers.h"
#include <QMessageBox>
#include<QRegularExpressionValidator>
#include <QPixmap>

#include <iostream>

void LoginWindow::OnLoginFailure(const std::vector<data::LogginErrors>& errors)
{
    for (auto error : errors)
        std::cout << static_cast<uint8_t>(error);
}

void LoginWindow::OnLoginSucces()
{
    std::cout << "Login successfull";
}

void LoginWindow::OnRegisterSuccess()
{
    std::cout << "Register successfull";
}

void LoginWindow::OnRegisterFailure(const std::vector<data::RegisterErrors>& errors)
{
    for (auto error : errors)
        std::cout << static_cast<uint8_t>(error);
}

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QIcon icon("../online-library.png");
    LoginWindow::setWindowIcon(icon);
    LoginWindow::setWindowTitle("Online library - Account");

    ui->confirmPasswordLabel->hide();
    ui->confirmPasswordLineEdit->hide();
    ui->backToLoginButton->hide();
    ui->signupButton->hide();

    mainWindow = new MainWindow;
    mainWindow->showMaximized();

    connect(ui->loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(ui->signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);
    connect(ui->backToLoginButton, &QPushButton::released, this, &LoginWindow::HandleBackToLoginButton);
    connect(ui->registerButton, &QPushButton::released, this, &LoginWindow::HandleRegisterButton);

    //connect(this, SIGNAL(LoginButtonClicked(User)), mainWindow, SLOT(LoginButtonClicked(User)));
    //this->show();


    QPixmap pix("../login.png");
    ui->labelImage->setPixmap(pix);
}

void LoginWindow::HandleLoginButton()
{
    emit LoginButtonClicked({0, ui->usernameLineEdit->text().toStdString() , ui->passwordLineEdit->text().toStdString() });
}

void LoginWindow::HandleSignUpButton()
{
    bool uniqueUsername = true;
    if (ui->usernameLineEdit->text().size() >= 5 && ui->passwordLineEdit->text().size() >= 5 && ui->confirmPasswordLineEdit->text().size() >= 5)
    {
        for (auto it : mock.GetUsers())
        {
            if (it.GetUsername() == ui->usernameLineEdit->text().toStdString())
            {
                uniqueUsername = false;
                break;
            }
        }

        if (uniqueUsername == true)
        {
            if (ui->passwordLineEdit->text() == ui->confirmPasswordLineEdit->text())
            {
                 QRegularExpression reg("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[a-zA-Z]).{8,}$");
                 QValidator* validator = new QRegularExpressionValidator(reg, this);

                 if (reg.match(ui->passwordLineEdit->text()).hasMatch())
                 {
                     mock.AddUser(User(ui->usernameLineEdit->text().toStdString(), ui->passwordLineEdit->text().toStdString()));
                     QMessageBox::information(this, "Register", "Account created succesfully");
                     HandleBackToLoginButton();
                 }
                 else
                 {
                     QMessageBox::warning(this, "Register", "Password must be min 8 char length, min 1 big letter and 1 number");
                 }


            }
            else
            {
                QMessageBox::warning(this, "Register", "Passwords don't match");
            }
        }
        else
        {
            QMessageBox::warning(this, "Register", "Username already used.");

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