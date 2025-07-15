#include "device.h"


/*!
 * \brief - constructor with setting all fields directly
 * \param id -  id in database
 * \param serial - serial nubmer
 * \param name - device name
 * \param description - device description
 * \param type - true for monitoring from bash, false for monitoring from file
 */

device::device(int id, QString serial, QString name, QString description, bool type) {
    this->id = id;
    this->serial = serial;
    this->name = name;
    this->description = description;
    this->type = type;
}

/*!
 * \brief constructor from Json
 * \param deviceJson - json with necessary fields: id, serial, name, description, type
 */
device::device(QJsonObject deviceJson) {
    this->id = deviceJson.value("id").toInt();
    this->serial = deviceJson.value("serial").toString();
    this->name = deviceJson.value("name").toString();
    this->description = deviceJson.value("description").toString();
    this->type = deviceJson.value("type").toBool();
}

/*!
 * \brief comparison operator
 * \param other - other device
 * \return true if equals, false otherwise.
 */
bool device::operator==(const device& other) const {
    return id == other.id && serial == other.serial && name == other.name && description == other.description && type == other.type;
}

/*!
 * \brief creates json representation of device
 * \return Json-representation of device with fields: id, serial, name, description, type
 */
QJsonObject device::toJson() {
    QJsonObject deviceJson;
    deviceJson["id"] = id;
    deviceJson["serial"] = serial;
    deviceJson["name"] = name;
    deviceJson["description"] = description;
    deviceJson["type"] = type;


    return deviceJson;
}
