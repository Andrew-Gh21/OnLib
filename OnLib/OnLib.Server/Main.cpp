#include "RemoteServer.h"
#include <string>
#include <iostream>
#include "OStreamLogger.h"

int main()
{
	ILogger* logger = new OStreamLogger(std::cout);
	MultiLogger multiLogger;
	multiLogger.AddLogger(logger);

	sqlite::database db("database.db");

	RemoteServer server(6000, multiLogger, db);
	server.Start();

	while (true)
	{
		server.ProcessMessages(-1, true);
	}
}