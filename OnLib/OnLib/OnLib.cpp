#include "RemoteClient.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include "WindowsManager.h"

//void makeConnections(RemoteClient& client, LoginWindow& window, MainWindow& mainWindow)
//{
//    // Client <-> Login/Register
//    QObject::connect(&window, &LoginWindow::LoginButtonClicked, &client, &RemoteClient::OnLoginRequest);
//    QObject::connect(&window, &LoginWindow::RegisterButtonClicked, &client, &RemoteClient::OnRegisterRequest);
//    QObject::connect(&client, &RemoteClient::LoginInvalid, &window, &LoginWindow::OnLoginFailure);
//    QObject::connect(&client, &RemoteClient::LoginSuccessfull, &window, &LoginWindow::OnLoginSucces);
//    QObject::connect(&client, &RemoteClient::RegisterInvalid, &window, &LoginWindow::OnRegisterFailure);
//    QObject::connect(&client, &RemoteClient::RegisterSuccesfull, &window, &LoginWindow::OnRegisterSuccess);
//    // Client <-> Main
//    QObject::connect(&client, &RemoteClient::DisplayBooksRecieved, &mainWindow, &MainWindow::AddBooksToSection);
//}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<RemoteClient> remote = std::make_unique<RemoteClient>(nullptr);
    remote->Connect("127.0.0.1", 6000);

    WindowsManager manager(std::move(remote));
    manager.Start();

    /*MainWindow* mainWindow = new MainWindow;
    LoginWindow* window = new LoginWindow(mainWindow);

    RemoteClient client(nullptr);
    client.Connect("127.0.0.1", 6000);*/
    
    //makeConnections(client, *window, *mainWindow);
    
    /*window->show();*/

    return app.exec();
}