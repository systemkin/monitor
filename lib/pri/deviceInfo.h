#include <QJsonObject>
#include <QString>
#include "states.h"

/*!
 * \brief struct for representation, serealization and deserealization of devices with known state
 */
struct deviceInfo {
    int id;
    QString serial;
    QString name;
    QString description;
    bool type;
    State state;
    deviceInfo() {
        id = 0;
        serial = "";
        name = "";
        description = "";
        type = false;
        state = State::Undefined;
    };

    bool operator==(const deviceInfo& other) const {
        return id == other.id && serial == other.serial && name == other.name && description == other.description && type == other.type && other.state == state;
    }
    /*!
     * \brief creates object from json
     * \param deviceInfo in json format
     * \return object of struct deviceInfo
     */

    deviceInfo(QJsonObject jsonDeviceInfo) {
        id = jsonDeviceInfo.value("id").toInt();
        serial = jsonDeviceInfo.value("number").toString();
        name = jsonDeviceInfo.value("name").toString();
        description = jsonDeviceInfo.value("description").toString();
        type = jsonDeviceInfo.value("type").toBool();
        state = intToState(jsonDeviceInfo.value("state").toInt());
    }

    static deviceInfo fromDbJson(QJsonObject jsonDeviceInfo) {
        deviceInfo deviceInfo;
        deviceInfo.id = jsonDeviceInfo.value("id").toInt();
        deviceInfo.serial = jsonDeviceInfo.value("serial").toString();
        deviceInfo.name = jsonDeviceInfo.value("name").toString();
        deviceInfo.description = jsonDeviceInfo.value("description").toString();
        deviceInfo.type = jsonDeviceInfo.value("type").toBool();
        deviceInfo.state = intToState(jsonDeviceInfo.value("state").toInt());
        return deviceInfo;
    }

    QJsonObject toJson() {
        QJsonObject deviceInfoJson;
        deviceInfoJson["id"] = id;
        deviceInfoJson["number"] = serial;
        deviceInfoJson["name"] = name;
        deviceInfoJson["description"] = description;
        deviceInfoJson["state"] = stateToInt(state);
        deviceInfoJson["type"] = type;
        return deviceInfoJson;
    }


    /*!
     * \brief creates QString representation of a device for export to text files
     * \return QString representation of a device
     */
    QString toText() const {
        return QString(QString(id) + "\t"
                       + serial + "\t"
                       + name + "\t"
                       + description + "\t"
                       + (type ?  QString::fromUtf8("bash") :  QString::fromUtf8("Файл")) + "\t"
                       + stateToString(state) + "\n");
    }
    /*!
     * \brief creates QString representation of a device for export to CSV files
     * \return QString representation of a device
     */
    QString toCSV() const {
        return QString ("\"" + QString::number(id) + "\","
        + "\"" + serial + "\","
        + "\"" + name + "\","
        + "\"" + description + "\","
        + "\"" + (type ? "bash" : QString::fromUtf8("Файл")) + "\","
        + "\"" + stateToString(state) + "\"\n");
    }
};
