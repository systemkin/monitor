#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
}
void Server::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    test(clientSocket);
}

void Server::test(QTcpSocket *clientSocket) {
    QJsonObject responseObj;
    responseObj["status"] = "success";
    responseObj["data"] = generateJson();

    QJsonDocument responseDoc(responseObj);
    QByteArray responseData = responseDoc.toJson();

    const int chunkSize = 1024;
    for (int i = 0; i < responseData.size(); i += chunkSize) {
        QByteArray chunk = responseData.mid(i, chunkSize);
        clientSocket->write(chunk);
        clientSocket->waitForBytesWritten();
    }
    clientSocket->disconnectFromHost();
}

QJsonArray Server::generateJson() {
    QJsonArray largeDataArray;
    for (int i = 0; i < 1000; ++i) {
        QJsonObject item;
        item["id"] = i;
        item["value"] = QString("Value %1").arg(i);
        largeDataArray.append(item);
    }
    return largeDataArray;
}
