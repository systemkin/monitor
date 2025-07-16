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
#include "historyItem.h"
#include "deviceInfo.h"

/*!
 * \brief class for mamging incloming connections on service. Recieves request, serves it and responds
 */
class SERVERLIB_EXPORT Service :  public QTcpServer
{
public:
    Service (QObject *parent = nullptr);
private:
    /*!
     * \brief function for responding and disconnecting from host
     * \param QTcpSocket with active connection
     * \param JSON-document for respond
     */
    void respond(QTcpSocket *clientSocket, QJsonDocument responseDoc);

private slots:
    /*!
     * \brief slot, that connects events of tcp-connection with lambda, that processes requests
     */
    void onNewConnection();

};

#endif // SERVERLIB_H
