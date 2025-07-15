





#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H


#include <QAbstractTableModel>
#include <QFile>
#include <QJsonObject>
#include "states.h"

struct deviceInfo {
    int id;
    QString serial;
    QString name;
    QString description;
    bool type;
    int state;
    bool operator==(const deviceInfo& other) const {
        return id == other.id && serial == other.serial && name == other.name && description == other.description && type == other.type && other.state == state;
    }
    /*!
     * \brief creates object from json
     * \param deviceInfo in json format
     * \return object of struct deviceInfo
     */
    static deviceInfo fromJson(QJsonObject jsonDeviceInfo) {
        return deviceInfo({
            jsonDeviceInfo.value("id").toInt(),
            jsonDeviceInfo.value("serial").toString(),
            jsonDeviceInfo.value("name").toString(),
            jsonDeviceInfo.value("description").toString(),
            jsonDeviceInfo.value("type").toBool(),
            jsonDeviceInfo.value("state").toInt(),
            });
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
    QString toCSV() const {
        return QString ("\"" + QString(id) + "\","
        + "\"" + serial + "\","
        + "\"" + name + "\","
        + "\"" + description + "\","
        + "\"" + (type ? "bash" : QString::fromUtf8("Файл")) + "\","
        + "\"" + stateToString(state) + "\"\n");
    }
};

class DeviceInfoModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    std::vector<deviceInfo> container;
    int rows;
    int columns;
public:
    DeviceInfoModel();
    DeviceInfoModel(int rows, int columns, std::vector<deviceInfo> container);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    deviceInfo at(int row);

    bool reloadData();
    std::vector<deviceInfo> getContainerCopy();
};

#endif // DEVICEINFOMODEL_H
