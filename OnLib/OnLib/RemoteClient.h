#pragma once

#include "Networking.h"
#include "../OnLib.DataTransfer/User.h"
#include "Errors.h"
#include <qobject.h>
#include <vector>
#include "ClientRequest.h"

class RemoteClient : public QObject, public net::Client
{
	Q_OBJECT

public:
	RemoteClient(QObject* p);
	~RemoteClient();

public slots:
	void OnLoginRequest(const data::User & user);

signals:
	void LoginSuccessfull();
	void LoginInvalid(const std::vector<data::LogginErrors>& errors);
	void RegisterSuccesfull();
	void RegisterInvalid(const std::vector<data::RegisterErrors>& errors);

private:
	void OnMessageRecieved(net::Message& msg);
	void ProcessMessages(std::size_t maxCount, bool wait);

private:
	std::thread processingThread;
};

