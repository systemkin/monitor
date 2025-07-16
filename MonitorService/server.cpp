#include "server.h"
#include <monitor_db.h>

#include <QDomDocument>

Server::Server(QObject *parent) : QObject(parent) {

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
    dbm = MonitorDB::initialize(host, name, username, password, port);
    dbm = MonitorDB::getInstance();

    QFile* file = new QFile(filePath);
    QFile* bash = new QFile(bashPath);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << filePath;
        qDebug() << "No such data file or Error during opening" << file->errorString();
        return;
    }


    refresherThread = new QThread(nullptr);
    refresher = new Refresher(dbm, file, bash);
    refresher->moveToThread(refresherThread);

    connect(refresherThread, &QThread::started, refresher, [this, timer]() {
        this->refresher->start(timer);
    });
    connect(refresherThread, &QThread::finished, refresher, &Refresher::stop);

    refresherThread->start();

    serviceThread = new QThread();
    service = new Service(nullptr);
    service->listen(QHostAddress::Any, 12345);
    service->moveToThread(serviceThread);
    serviceThread->start();
}
Server::~Server(){
    delete serviceThread;
    delete refresherThread;
}
