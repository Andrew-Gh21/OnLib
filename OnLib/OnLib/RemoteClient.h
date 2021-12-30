#pragma once

#include "Networking.h"
#include "../OnLib.DataTransfer/User.h"
#include "Errors.h"
#include <qobject.h>
#include <vector>
#include "ClientRequest.h"
#include <Book.h>

class RemoteClient : public QObject, public net::Client
{
	Q_OBJECT

public:
	RemoteClient(QObject* p);
	~RemoteClient();

	void RequestDisplayBooks();
	void RequestBorrowedBooks();
	void Send(const net::Message& msg) const override;
public slots:
	void OnLoginRequest(const data::User& user);
	void OnRegisterRequest(const data::User& user);

signals:
	void LoginSuccessfull();
	void LoginInvalid(const std::vector<data::LoginErrors>& errors);
	void RegisterSuccesfull();
	void RegisterInvalid(const std::vector<data::RegisterErrors>& errors);
	void ConnectionLost() const;

	void DisplayBooksRecieved(const std::vector<data::Book>& books);
	void BorrowedBooksRecieved(const std::vector<data::LendBook>& books);
private:
	void OnMessageRecieved(net::Message& msg);
	void ProcessMessages(std::size_t maxCount, bool wait);

private:
	std::thread processingThread;
};