#include "refresherlib.h"
#include <QProcess>


Refresher::Refresher(MonitorDB* dbm, QFile* dataFile, QFile* bashFile,QObject* parent)
    : QObject(parent), m_dbm(dbm), dataFile(dataFile), bashFile(bashFile)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Refresher::refreshStates);
}
Refresher::~Refresher()
{
    stop();
    timer->deleteLater();
}
void Refresher::start(int time)
{
    timer->start(time);
}

void Refresher::stop()
{
    timer->stop();
}

int Refresher::getState(QString serial, bool type)
{
    if (type) {
        QProcess process;
        process.start("bash", QStringList() << bashFile->fileName() << serial << dataFile->fileName());
        if (!process.waitForFinished()) {
            qDebug() << "Script execution error";
            return 0;
        }

        QByteArray output = process.readAllStandardOutput();
        bool ok;
        int state = output.trimmed().toInt(&ok);
        if (ok) return state;
        else return 0;
    }
    else {
        QTextStream in(dataFile);
        dataFile->seek(0);
        while (!in.atEnd()) {

            QString line = in.readLine();
            QStringList stringList = line.split(" ");

            if (stringList[0] == serial) {
                return stringList[1].toInt();
            }
        }
        return 0;
    }
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
            qDebug() << "changed to: " << currState << " from" << dbState;
            if (updateResult["status"] != "success") {
                qDebug() << "Update failed, problem with device: " << device["id"].toInt();
                return;
            }
        }
    }

}
