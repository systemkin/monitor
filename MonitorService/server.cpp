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
        qDebug() << "Failed to connect: " << db.lastError().text();
        return;
    }
    file = new QFile("C:\\Users\\alexej\\Documents\\monitor\\MonitorService\\data.txt");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No such data file or Error during opening" << file->errorString();
        return;
    }
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Server::refreshStates);
    timer->start(10000);

}
int Server::getState(QString serial) {
    QTextStream in(file);
    file->seek(0);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList stringList = line.split(" ");
        if (stringList[0] == serial) {
            return stringList[1].toInt();
        }
    } return -1;
}
void Server::refreshStates() {

    db.transaction();
    QSqlQuery query;
    if (!query.exec("SELECT devices.id, devices.serial, states.state FROM devices LEFT JOIN states ON devices.id = states.device_id")) {
        qDebug() << "Unexpected Error (while: Execute query).";
        return;
    }

    QSqlQuery updateStatesQuery(db);
    updateStatesQuery.prepare("UPDATE states SET state = (?) WHERE device_id = (?)");
    while (query.next()) {
        int currState = getState(query.value(1).toString());
        if (currState != query.value(2)) {
            updateStatesQuery.addBindValue(currState);
            updateStatesQuery.addBindValue(query.value(0).toInt());
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

                QJsonObject response;
                QJsonArray jsonArray;

                QSqlQuery query;
                if (!query.exec("SELECT devices.serial, devices.name, devices.description, devices.type, states.state, devices.id FROM states INNER JOIN devices ON devices.id = device_id")) {
                    qDebug() << "Unexpected Error (while: Execute query).";
                    response["status"] = "fail";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }


                while (query.next()) {
                    QJsonObject jsonObject;
                    jsonObject["serial"] = query.value(0).toString();
                    jsonObject["name"] = query.value(1).toString();
                    jsonObject["description"] = query.value(2).toString();
                    jsonObject["type"] = query.value(3).toBool();
                    jsonObject["state"] = query.value(4).toString();
                    jsonObject["id"] = query.value(5).toInt();
                    jsonArray.append(jsonObject);
                }

                response["status"] = "success";
                response["data"] = jsonArray;

                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);
            } else if (requestObject["requestType"] == "addDevice") {
                QJsonObject response;

                QJsonObject requestData = requestObject["requestData"].toObject();
                QString serial = requestData["serial"].toString();
                QString name = requestData["name"].toString();
                QString description = requestData["description"].toString();
                bool type = requestData["type"].toBool();

                QSqlQuery query2;


                query2.prepare("SELECT id FROM devices WHERE serial = ?");
                query2.addBindValue(serial);
                if (query2.exec() && query2.next()) {
                    qDebug() << "Already exists while creating " + serial;
                    response["status"] = "fail";
                    response["cause"] = "Already exists";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }


                QSqlQuery query;
                query.prepare("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)");
                query.addBindValue(serial);
                query.addBindValue(name);
                query.addBindValue(description);
                query.addBindValue(type);
                if (!query.exec()) {
                    qDebug() << "Unexpected Error (while: Execute query).";
                    qDebug() << query.executedQuery();
                    qDebug() << db.lastError();
                    response["status"] = "fail";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                response["status"] = "success";

                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "editDevice") {
                QJsonObject response;

                QJsonObject requestData = requestObject["requestData"].toObject();
                QString id = requestData["id"].toString();
                QString serial = requestData["serial"].toString();
                QString name = requestData["name"].toString();
                QString description = requestData["description"].toString();
                bool type = requestData["type"].toBool();

                QSqlQuery query2;
                query2.prepare("SELECT id FROM devices WHERE serial = ? AND id != ?");
                query2.addBindValue(serial);
                query2.addBindValue(id);
                if (query2.exec()) {
                    qDebug() << "Already exists with same serial while updating";
                    response["status"] = "fail";
                    response["cause"] = "Already exists with same serial";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                QSqlQuery query;
                query.prepare("UPDATE devices SET serial = ?, name = ?, description = ?, type = ? WHERE id = ? ");
                query.addBindValue(serial);
                query.addBindValue(name);
                query.addBindValue(description);
                query.addBindValue(type);
                query.addBindValue(id);
                if (!query.exec()) {
                    qDebug() << "Unexpected Error (while: Execute query).";
                    response["status"] = "fail";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                response["status"] = "success";


                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);
            } else if (requestObject["requestType"] == "changeDevices") {
                QJsonObject response;

                db.transaction();

                QSqlQuery query;

                if (!query.exec("DELETE FROM devices")) {
                    db.rollback();
                    response["status"] = "fail";
                    respond(clientSocket, QJsonDocument(response));
                    return;
                }

                query.prepare("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)");

                QJsonArray jsonArray = requestObject["requestData"].toArray();

                for (const QJsonValue &value : std::as_const(jsonArray)) {

                    QJsonObject device = value.toObject();

                    query.addBindValue(device["serial"].toString());
                    query.addBindValue(device["name"].toString());
                    query.addBindValue(device["description"].toString());
                    query.addBindValue(device["type"].toBool());
                    if (!query.exec()) {
                        db.rollback();
                        response["status"] = "fail";
                        respond(clientSocket, QJsonDocument(response));
                        return;
                    }

                }

                if (!db.commit()) {
                    response["status"] = "fail";
                } else {
                    response["status"] = "success";
                }

                respond(clientSocket, QJsonDocument(response));

            } else if (requestObject["requestType"] == "showHistory") {

                QJsonObject response;
                QJsonArray jsonArray;

                QSqlQuery query;
                QString sql =
                    "SELECT "
                    "   devices.serial, "
                    "   devices.name, "
                    "   log.prev_state, "
                    "   log.new_state, "
                    "   log.time "
                    "FROM log "
                    "INNER JOIN devices ON devices.id = log.device_id "
                    "WHERE 1=1";

                if (requestObject.contains("state")) {
                    sql += " AND (log.prev_state = :state OR log.new_state = :state)";
                }
                if (requestObject.contains("low")) {
                    sql += " AND log.time > :timestamp";
                }
                if (requestObject.contains("high")) {
                    sql += " AND log.time < :timestamp2";
                }

                query.prepare(sql);

                if (requestObject.contains("state")) {
                    query.bindValue(":state", requestObject["state"].toInt());
                }
                if (requestObject.contains("low")) {
                    query.bindValue(":timestamp", QDateTime::fromString(requestObject["low"].toString(), Qt::ISODate));
                }
                if (requestObject.contains("high")) {
                    query.bindValue(":timestamp2", QDateTime::fromString(requestObject["high"].toString(), Qt::ISODate));
                }

                if (!query.exec()) {
                    qDebug() << "Unexpected Error (while: Execute query).";
                    qDebug() << query.lastError();
                    qDebug() << query.lastQuery();
                    response["status"] = "fail";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }


                while (query.next()) {
                    QJsonObject jsonObject;
                    jsonObject["serial"] = query.value(0).toString();
                    jsonObject["name"] = query.value(1).toString();
                    jsonObject["prev_state"] = query.value(2).toString();
                    jsonObject["new_state"] = query.value(3).toString();
                    jsonObject["time"] = query.value(4).toString();
                    jsonArray.append(jsonObject);
                }

                response["status"] = "success";
                response["data"] = jsonArray;
                qDebug() << jsonArray;
                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "deleteDevice") {

                QJsonObject response;

                QJsonObject requestData = requestObject["requestData"].toObject();
                int id = requestData["id"].toInt();

                QSqlQuery query;


                query.prepare("DELETE FROM devices WHERE id = ?");
                query.addBindValue(id);
                if (!query.exec()) {
                    qDebug() << "Unexpected error while deleting, execute query " + QString::number(id);
                    response["status"] = "fail";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                if (query.numRowsAffected() == 0) {
                    qDebug() << "Already deleted " + QString::number(id);
                    response["status"] = "fail";
                    response["cause"] = "Already deleted";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                response["status"] = "success";

                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);

            }

        }

    });

}

void Server::respond(QTcpSocket *clientSocket, QJsonDocument responseDoc) {
;
    QByteArray responseData = responseDoc.toJson();

    const int chunkSize = 1024;
    for (int i = 0; i < responseData.size(); i += chunkSize) {
        QByteArray chunk = responseData.mid(i, chunkSize);
        clientSocket->write(chunk);
        clientSocket->waitForBytesWritten();
    }
    clientSocket->disconnectFromHost();
}
