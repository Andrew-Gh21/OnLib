#include "RemoteClient.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include "WindowsManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<RemoteClient> remote = std::make_unique<RemoteClient>(nullptr);
    remote->Connect("127.0.0.1", 6000);

    WindowsManager manager(std::move(remote));
    manager.Start();

    return app.exec();
}