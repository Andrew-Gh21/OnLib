#include "RemoteServer.h"
#include <string>
#include <iostream>
#include "OStreamLogger.h"
#include <filesystem>
#include "../../Include/Json/json11.cpp"

#include <fstream>

void CreateBasicConfig()
{
	std::ofstream file("appconfig.json");
	file << "{\"server_port\": 6000,\"database\" : \"database.db\",\"log_file\" : \"log.txt\"}";
	file.close();
}

std::string ReadAppConfig()
{
	if (!std::filesystem::exists("appconfig.json"))
		CreateBasicConfig();

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
	std::ofstream logFile(appConfig["log_file"].string_value(), std::fstream::app);
	std::string databasePath = appConfig["database"].string_value();
	uint16_t port = appConfig["server_port"].int_value();

	std::shared_ptr<ILogger> consoleLogger = std::make_shared<OStreamLogger>(std::cout);
	std::shared_ptr<ILogger> fileLogger = std::make_shared<OStreamLogger>(logFile);
	MultiLogger multiLogger;
	multiLogger.AddLogger(consoleLogger);
	multiLogger.AddLogger(fileLogger);

	sqlite::database db(databasePath);

	RemoteServer server(port, multiLogger, db);
	server.Start();

	while (true)
	{
		try
		{
			server.ProcessMessages(-1, true);
		}
		catch (std::exception& e)
		{
			LogMessage msg;
			msg << e.what();
			multiLogger.Send(msg, LogSeverity::Crititical);
		}
	}

	return 0;
}