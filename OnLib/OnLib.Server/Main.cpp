#include "RemoteServer.h"
#include <string>
#include <iostream>
#include "User.h"

int main()
{
	net::Message msg;
	data::User user{ 1,"test", "pass" };
	data::User result;

	data::User::Serialize(msg,user);
	data::User::Deserialize(msg, result);

	RemoteServer server(6000);
	server.Start();

	while (true)
	{
		server.ProcessMessages(-1, true);
	}
}