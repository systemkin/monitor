#include "server.h"
#include "configreader.h"
#include <monitor_db.h>

#include <QDomDocument>

Server::Server(QObject *parent) : QObject(parent) {

    configReader reader = configReader();
    config = reader.get("/home/alexej/monitor/configService.xml");
    dbm = MonitorDB::initialize(config.database.host, config.database.name, config.database.username, config.database.password, config.database.port);

    QFile* file = new QFile(config.control.file);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << config.control.file;
        qDebug() << "No such data file or Error during opening" << file->errorString();
        return;
    }
    file->close();
    delete file;


    refresherThread = new QThread(nullptr);
    refresher = new Refresher(config.control.file, config.control.bash, nullptr);
    refresher->moveToThread(refresherThread);

    int time = config.control.time;
    connect(refresherThread, &QThread::started, refresher, [this, time]() {
        this->refresher->start(time);
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
