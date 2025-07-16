#ifndef SERVER_H
#define SERVER_H

#include <QThread>
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
#include "refresherlib.h"
#include "serverlib.h"

class Server : public QObject
{
public:
    Server(QObject *parent = nullptr);
    ~Server();
    QThread* refresherThread;
    Refresher* refresher;

    QThread* serviceThread;
    Service* service;

    MonitorDB* dbm;

};

#endif // SERVER_H
