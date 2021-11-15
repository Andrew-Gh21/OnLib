#include "RemoteClient.h"
#include <QtWidgets>
#include "LoginWindow.h"
//#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    RemoteClient client(nullptr);
    client.Connect("127.0.0.1", 6000);
    LoginWindow window;
    QObject::connect(&window, &LoginWindow::LoginButtonClicked, &client, &RemoteClient::OnLoginRequest);
    window.show();

    return app.exec();
}