#include "LoginWindow.h"
#include "MainWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    username = new QLineEdit;
    password = new QLineEdit;
    confirmPassword = new QLineEdit;
    
     mainLayout = new QFormLayout;

    mainLayout->addRow(tr("&Username:"), username);
    username->setMaximumWidth(200);

    mainLayout->addRow(tr("&Password:"), password);
    password->setMaximumWidth(200);


    mainLayout->addRow(tr("&Confirm Password:"), confirmPassword);
    confirmPassword->setMaximumWidth(200);
    confirmPassword->hide();
    mainLayout->labelForField(confirmPassword)->hide();
    

    loginButton = new QPushButton("Log in", this);
    loginButton->setText("Log in");
    loginButton->setMaximumWidth(200);
    mainLayout->addWidget(loginButton);

    signupButton = new QPushButton("Sign up", this);
    signupButton->setText("Sign up");
    signupButton->setMaximumWidth(200);
    mainLayout->addWidget(signupButton);

    QWidget* window = new QWidget();
    mainLayout->setFormAlignment(Qt::AlignCenter);
    window->setLayout(mainLayout);
    window->setFixedSize(500, 400);
    window->setWindowTitle("Online library");


    connect(loginButton, &QPushButton::released, this, &LoginWindow::HandleLoginButton);
    connect(signupButton, &QPushButton::released, this, &LoginWindow::HandleSignUpButton);


    window->show();
}

void LoginWindow::HandleLoginButton()
{
    //User user(username->text().toStdString(), password->text().toStdString());

    if (username->text().size()>4 && password->text().size()>4)
    {
        MainWindow* mainWindow = new MainWindow;
        mainWindow->show();
    }
    else
    {

    }



}

void LoginWindow::HandleSignUpButton()
{
    loginButton->hide();
    confirmPassword->show();
    mainLayout->labelForField(confirmPassword)->show();

}
