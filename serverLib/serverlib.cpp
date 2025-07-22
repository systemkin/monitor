#include "serverlib.h"
#include <monitor_db.h>

#include <QDomDocument>
#include "db_repository.h"
#include "device.h"

Service::Service(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &Service::onNewConnection);
}

void Service::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    QByteArray *buffer = new QByteArray();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket, buffer]() {

        buffer->append(clientSocket->readAll());
        while (true) {

            int newlineIndex = buffer->indexOf('\n');
            if (newlineIndex == -1) break;

            QByteArray lengthPrefix = buffer->left(newlineIndex);
            int messageLength = lengthPrefix.toInt();
            if (buffer->size() < newlineIndex + 1 + messageLength) break;

            QByteArray messageData = buffer->mid(newlineIndex + 1, messageLength);
            buffer->clear();
            QJsonObject requestObject = QJsonDocument::fromJson(messageData).object();

            if (requestObject["requestType"] == "getStates") {
                QJsonObject result = db_repository::getInstance()->getStates();
                QJsonDocument responseDoc(result);
                qDebug() << "responded: " << result;
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "addDevice") {
                device device(requestObject["requestData"].toObject());
                QJsonObject result = db_repository::getInstance()->addDevice(device);
                QJsonDocument responseDoc(result);
                qDebug() << "responded: " << result;
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "editDevice") {
                QJsonObject requestData = requestObject["requestData"].toObject();
                int id = requestData.value("id").toInt();
                device device(requestData.value("device").toObject());
                QJsonObject result = db_repository::getInstance()->editDevice(id, device);
                QJsonDocument responseDoc(result);
                qDebug() << "responded: " << result;
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "changeDevices") {
                QJsonArray jsonArray = requestObject["requestData"].toArray();
                std::vector<device> devices(jsonArray.size());
                int i = 0;
                for (const QJsonValue &value : std::as_const(jsonArray)) {
                    devices[i++] = device(value.toObject());
                }
                QJsonObject result = db_repository::getInstance()->changeDevices(devices);
                qDebug() << "responded: " << result;
                respond(clientSocket, QJsonDocument(result));

            } else if (requestObject["requestType"] == "showHistory") {
                QJsonObject logRequestObject = requestObject;
                QJsonObject result = db_repository::getInstance()->getLog(logRequestObject);
                qDebug() << "responded: " << result;
                QJsonDocument responseDoc(result);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "deleteDevice") {
                QJsonObject requestData = requestObject["requestData"].toObject();
                int id = requestData["id"].toInt();
                QJsonObject result = db_repository::getInstance()->deleteDevice(id);
                QJsonDocument responseDoc(result);
                qDebug() << "responded: " << result;
                respond(clientSocket, responseDoc);
            }
        }
    });
}

void Service::respond(QTcpSocket *clientSocket, QJsonDocument responseDoc) {
    QByteArray responseData = responseDoc.toJson();

    const int chunkSize = 1024;
    for (int i = 0; i < responseData.size(); i += chunkSize) {
        QByteArray chunk = responseData.mid(i, chunkSize);
        clientSocket->write(chunk);
        clientSocket->waitForBytesWritten();
    }
    clientSocket->disconnectFromHost();
}
