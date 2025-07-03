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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QFile>
#include <QTextStream>

class Server : public QTcpServer
{
public:
    Server(QObject *parent = nullptr);
private:
    int getState(QString Serial);
    QFile *file;
    void respond(QTcpSocket *clientSocket, QJsonObject responseObj);
    void refreshStates();
    QSqlDatabase db;
private slots:
    void onNewConnection();

};

#endif // SERVER_H
