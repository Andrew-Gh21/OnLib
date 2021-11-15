#pragma once

#include "Networking.h"
#include "User.h"
#include "../OnLib.DataTransfer/User.h"
#include <qobject.h>
#include "ClientRequest.h"

class RemoteClient : public QObject, public net::Client
{
	Q_OBJECT

public:
	RemoteClient(QObject* p);

public slots:
	void OnLoginRequest(const data::User & user);
};

