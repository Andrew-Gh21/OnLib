#include "FileDownloader.h"

FileDownloader::FileDownloader(const std::string& url, QObject* parent)
	: QObject(parent)
{
	connect(&accessManager, &QNetworkAccessManager::finished, this, &FileDownloader::FileDownloaded);

	QNetworkRequest request(QUrl(QString::fromStdString(url)));
	accessManager.get(request);
}

void FileDownloader::FileDownloaded(QNetworkReply* reply)
{
	data = reply->readAll();
	reply->deleteLater();
	emit DownloadFinished();
}