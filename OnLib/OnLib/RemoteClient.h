#pragma once

#include "Networking.h"
#include "../OnLib.DataTransfer/User.h"
#include <qobject.h>
#include <vector>
#include <Book.h>
#include "Enums.h"

class RemoteClient : public QObject, public net::Client
{
	Q_OBJECT

public:
	RemoteClient(QObject* p);
	~RemoteClient();
	
	void Send(const net::Message& msg) const override;

	void RequestDisplayBooks();
	void RequestBorrowedBooks();

public slots:
	void OnLoginRequest(const data::User& user);
	void OnRegisterRequest(const data::User& user);
	void OnLogoutRequest();
	void OnDeleteAccountRequest(const std::string& password);

	void OnRefreshRequest();

	void OnSearchRequest(const std::string& search);
	void OnBookRated(int rating, uint64_t bookId);
	void OnBookBorrowRequest(uint64_t bookId);
	void OnBookReturnRequest(uint64_t bookId);
	void OnBookExtendDateRequest(uint64_t bookId);
signals:
	void ExceptionThrown(const std::exception& e);

	void LoginSuccessfull();
	void LoginInvalid(const std::vector<data::LoginErrors>& errors);

	void RegisterSuccesfull();
	void RegisterInvalid(const std::vector<data::RegisterErrors>& errors);

	void AccountDeleteSuccess();
	void AccountDeleteFailure();

	void ConnectionLost() const;

	void DisplayBooksRecieved(const std::vector<data::Book>& books);
	void BorrowedBooksRecieved(const std::vector<data::LendBook>& books);
	void SearchedBooksRecieved(const std::vector<data::Book>& books);

private:
	void OnMessageRecieved(net::Message& msg);
	void ProcessMessages(std::size_t maxCount, bool wait);

private:
	std::thread processingThread;
};