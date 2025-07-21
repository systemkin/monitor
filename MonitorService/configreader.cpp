#include "configreader.h"
#include <QFile>
#include <QDebug>
#include <QDomDocument>
configReader::configReader() {
}
Config configReader::get(QString configFile) {
    Config result;
    result.database = Database();
    result.control = Control();
    QFile config(configFile);
    if (!config.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open config file";
        return result;
    }
    QDomDocument doc;
    if (!doc.setContent(&config)) {
        qDebug() << "Failed to parse XML config file";
        config.close();
        return result;
    }
    config.close();

    QDomElement root = doc.documentElement();
    QDomElement database = root.firstChildElement("database");

    if (database.isNull()) {
        qDebug() << "No field 'database' in file";
        return result;
    }
    QDomElement hostElem = database.firstChildElement("host");
    QDomElement nameElem = database.firstChildElement("name");
    QDomElement usernameElem = database.firstChildElement("username");
    QDomElement passwordElem = database.firstChildElement("password");
    QDomElement portElem = database.firstChildElement("port");

    if (!hostElem.isNull()) {
        result.database.host = hostElem.text();
    } else {
        qDebug() << "No field 'database.host' in file";
        return result;
    }
    if (!nameElem.isNull()) {
        result.database.name = nameElem.text();
    } else {
        qDebug() << "No field 'database.name' in file";
        return result;
    }
    if (!usernameElem.isNull()) {
        result.database.username = usernameElem.text();
    } else {
        qDebug() << "No field 'database.username' in file";
        return result;
    }
    if (!passwordElem.isNull()) {
        result.database.password = passwordElem.text();
    } else {
        qDebug() << "No field 'database.password' in file";
        return result;
    }
    if (portElem.isNull()) {
        qDebug() << "No field 'database.port' in file";
        return result;
    }

    bool ok;
    result.database.port = portElem.text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid field 'connection.port' in file";
        return result;
    }





    QDomElement control = root.firstChildElement("control");

    if (control.isNull()) {
        qDebug() << "No field 'control' in file";
        return result;
    }
    QDomElement fileElem = control.firstChildElement("file");
    QDomElement bashElem = control.firstChildElement("bash");
    QDomElement timeElem = control.firstChildElement("time");

    if (!fileElem.isNull()) {
        result.control.file = fileElem.text();
    } else {
        qDebug() << "No field 'control.file' in file";
        return result;
    }

    if (!bashElem.isNull()) {
        result.control.bash = bashElem.text();
    } else {
        qDebug() << "No field 'control.bash' in file";
        return result;
    }


    if (!timeElem.isNull()) {
        result.control.time = timeElem.text().toInt();
    } else {
        qDebug() << "No field 'control.time' in file";
        return result;
    }
    QDomElement server = root.firstChildElement("server");
    if (server.isNull()) {
        qDebug() << "No field 'server' in file";
        return result;
    }
    QDomElement portElemServer = server.firstChildElement("port");
    result.port = portElemServer.text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid field 'server.port' in file";
        return result;
    }
    return result;
}
