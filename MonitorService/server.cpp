#include "server.h"
#include "configreader.h"
#include <monitor_db.h>
#include "db_repository.h"
#include <QDomDocument>

Server::Server(QObject *parent) : QObject(parent) {

    configReader reader = configReader();
    config = reader.get("/home/alexej/monitor/configService.xml");
    db_repository::getInstance(config.database.host, config.database.name, config.database.username, config.database.password, config.database.port);

    refresherThread = new QThread(this);
    refresher = new Refresher(config.control.file, config.control.bash, nullptr);
    refresher->moveToThread(refresherThread);

    int time = config.control.time;
    connect(refresherThread, &QThread::started, refresher, [this, time]() {
        this->refresher->start(time);
    });
    connect(refresherThread, &QThread::finished, refresher, &Refresher::stop);

    refresherThread->start();

    serviceThread = new QThread(this);
    service = new Service(nullptr);
    service->listen(QHostAddress::Any, config.port);
    service->moveToThread(serviceThread);
    serviceThread->start();
}
Server::~Server(){
    delete refresher;
    delete service;
    db_repository::deleteInstance();
}
