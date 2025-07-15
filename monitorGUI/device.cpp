#include "device.h"




device::device(QString serial, QString name, QString description, bool type) {
    this->serial = serial;
    this->name = name;
    this->description = description;
    this->type = type;
}


device::device(QJsonObject deviceJson) {
    this->serial = deviceJson.value("serial").toString();
    this->name = deviceJson.value("name").toString();
    this->description = deviceJson.value("description").toString();
    this->type = deviceJson.value("type").toBool();
}


bool device::operator==(const device& other) const {
    return serial == other.serial && name == other.name && description == other.description && type == other.type;
}

QJsonObject device::toJson() {
    QJsonObject deviceJson;
    deviceJson["serial"] = serial;
    deviceJson["name"] = name;
    deviceJson["description"] = description;
    deviceJson["type"] = type;


    return deviceJson;
}
