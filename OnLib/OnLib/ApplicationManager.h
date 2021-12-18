#pragma once
#include <qobject.h>
#include <QtWidgets/qmessagebox.h>

#include "MainWindow.h"
#include "LoginWindow.h"
#include "RemoteClient.h"

class ApplicationManager :
    public QObject
{
    Q_OBJECT

public:
    ApplicationManager(std::unique_ptr<RemoteClient> client, QApplication& application);
    void Start();

public slots:
    void OnConnectionLost();

private:
    void ConnectRemoteAndLogin();
    void ConnectRemoteAndMain();

    void SwitchToMainWindow();
    void SwitchToLoginWindow();
private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<LoginWindow> loginWindow;
    std::unique_ptr<RemoteClient> remote;
    QApplication& app;
};

