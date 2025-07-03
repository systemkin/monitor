#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>


class Server : public QTcpServer
{
public:
    Server(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void test(QTcpSocket *clientSocket);
    QJsonArray generateJson();


};

#endif // SERVER_H
