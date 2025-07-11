#ifndef TCPCOMMUNICATOR_H
#define TCPCOMMUNICATOR_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class tcpCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit tcpCommunicator(QObject *parent = nullptr);
    tcpCommunicator(const QString &host, int port, QObject *parent = nullptr);
    void makeJsonRequest(const QJsonObject &requestObject);

signals:
    void requestCompleted(QJsonObject request, QJsonDocument response);

private:
    QString host;
    int port;
};

#endif // TCPCOMMUNICATOR_H
