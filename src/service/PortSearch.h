#ifndef PORTSEARCH_H
#define PORTSEARCH_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>

struct PortData {
    int id;
    QString country;
    QString port;
    double latitude;
    double longitude;
};

class PortSearch : public QObject
{
    Q_OBJECT
public:
    explicit PortSearch(QObject *parent = nullptr);

    void searchPort(const QString &searchText);

signals:
    void searchFinished(const QVector<PortData> &ports);
    void searchError(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
};

#endif // PORTSEARCH_H
