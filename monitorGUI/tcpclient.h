#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QMutex>

class tcpClient : public QObject
{
    Q_OBJECT
public:
    tcpClient(QString host, int port);
    void makeJsonRequest(QJsonObject requestObject);
private:
    QString host;
    int port;
    QMutex connectionMutex;
signals:
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);

};

#endif // TCPCLIENT_H
