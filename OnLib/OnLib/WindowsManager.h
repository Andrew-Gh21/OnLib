#pragma once
#include <qobject.h>

#include "MainWindow.h"
#include "LoginWindow.h"
#include "RemoteClient.h"

class WindowsManager :
    public QObject
{
    Q_OBJECT

public:
    WindowsManager(std::unique_ptr<RemoteClient> client);
    void Start();

private:
    void ConnectRemoteAndLogin();
    void ConnectRemoteAndMain();

    void SwitchToMainWindow();
    void SwitchToLoginWindow();
private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<LoginWindow> loginWindow;
    std::unique_ptr<RemoteClient> remote;
};

