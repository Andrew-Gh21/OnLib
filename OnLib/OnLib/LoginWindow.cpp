#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    username = new QLineEdit;
    password = new QLineEdit;
    confirmPassword = new QLineEdit;
    
    QFormLayout* mainLayout = new QFormLayout;

    mainLayout->addRow(tr("&Username:"), username);
    username->setMaximumWidth(200);

    mainLayout->addRow(tr("&Password:"), password);
    password->setMaximumWidth(200);

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

    window->show();
}