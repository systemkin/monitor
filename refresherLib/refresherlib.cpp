#include "refresherlib.h"
#include <QProcess>
#include "db_repository.h"
#include "deviceInfo.h"
#include "states.h"

Refresher::Refresher(QString dataPath, QString bashPath,QObject* parent)
    : QObject(parent)
{
    this->dataPath = dataPath;
    this->bashPath = bashPath;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Refresher::refreshStates);
}
Refresher::~Refresher()
{
    stop();
}
void Refresher::start(int time)
{
    timer->start(time);
}

void Refresher::stop()
{
    timer->stop();
}

State Refresher::getState(QString serial, bool type)
{
    if (type) {
        QProcess process;
        process.start("bash", QStringList() << bashPath << serial << dataPath);
        if (!process.waitForFinished()) {
            qDebug() << "Script execution error";
            return State::Undefined;
        }

        QByteArray output = process.readAllStandardOutput();
        bool ok;
        int state = output.trimmed().toInt(&ok);
        if (ok) return intToState(state);
        else {
            qDebug() << "Troubles with parsing, probably bash or datafile is not acessible or file data format error";
            return State::Undefined;
        }
    }
    else {
        QFile dataFile(dataPath);
        if(!dataFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Cant open data file";
            return State::Undefined;
        }
        QTextStream in(&dataFile);
        dataFile.seek(0);
        while (!in.atEnd()) {

            QString line = in.readLine();
            QStringList stringList = line.split(" ");

            if (stringList[0] == serial) {
                return intToState(stringList[1].toInt());
            }
        }
        return State::Undefined;
    }
}

void Refresher::refreshStates()
{
    QJsonObject result = db_repository::getInstance()->getStates();

    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return;
    }

    QJsonArray devices = result["data"].toArray();
    for (const QJsonValue& deviceValue : std::as_const(devices)) {
        deviceInfo device(deviceValue.toObject());
        State currState = getState(device.serial, device.type);
        if (currState != device.state) {
            device.state = currState;
            QJsonObject updateResult = db_repository::getInstance()->updateState(device.id, currState);
            if (updateResult["status"] != "success") {
                qDebug() << "Update failed, problem with device: " << device.id;
                return;
            }
        }
    }

}
