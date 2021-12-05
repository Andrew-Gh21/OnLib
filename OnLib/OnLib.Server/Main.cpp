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
	net::Message test;
	std::vector<data::User> testUsers{ {0, "Stefan", "pass"}, {1, "stefan2", "pass2"} };
	std::vector<data::User> resultUsers;

	std::vector<data::LoginErrors> testErrors{ data::LoginErrors::InvalidPassword, data::LoginErrors::InvalidUser };
	std::vector<data::LoginErrors> result;

	net::Serialize(test, std::cbegin(testErrors), std::cend(testErrors));
	net::Deserialize(test, result, true);

	net::Serialize(test, std::cbegin(testUsers), std::cend(testUsers));
	net::Deserialize(test, resultUsers, true);

	json11::Json appConfig = GetAppConfig();
	std::ofstream logFile(appConfig["log_file"].string_value(), std::fstream::app);
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