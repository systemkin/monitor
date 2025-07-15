





#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H


#include <QAbstractTableModel>
#include <QFile>
#include <QJsonObject>

#include "deviceInfo.h"


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
