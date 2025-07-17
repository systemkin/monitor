#include "db_repository.h"
db_repository* db_repository::instance = nullptr;
db_repository::db_repository() {}

db_repository* db_repository::getInstance()
{
    if (!instance) instance = new db_repository();
    return instance;
}


QJsonObject db_repository::getLog(QJsonObject requestObject) {
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

    QJsonObject result = MonitorDB::getInstance()->executeQuery(sql, params);

    if (result["status"] != "success") {
        qDebug() << "Query failed:" << result["message"];
        response["status"] = "fail";
    } else {
        response["status"] = "success";
        response["data"] = result["data"];

    }

    return response;
}

QJsonObject db_repository::getStates() {

    QJsonObject result = MonitorDB::getInstance()->executeQuery("SELECT devices.serial, devices.name, devices.description, devices.type, states.state, devices.id FROM states INNER JOIN devices ON devices.id = device_id");
    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
    }
    return result;
}

QJsonObject db_repository::addDevice(device device) {

    QJsonObject result = MonitorDB::getInstance()->executeQuery("SELECT id FROM devices WHERE serial = ?", {device.serial});
    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return result;
    }
    if (result["data"].toArray().size() != 0) {
        QJsonObject response;
        qDebug() << "Already exists while creating " + device.serial;
        response["status"] = "fail";
        response["message"] = "Already exists";
        return response;
    }

    QJsonObject insertResult = MonitorDB::getInstance()->executeQuery("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)", {device.serial, device.name, device.description, device.type});
    if (insertResult["status"] != "success") {
        qDebug() << "Can not execute query. message: " << insertResult["message"];
    }
    return insertResult;
}

QJsonObject db_repository::editDevice(int id, device device) {

    QJsonObject result = MonitorDB::getInstance()->executeQuery("SELECT id FROM devices WHERE serial = ? AND id != ?", {device.serial, id});
    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return result;
    }
    if (!result["data"].toArray().empty()) {
        QJsonObject response;
        qDebug() << "Already exists with same serial while updating";
        response["status"] = "fail";
        response["message"] = "Already exists with same serial";
        return response;
    }
    QJsonObject updateResult = MonitorDB::getInstance()->executeQuery("UPDATE devices SET serial = ?, name = ?, description = ?, type = ? WHERE id = ? ", {device.serial, device.name, device.description, device.type, id});
    if (updateResult["status"] != "success") {
        qDebug() << "Can not execute query. message: " << updateResult["message"];
    }
    return updateResult;

}

QJsonObject db_repository::deleteDevice(int id) {
    QJsonObject response;

    QJsonObject result = MonitorDB::getInstance()->executeQuery("DELETE FROM devices WHERE id = ?", {id});
    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return result;
    }
    if (result["rowsAffected"].toInt() == 0) {
        qDebug() << "Already deleted " + QString::number(id);
        response["status"] = "success";
        response["message"] = "Already deleted";
        return response;
    }
    response["status"] = "success";

    return response;
}

QJsonObject db_repository::changeDevices(std::vector<device> devices) {
    QJsonObject response;


    QJsonObject result = MonitorDB::getInstance()->executeQuery("DELETE FROM devices");
    if (result["status"] != "success") {
        qDebug() << "Can not execute query. message: " << result["message"];
        return result;
    }
    for (const device &device : devices) {
        QJsonObject insertResult = MonitorDB::getInstance()->executeQuery("INSERT INTO devices (serial, name, description, type) VALUES (?, ?, ?, ?)", {device.serial, device.name, device.description, device.type});
        if (insertResult["status"] != "success") {
            qDebug() << "Can not execute query. message: " << insertResult["message"];

            return insertResult;
        }

    }
    response["status"] = "success";
    return response;
}

QJsonObject db_repository::updateState(int id, State state) {
    QJsonObject updateResult = MonitorDB::getInstance()->executeQuery("UPDATE states SET state = ? WHERE device_id = ? ", {stateToInt(state), id});
    if (updateResult["status"] != "success") {
        qDebug() << "Can not execute query. message: " << updateResult["message"];
    }
    return updateResult;
}
