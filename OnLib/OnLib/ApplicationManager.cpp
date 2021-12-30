#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(std::unique_ptr<RemoteClient> client, QApplication& application)
	: remote(std::move(client)),
	app(application)
{
	QObject::connect(remote.get(), &RemoteClient::ConnectionLost, this, &ApplicationManager::OnConnectionLost);
}

void ApplicationManager::Start()
{
	loginWindow = std::make_unique<LoginWindow>();
	ConnectRemoteAndLogin();

	loginWindow->show();
}

void ApplicationManager::ConnectRemoteAndLogin()
{
	QObject::connect(loginWindow.get(), &LoginWindow::LoginButtonClicked, remote.get(), &RemoteClient::OnLoginRequest);
	QObject::connect(loginWindow.get(), &LoginWindow::RegisterButtonClicked, remote.get(), &RemoteClient::OnRegisterRequest);
	QObject::connect(remote.get(), &RemoteClient::LoginInvalid, loginWindow.get(), &LoginWindow::OnLoginFailure);

	QObject::connect(remote.get(), &RemoteClient::LoginSuccessfull, loginWindow.get(), [this]() {
		SwitchToMainWindow();
		remote->RequestDisplayBooks();
		});

	QObject::connect(remote.get(), &RemoteClient::RegisterInvalid, loginWindow.get(), &LoginWindow::OnRegisterFailure);
	QObject::connect(remote.get(), &RemoteClient::RegisterSuccesfull, loginWindow.get(), &LoginWindow::OnRegisterSuccess);
}

void ApplicationManager::ConnectRemoteAndMain()
{
	QObject::connect(remote.get(), &RemoteClient::DisplayBooksRecieved, mainWindow.get(), &MainWindow::AddBooksToSection);
	QObject::connect(remote.get(), &RemoteClient::BorrowedBooksRecieved, mainWindow.get(), &MainWindow::AddBorrowedBooks);
}

void ApplicationManager::SwitchToMainWindow()
{
	loginWindow->hide();
	loginWindow.reset();

	mainWindow = std::make_unique<MainWindow>();
	ConnectRemoteAndMain();
	mainWindow->showMaximized();
}

void ApplicationManager::SwitchToLoginWindow()
{
	mainWindow->hide();
	mainWindow.reset();

	loginWindow = std::make_unique<LoginWindow>();
	ConnectRemoteAndLogin();

	loginWindow->show();
}

void ApplicationManager::OnConnectionLost()
{
	auto reply = QMessageBox::critical(nullptr, "Server connection", "Connection lost!");
	app.quit();
	std::exit(0);
}