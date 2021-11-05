#include "LoginWindow.h"
#include "MainWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

    window = new QWidget();
    ui.setupUi(window);

    ui.confirmPasswordLabel->hide();
    ui.confirmPassword->hide();
    ui.backToLoginButton->hide();

    connect(ui.loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(ui.signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);
    connect(ui.backToLoginButton, &QPushButton::released, this, &LoginWindow::HandleBackToLoginButton);

    window->show();
}

void LoginWindow::HandleLoginButton()
{
    //User user(username->text().toStdString(), password->text().toStdString());

    if (ui.username->text().size() > 4 && ui.password->text().size() > 4)
    {
        MainWindow* mainWindow = new MainWindow;
        mainWindow->show();
        window->hide();
    }
    else
    {

    }
}

void LoginWindow::HandleSignUpButton()
{
    ui.loginButton->hide();
    ui.confirmPassword->show();
    ui.backToLoginButton->show();
    ui.mainLayout->labelForField(ui.confirmPassword)->show();
}

void LoginWindow::HandleBackToLoginButton()
{
    ui.confirmPassword->hide();
    ui.loginButton->show();
    ui.backToLoginButton->hide();
    ui.confirmPasswordLabel->hide();
    ui.username->clear();
    ui.password->clear();
    ui.confirmPassword->clear();
}

LoginWindow::~LoginWindow()
{
}
