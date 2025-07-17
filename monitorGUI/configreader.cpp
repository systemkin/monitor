#include "configreader.h"
#include <QFile>
#include <QDebug>
#include <QDomDocument>
configReader::configReader() {
}
Config configReader::get(QString configFile) {
    Config result;
    result.host = "";
    result.port = 0;
    QFile file(configFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open config file";
        return result;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML config file";
        file.close();
        return result;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement connection = root.firstChildElement("connection");

    if (connection.isNull()) {
        qDebug() << "No field 'connection' in file";
        return result;
    }
    QDomElement hostElem = connection.firstChildElement("host");
    QDomElement portElem = connection.firstChildElement("port");

    if (!hostElem.isNull()) {
        result.host = hostElem.text();
    } else {
        qDebug() << "No field 'connection.host' in file";
        return result;
    }

    if (portElem.isNull()) {
        qDebug() << "No field 'connection.port' in file";
        return result;
    }

    bool ok;
    result.port = portElem.text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid field 'connection.port in file";
        return result;
    }
    return result;
}
