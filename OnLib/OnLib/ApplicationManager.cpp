#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(std::unique_ptr<RemoteClient> client, QApplication& application)
	: remote(std::move(client)),
	app(application)
{
	connect(remote.get(), &RemoteClient::ConnectionLost, this, &ApplicationManager::OnConnectionLost);

	connect(remote.get(), &RemoteClient::AccountDeleteFailure, this, &ApplicationManager::OnAccountDeleteFailure);
	connect(remote.get(), &RemoteClient::AccountDeleteSuccess, this, &ApplicationManager::OnAccountDeleteSuccess);

	connect(remote.get(), &RemoteClient::ExceptionThrown, [this](const std::exception& e) {
		QMessageBox::critical(nullptr, "Exception in remote client", e.what());
		});
}

void ApplicationManager::Start()
{
	loginWindow = std::make_unique<LoginWindow>();
	ConnectRemoteAndLogin();

	loginWindow->show();
}

void ApplicationManager::OnAccountDeleteSuccess()
{
	SwitchToLoginWindow();
	QMessageBox::information(loginWindow.get(), "Account Removal", "You successfully deleted your account");
}

void ApplicationManager::OnAccountDeleteFailure()
{
	QMessageBox::critical(mainWindow.get(), "Account Removal", "Account deletion failure. You inserted an invalid password");
}

void ApplicationManager::ConnectRemoteAndLogin()
{
	connect(loginWindow.get(), &LoginWindow::LoginButtonClicked, remote.get(), &RemoteClient::OnLoginRequest);
	connect(loginWindow.get(), &LoginWindow::RegisterButtonClicked, remote.get(), &RemoteClient::OnRegisterRequest);
	connect(remote.get(), &RemoteClient::LoginInvalid, loginWindow.get(), &LoginWindow::OnLoginFailure);

	connect(remote.get(), &RemoteClient::RegisterInvalid, loginWindow.get(), &LoginWindow::OnRegisterFailure);
	connect(remote.get(), &RemoteClient::RegisterSuccesfull, loginWindow.get(), &LoginWindow::OnRegisterSuccess);

	connect(remote.get(), &RemoteClient::LoginSuccessfull, loginWindow.get(), [this]() {
		SwitchToMainWindow();
		remote->RequestDisplayBooks();
		remote->RequestBorrowedBooks();
		});
}

void ApplicationManager::ConnectRemoteAndMain()
{
	connect(remote.get(), &RemoteClient::DisplayBooksRecieved, mainWindow.get(), &MainWindow::AddBooksToSection);
	connect(remote.get(), &RemoteClient::BorrowedBooksRecieved, mainWindow.get(), &MainWindow::AddBorrowedBooks);
	connect(remote.get(), &RemoteClient::SearchedBooksRecieved, mainWindow.get(), &MainWindow::AddSearchedBooks);

	connect(mainWindow.get(), &MainWindow::LogOutRequest, [this]() {
		SwitchToLoginWindow();
		remote->OnLogoutRequest();
		});

	connect(mainWindow.get(), &MainWindow::DeleteAccountRequest, remote.get(), &RemoteClient::OnDeleteAccountRequest);
	connect(mainWindow.get(), &MainWindow::SearchRequest, remote.get(), &RemoteClient::OnSearchRequest);
	connect(mainWindow.get(), &MainWindow::RefreshButtonPressed, remote.get(), &RemoteClient::OnRefreshRequest);
	connect(mainWindow.get(), &MainWindow::BookRated, remote.get(), &RemoteClient::OnBookRated);
	connect(mainWindow.get(), &MainWindow::BorrowBookRequest, remote.get(), &RemoteClient::OnBookBorrowRequest);
	connect(mainWindow.get(), &MainWindow::ReturnBookRequest, remote.get(), &RemoteClient::OnBookReturnRequest);
	connect(mainWindow.get(), &MainWindow::ExtendDateRequest, remote.get(), &RemoteClient::OnBookExtendDateRequest);
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