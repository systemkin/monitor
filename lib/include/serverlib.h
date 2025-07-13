#ifndef SERVERLIB_H
#define SERVERLIB_H

#include "serverLib_global.h"
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


class SERVERLIB_EXPORT Service :  public QTcpServer
{
public:
    Service (MonitorDB* dbm, QObject *parent = nullptr);
private:
    int getState(QString Serial);
    void respond(QTcpSocket *clientSocket, QJsonDocument responseDoc);
    void refreshStates();
    QSqlDatabase db;
    MonitorDB* dbm;

private slots:
    void onNewConnection();

};

#endif // SERVERLIB_H
