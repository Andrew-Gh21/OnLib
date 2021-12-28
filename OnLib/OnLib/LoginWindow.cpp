#include "LoginWindow.h"
#include "MainWindow.h"
#include <QMessageBox>
#include<QRegularExpressionValidator>
#include <sstream>
#include <iostream>

std::unordered_map<data::LoginErrors, std::string> LoginWindow::loginErrors
{
    {data::LoginErrors::InvalidPassword, "Password is invalid"},
    {data::LoginErrors::InvalidUser, "Username is invalid"},
    {data::LoginErrors::UserAlreadyConnected, "This account is already logged in"}
};

std::unordered_map<data::RegisterErrors, std::string> LoginWindow::registerErrors
{
    {data::RegisterErrors::InvalidUsername, "Username must have at least 5 characters"},
    {data::RegisterErrors::InvalidPassword, "Password must be min 8 char length, min 1 big letter and 1 number"},
    {data::RegisterErrors::UsernameAlreadyExists, "Username taken. Please choose another one"},
    {data::RegisterErrors::EmptyField, "Please fill all the fields before registering"},
    {data::RegisterErrors::PassConfirmMismatch, "Password confirmation is not the same as the password"}
};

void LoginWindow::OnLoginFailure(const std::vector<data::LoginErrors>& errors)
{
    std::stringstream errorsStream;
    for (auto error : errors)
        errorsStream << loginErrors[error] << '\n';

    QMessageBox::warning(this, "Login Error", QString::fromStdString(errorsStream.str()));
}

void LoginWindow::OnRegisterSuccess()
{
    QMessageBox::information(this, "Register Successfull", "You have successfully registered.");
    HandleBackToLoginButton();
}

void LoginWindow::OnRegisterFailure(const std::vector<data::RegisterErrors>& errors)
{
    std::stringstream errorsStream;
    for (auto error : errors)
        errorsStream << registerErrors[error] << '\n';

    QMessageBox::warning(this, "Register Error", QString::fromStdString(errorsStream.str()));
}

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QIcon icon("Images/online-library.png");
    LoginWindow::setWindowIcon(icon);
    LoginWindow::setWindowTitle("Online library - Account");

    this->centralWidget()->setStyleSheet("background-image:url(Images/login_background.jpg); background-position: center;");
    this->setFixedSize(900, 600);

    ui->confirmPasswordLabel->hide();
    ui->confirmPasswordLineEdit->hide();
    ui->backToLoginButton->hide();
    ui->signupButton->hide();

    connect(ui->loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(ui->signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);
    connect(ui->backToLoginButton, &QPushButton::released, this, &LoginWindow::HandleBackToLoginButton);
    connect(ui->registerButton, &QPushButton::released, this, &LoginWindow::HandleRegisterButton);
}

void LoginWindow::HandleLoginButton()
{
    std::string user = ui->usernameLineEdit->text().toStdString();
    std::string pass = ui->passwordLineEdit->text().toStdString();

    if (user.empty() || pass.empty())
    {
        QMessageBox::warning(this, "Failure", "Please complete all the fields!");
        return;
    }

    data::User dataUser{ uint64_t(0), user, pass };

    emit LoginButtonClicked(dataUser);
}

void LoginWindow::HandleSignUpButton()
{
    std::string user = ui->usernameLineEdit->text().toStdString();
    std::string pass = ui->passwordLineEdit->text().toStdString();
    std::string confirm = ui->confirmPasswordLineEdit->text().toStdString();

    std::vector<data::RegisterErrors> errors;

    if (user.size() < 5)
        errors.push_back(data::RegisterErrors::InvalidUsername);

    if (pass != confirm)
        errors.push_back(data::RegisterErrors::PassConfirmMismatch);

    if (user.empty() || pass.empty() || confirm.empty())
        errors.push_back(data::RegisterErrors::EmptyField);

    if (QRegularExpression reg("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[a-zA-Z]).{8,}$");
        !reg.match(QString::fromStdString(pass)).hasMatch())
        errors.push_back(data::RegisterErrors::InvalidPassword);

    if (errors.empty())
        emit RegisterButtonClicked({ uint64_t(0), user, pass });
    else
        OnRegisterFailure(errors);
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