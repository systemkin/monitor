#include "device.h"

device::device() {
    serial = "";
    name = "";
    description = "";
    type = false;
}
device::device(QString serial, QString name, QString description, bool type) {
    this->serial = serial;
    this->name = name;
    this->description = description;
    this->type = type;
}


device::device(QJsonObject deviceJson) {
    this->serial = deviceJson.value("number").toString();
    this->name = deviceJson.value("name").toString();
    this->description = deviceJson.value("description").toString();
    this->type = deviceJson.value("type").toBool();
}

device device::fromDbJson(QJsonObject deviceJson) {
    device device;
    device.serial = deviceJson.value("serial").toString();
    device.name = deviceJson.value("name").toString();
    device.description = deviceJson.value("description").toString();
    device.type = deviceJson.value("type").toBool();
    return device;
}

bool device::operator==(const device& other) const {
    return serial == other.serial && name == other.name && description == other.description && type == other.type;
}

QJsonObject device::toJson() {
    QJsonObject deviceJson;
    deviceJson["number"] = serial;
    deviceJson["name"] = name;
    deviceJson["description"] = description;
    deviceJson["type"] = type;


    return deviceJson;
}
