#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("monitorDB");
    db.setUserName("postgres");
    db.setPassword("root");
    db.setPort(5432);


    if (!db.open()) {
        //qDebug() << "Failed to connect: " << db.lastError().text();
        return;
    }
    file = new QFile("C:\\Users\\alexej\\Documents\\monitor\\MonitorService\\data.txt");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug() << "No such data file or Error during opening" << file->errorString();
        return;
    }
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Server::refreshStates);
    timer->start(10000);

}
int Server::getState(QString serial) {
    QTextStream in(file);
    file->seek(0);
    //qDebug() << "start seeking in file";
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList stringList = line.split(" ");
        //qDebug() << "serial = " + stringList[0];
        //qDebug() << "seeks = " + serial;
        if (stringList[0] == serial) {
            return stringList[1].toInt();
        }
    } return -1;
}
void Server::refreshStates() {
    //qDebug() << "start1";
    db.transaction();
    QSqlQuery query;
    if (!query.exec("SELECT devices.id, devices.serial, states.state FROM devices INNER JOIN states ON devices.id = states.device_id")) {
        //qDebug() << "Unexpected Error (while: Execute query).";
        return;
    }

    QSqlQuery updateStatesQuery(db);
    updateStatesQuery.prepare("UPDATE states SET state = (?) WHERE device_id = (?)");
    //qDebug() << "start";
    while (query.next()) {
        //qDebug() << "Cycle";
        int currState = getState(query.value(1).toString());
        //qDebug() << currState;
        //qDebug() << query.value(2).toInt();
        if (currState != query.value(2)) {
            //qDebug() << "not Equals";
            updateStatesQuery.addBindValue(currState);
            updateStatesQuery.addBindValue(query.value(0).toString());
            updateStatesQuery.exec();
        }
    }
    db.commit();
}


void Server::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    QByteArray *buffer = new QByteArray();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket, buffer]() {

        buffer->append(clientSocket->readAll());
        while (true) {

            int newlineIndex = buffer->indexOf('\n');
            if (newlineIndex == -1) {
                break;
            }

            QByteArray lengthPrefix = buffer->left(newlineIndex);

            int messageLength = lengthPrefix.toInt();

            if (buffer->size() < newlineIndex + 1 + messageLength) {
                break;
            }

            QByteArray messageData = buffer->mid(newlineIndex + 1, messageLength);
            buffer->clear();
            QJsonObject requestObject = QJsonDocument::fromJson(messageData).object();


            if (requestObject["requestType"] == "getStates") {
                QSqlQuery query;
                if (!query.exec("SELECT * FROM devices")) {
                    //qDebug() << "Unexpected Error (while: Execute query).";
                return;
                }
                QString value;
                while (query.next()) {
                    value = query.value(2).toString();
                }
                QJsonObject response;

                response["data"] = value;

                respond(clientSocket, response);
            } else if (requestObject["requestType"] == "addDevice") {


            } else if (requestObject["requestType"] == "editDevice") {


            } else if (requestObject["requestType"] == "deleteDevice") {


            } else if (requestObject["requestType"] == "changeDevices") {


            } else if (requestObject["requestType"] == "showHistory") {


            }

        }

    });

}

void Server::respond(QTcpSocket *clientSocket, QJsonObject responseObj) {

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
