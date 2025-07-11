#include "refresher.h"



Refresher::Refresher(MonitorDB* dbm, QFile* dataFile, QObject* parent)
    : QObject(parent), m_dbm(dbm), m_dataFile(dataFile)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Refresher::refreshStates);
}
Refresher::~Refresher()
{
    stop();
    m_timer->deleteLater();
}
void Refresher::start(int time)
{
    m_timer->start(time);
}

void Refresher::stop()
{
    m_timer->stop();
}

int Refresher::getState(QString serial, bool type)
{
    QTextStream in(m_dataFile);
    m_dataFile->seek(0);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList stringList = line.split(" ");
        if (stringList[0] == serial) {
            return stringList[1].toInt();
        }
    }
    return 0;
}

void Refresher::refreshStates()
{
    QJsonObject result = m_dbm->executeQuery("SELECT devices.id, devices.serial, states.state, devices.type FROM devices LEFT JOIN states ON devices.id = states.device_id");

    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return;
    }

    QJsonArray devices = result["data"].toArray();
    for (const QJsonValue& deviceValue : std::as_const(devices)) {
        QJsonObject device = deviceValue.toObject();
        QString serial = device["serial"].toString();
        int dbState = device["state"].toInt();
        bool type = device["type"].toBool();
        int currState = getState(serial, type);

        if (currState != dbState) {
            QJsonObject updateResult = m_dbm->executeQuery("UPDATE states SET state = ? WHERE device_id = ?", {currState, device["id"].toInt()});
            if (updateResult["status"] != "success") {
                qDebug() << "Update failed, problem with device: " << device["id"].toInt();
                return;
            }
        }
    }

}
