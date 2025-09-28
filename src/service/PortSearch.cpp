#include "PortSearch.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

PortSearch::PortSearch(QObject *parent)
    : QObject(parent),
    m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &PortSearch::onReplyFinished);
}

void PortSearch::searchPort(const QString &searchText)
{
    QString url = QString("https://seaway-api.heyrend.cloud/api/v1/public/ports?port=%1")
    .arg(searchText);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_networkManager->get(request);
}

void PortSearch::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit searchError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &jsonError);

    if (jsonError.error != QJsonParseError::NoError || !doc.isObject()) {
        emit searchError("Invalid JSON response");
        return;
    }

    QJsonObject obj = doc.object();
    if (!obj.contains("data") || !obj["data"].isArray()) {
        emit searchError("Response format error");
        return;
    }

    QVector<PortData> ports;
    QJsonArray arr = obj["data"].toArray();
    for (const QJsonValue &val : arr) {
        if (!val.isObject()) continue;
        QJsonObject portObj = val.toObject();

        PortData port;
        port.id = portObj["id"].toInt();
        port.country = portObj["country"].toString();
        port.port = portObj["port"].toString();
        port.latitude = portObj["latitude"].toDouble();
        port.longitude = portObj["longitude"].toDouble();

        ports.append(port);
    }

    emit searchFinished(ports);
}
