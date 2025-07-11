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
#include "monitor_db.h"
#include "refresher.h"

class Server : public QTcpServer
{
public:
    Server(QObject *parent = nullptr);
private:
    int getState(QString Serial);
    QFile *file;
    void respond(QTcpSocket *clientSocket, QJsonDocument responseDoc);
    void refreshStates();
    QSqlDatabase db;
    MonitorDB* dbm;
    QThread* refresherThread;
    Refresher* refresher;
private slots:
    void onNewConnection();

};

#endif // SERVER_H
