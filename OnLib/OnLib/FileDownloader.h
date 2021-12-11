#pragma once
#include <qobject.h>
#include <qbytearray.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>

class FileDownloader :
    public QObject
{
    Q_OBJECT

public:
    FileDownloader(const std::string& url, QObject* parent = nullptr);
    virtual ~FileDownloader() {};

    QByteArray GetData() const noexcept { return data; }

signals:
    void DownloadFinished();

private slots:
    void FileDownloaded(QNetworkReply* reply);

private:
    QNetworkAccessManager accessManager;
    QByteArray data;
};

