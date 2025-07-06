#include "deviceinfomodel.h"
#include <QTextStream>

DeviceInfoModel::DeviceInfoModel()
    : QAbstractTableModel()
{
    rows = 0;
    columns = 6;
    container = std::vector<deviceInfo>();
}

DeviceInfoModel::DeviceInfoModel(int rows, int columns, std::vector<deviceInfo> container) {

    this->rows = rows;
    this->columns = columns;
    this->container = container;
}

int DeviceInfoModel::rowCount(const QModelIndex & /*parent*/) const {
    return rows;
}

int DeviceInfoModel::columnCount(const QModelIndex & /*parent*/) const {
    return columns;
}

QVariant DeviceInfoModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0:
            return QString::number(container[index.row()].id);
            break;
        case 1:
            return QString(container[index.row()].serial);
            break;
        case 2:
            return QString(container[index.row()].name);
            break;
        case 3:
            return QString(container[index.row()].description);
            break;
        case 4:
            if (container[index.row()].type)
                return QString("Файл");
            else return QString("bash");
            break;
        case 5:
            return container[index.row()].state;
            break;
        }
    }
    return QVariant();
}
QVariant DeviceInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((role == Qt::DisplayRole) || (orientation == Qt::Vertical))
    {
        switch(section)
        {
        case 0:
        {
            return QString("id");
        }
        case 1:
        {
            return QString("Серийный номер");
        }
        case 2:
        {
            return QString("Наименование");
        }
        case 3:
        {
            return QString("Описание");
        }
        case 4:
        {
            return QString("Тип");
        }
        case 5:
        {
            return QString("Состояние");
        }
        default:
            return QVariant();
        }
    }
    return QVariant();
}
std::vector<deviceInfo> DeviceInfoModel::getContainerCopy() {
    return container;
}
deviceInfo DeviceInfoModel::at(int row) {
    return container[row];
}
