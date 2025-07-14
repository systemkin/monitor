#include "tcpclient.h"
#include <QJsonObject>
#include <QTcpSocket>
#include <QJsonDocument>

tcpClient::tcpClient(QString host, int port) {
    this->host = host;
    this->port = port;
}
void tcpClient::makeJsonRequest(QJsonObject requestObject) {
    QMutexLocker locker(&connectionMutex);
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::connected, this, [socket, requestObject]() {
        QJsonDocument requestDoc(requestObject);
        QByteArray requestData = requestDoc.toJson();
        QByteArray lengthPrefix = QByteArray::number(requestData.size()) + "\n";

        socket->write(lengthPrefix + requestData);
        socket->flush();
    });

    QByteArray *buffer = new QByteArray();
    connect(socket, &QTcpSocket::readyRead, this, [socket, buffer]() {
        buffer->append(socket->readAll());
    });

    connect(socket, &QTcpSocket::disconnected, this, [this, socket, requestObject, buffer]() {

        emit requestCompleted(requestObject, QJsonDocument::fromJson(*buffer));

        socket->deleteLater();
    });

}
