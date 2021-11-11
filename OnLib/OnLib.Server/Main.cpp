#include "RemoteServer.h"
#include <string>
#include <iostream>

int main()
{
	std::string test = "asdasdasdas";
	std::string out;
	std::vector<int> vecTest = { 1, 2, 3, 4 };
	std::vector<int> outVect;
	net::Message msg;

	msg << test;
	msg << vecTest;

	msg >> outVect;
	msg >> out;

	RemoteServer server(6000);
	server.Start();

	while (true)
	{
		server.ProcessMessages(-1, true);
	}
}