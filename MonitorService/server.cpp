#include "server.h"
#include <monitor_db.h>

#include <QDomDocument>

Server::Server(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);



    QFile config("/home/alexej/monitor/configService.xml");
    if (!config.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open config file";
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&config)) {
        qDebug() << "Failed to parse XML config file";
        config.close();
        return;
    }
    config.close();

    QDomElement root = doc.documentElement();
    QDomElement database = root.firstChildElement("database");

    if (database.isNull()) {
        qDebug() << "No field 'database' in file";
        return;
    }
    QDomElement hostElem = database.firstChildElement("host");
    QDomElement nameElem = database.firstChildElement("name");
    QDomElement usernameElem = database.firstChildElement("username");
    QDomElement passwordElem = database.firstChildElement("password");
    QDomElement portElem = database.firstChildElement("port");


    QString host;
    QString name;
    QString username;
    QString password;
    int port;

    if (!hostElem.isNull()) {
        host = hostElem.text();
    } else {
        qDebug() << "No field 'database.host' in file";
        return;
    }
    if (!nameElem.isNull()) {
        name = nameElem.text();
    } else {
        qDebug() << "No field 'database.name' in file";
        return;
    }
    if (!usernameElem.isNull()) {
        username = usernameElem.text();
    } else {
        qDebug() << "No field 'database.username' in file";
        return;
    }
    if (!passwordElem.isNull()) {
        password = passwordElem.text();
    } else {
        qDebug() << "No field 'database.password' in file";
        return;
    }
    if (portElem.isNull()) {
        qDebug() << "No field 'database.port' in file";
        return;
    }

    bool ok;
    port = portElem.text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid field 'connection.port in file";
        return;
    }

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(name);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);


    dbm = MonitorDB::getInstance(host, name, username, password, port);



    QDomElement control = root.firstChildElement("control");

    if (control.isNull()) {
        qDebug() << "No field 'control' in file";
        return;
    }
    QDomElement fileElem = control.firstChildElement("file");
    QDomElement bashElem = control.firstChildElement("bash");
    QDomElement timeElem = control.firstChildElement("time");
    QString filePath;
    QString bashPath;

    if (!fileElem.isNull()) {
        filePath = fileElem.text();
    } else {
        qDebug() << "No field 'control.file' in file";
        return;
    }

    if (!bashElem.isNull()) {
        bashPath = bashElem.text();
    } else {
        qDebug() << "No field 'control.bash' in file";
        return;
    }

    int timer;
    if (!timeElem.isNull()) {
        timer = timeElem.text().toInt();
    } else {
        qDebug() << "No field 'control.time' in file";
        return;
    }


    QFile* file = new QFile(filePath);
    QFile* bash = new QFile(bashPath);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << filePath;
        qDebug() << "No such data file or Error during opening" << file->errorString();
        return;
    }


    refresherThread = new QThread(this);
    refresher = new Refresher(dbm, file, bash);
    refresher->moveToThread(refresherThread);

    connect(refresherThread, &QThread::started, refresher, [this, timer]() {
        this->refresher->start(timer);
    });
    connect(refresherThread, &QThread::finished, refresher, &Refresher::stop);

    refresherThread->start();
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
                QJsonObject result = dbm->executeQuery("SELECT devices.serial, devices.name, devices.description, devices.type, states.state, devices.id FROM states INNER JOIN devices ON devices.id = device_id");
                if (result["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << result["message"];
                    respond(clientSocket, QJsonDocument(result));
                    return;
                }
                QJsonDocument responseDoc(result);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "addDevice") {
                QJsonObject requestData = requestObject["requestData"].toObject();
                QString serial = requestData["serial"].toString();
                QString name = requestData["name"].toString();
                QString description = requestData["description"].toString();
                bool type = requestData["type"].toBool();
                QJsonObject result = dbm->executeQuery("SELECT id FROM devices WHERE serial = ?", {serial});
                if (result["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << result["message"];
                    respond(clientSocket, QJsonDocument(result));
                    return;
                }
                if (result["data"].toArray().size() != 0) {
                    QJsonObject response;
                    qDebug() << "Already exists while creating " + serial;
                    response["status"] = "fail";
                    response["message"] = "Already exists";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }

                QJsonObject insertResult = dbm->executeQuery("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)", {serial, name, description, type});
                if (insertResult["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << insertResult["message"];
                    respond(clientSocket, QJsonDocument(insertResult));
                    return;
                }
                QJsonDocument responseDoc(insertResult);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "editDevice") {
                db.transaction();
                QJsonObject requestData = requestObject["requestData"].toObject();
                int id = requestData["id"].toInt();
                QString serial = requestData["serial"].toString();
                QString name = requestData["name"].toString();
                QString description = requestData["description"].toString();
                bool type = requestData["type"].toBool();

                QJsonObject result = dbm->executeQuery("SELECT id FROM devices WHERE serial = ? AND id != ?", {serial, id});
                if (result["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << result["message"];
                    respond(clientSocket, QJsonDocument(result));
                    return;
                }
                if (!result["data"].toArray().empty()) {
                    QJsonObject response;
                    qDebug() << "Already exists with same serial while updating";
                    response["status"] = "fail";
                    response["message"] = "Already exists with same serial";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }
                QJsonObject updateResult = dbm->executeQuery("UPDATE devices SET serial = ?, name = ?, description = ?, type = ? WHERE id = ? ", {serial, name, description, type, id});
                if (updateResult["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << updateResult["message"];
                    respond(clientSocket, QJsonDocument(updateResult));
                    return;
                }
                QJsonDocument responseDoc(updateResult);
                respond(clientSocket, responseDoc);

            } else if (requestObject["requestType"] == "changeDevices") {
                QJsonObject response;

                dbm->beginTransaction();

                QJsonObject result = dbm->executeQuery("DELETE FROM devices");
                if (result["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << result["message"];
                    respond(clientSocket, QJsonDocument(result));
                    return;
                }



                QJsonArray jsonArray = requestObject["requestData"].toArray();
                for (const QJsonValue &value : std::as_const(jsonArray)) {

                    QJsonObject device = value.toObject();

                    QJsonObject insertResult = dbm->executeQuery("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)", {device.value("serial").toString(), device.value("name").toString(), device.value("description").toString(), device.value("type").toBool()});
                    if (insertResult["status"] != "success") {
                        qDebug() << "Can not execute query. message: " << insertResult["message"];
                        dbm->rollbackTransaction();
                        respond(clientSocket, QJsonDocument(insertResult));
                        return;
                    }
                    qDebug() << "success";

                }

                if(!dbm->commitTransaction()) {
                    dbm->rollbackTransaction();
                    QJsonObject response;
                    qDebug() << "Cant commit, db closed";
                    response["status"] = "fail";
                    response["message"] = "Db closed";
                    QJsonDocument responseDoc(response);
                    respond(clientSocket, responseDoc);
                    return;
                }

                respond(clientSocket, QJsonDocument(response));

            } else if (requestObject["requestType"] == "showHistory") {
                QJsonObject response;
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

                QVariantList params;

                if (requestObject.contains("state")) {
                    sql += " AND (log.prev_state = ? OR log.new_state = ?)";
                    params << requestObject["state"].toInt() << requestObject["state"].toInt();
                }
                if (requestObject.contains("low")) {
                    sql += " AND log.time > ?";
                    params << QDateTime::fromString(requestObject["low"].toString(), Qt::ISODate);
                }
                if (requestObject.contains("high")) {
                    sql += " AND log.time < ?";
                    params << QDateTime::fromString(requestObject["high"].toString(), Qt::ISODate);
                }
                sql += " ORDER BY log.time";

                QJsonObject result = dbm->executeQuery(sql, params);

                if (result["status"] != "success") {
                    qDebug() << "Query failed:" << result["message"];
                    response["status"] = "fail";
                } else {
                    response["status"] = "success";
                    response["data"] = result["data"];

                }

                QJsonDocument responseDoc(response);
                respond(clientSocket, responseDoc);
            } else if (requestObject["requestType"] == "deleteDevice") {

                QJsonObject response;

                QJsonObject requestData = requestObject["requestData"].toObject();
                int id = requestData["id"].toInt();

                QSqlQuery query;

                QJsonObject result = dbm->executeQuery("DELETE FROM devices WHERE id = ?", {id});
                if (result["status"] != "success") {
                    qDebug() << "Can not execute query. message: " << result["message"];
                    respond(clientSocket, QJsonDocument(result));
                    return;
                }
                if (result["rowsAffected"].toInt() == 0) {
                    qDebug() << "Already deleted " + QString::number(id);
                    response["status"] = "fail";
                    response["message"] = "Already deleted";
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
