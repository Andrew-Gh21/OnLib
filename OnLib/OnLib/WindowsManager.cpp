#include "WindowsManager.h"

WindowsManager::WindowsManager(std::unique_ptr<RemoteClient> client)
	: remote(std::move(client))
{
}

void WindowsManager::Start()
{
    loginWindow = std::make_unique<LoginWindow>();
    ConnectRemoteAndLogin();

    loginWindow->show();
}

void WindowsManager::ConnectRemoteAndLogin() const
{
    QObject::connect(loginWindow.get(), &LoginWindow::LoginButtonClicked,       remote.get(),       &RemoteClient::OnLoginRequest);
    QObject::connect(loginWindow.get(), &LoginWindow::RegisterButtonClicked,    remote.get() ,      &RemoteClient::OnRegisterRequest);
    QObject::connect(remote.get(),      &RemoteClient::LoginInvalid,            loginWindow.get(),  &LoginWindow::OnLoginFailure);
    QObject::connect(remote.get(),      &RemoteClient::LoginSuccessfull,        loginWindow.get(),  &LoginWindow::OnLoginSucces);
    QObject::connect(remote.get(),      &RemoteClient::RegisterInvalid,         loginWindow.get(),  &LoginWindow::OnRegisterFailure);
    QObject::connect(remote.get(),      &RemoteClient::RegisterSuccesfull,      loginWindow.get(),  &LoginWindow::OnRegisterSuccess);
}

void WindowsManager::ConnectRemoteAndMain() const
{
    QObject::connect(remote.get(), &RemoteClient::DisplayBooksRecieved, mainWindow.get(), &MainWindow::AddBooksToSection);
}
