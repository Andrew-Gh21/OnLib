#include "RemoteClient.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include "ApplicationManager.h"

#include <fstream>
#include <filesystem>
#include <qmessagebox.h>
#include "../../Include/Json/json11.cpp"

void CreateBasicConfig()
{
	std::ofstream file("appconfig.json");
	file << "{\"server_port\": 6000,\"server_ip\" : \"127.0.0.1\"}";
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

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
	std::string ip;
	uint16_t port;

	try
	{
		json11::Json appConfig = GetAppConfig();
		ip = appConfig["server_ip"].string_value();
		port = appConfig["server_port"].int_value();
	}
	catch (std::exception& e)
	{
		QMessageBox::critical(nullptr, "Application config", "Could not read config file");
		return 0;
	}

    std::unique_ptr<RemoteClient> remote = std::make_unique<RemoteClient>(nullptr);
    remote->Connect(ip, port);

    ApplicationManager manager(std::move(remote), app);
    manager.Start();

	app.exit(app.exec());
	return 0;
}