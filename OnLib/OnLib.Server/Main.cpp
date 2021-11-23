#include "RemoteServer.h"
#include <string>
#include <iostream>
#include "OStreamLogger.h"
#include "../../Include/Json/json11.cpp"

#include <fstream>

std::string ReadAppConfig()
{
	std::ifstream cfg("appconfig.json");
	std::stringstream buffer;
	buffer << cfg.rdbuf();

	return buffer.str();
}

json11::Json GetAppConfig()
{
	std::string error;
	json11::Json config = json11::Json::parse(ReadAppConfig(), error);

	if (!error.empty())
		throw std::exception("Could not parse appconfig.json");

	return config;
}

int main()
{
	json11::Json appConfig = GetAppConfig();
	std::ofstream logFile(appConfig["log_file"].string_value());
	std::string databasePath = appConfig["database"].string_value();
	uint16_t port = appConfig["server_port"].int_value();

	ILogger* consoleLogger = new OStreamLogger(std::cout);
	ILogger* fileLogger = new OStreamLogger(logFile);
	MultiLogger multiLogger;
	multiLogger.AddLogger(consoleLogger);
	multiLogger.AddLogger(fileLogger);

	sqlite::database db(databasePath);

	RemoteServer server(port, multiLogger, db);
	server.Start();

	while (true)
	{
		server.ProcessMessages(-1, true);
	}

	return 0;
}