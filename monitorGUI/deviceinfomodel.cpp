#include "deviceinfomodel.h"
#include <QTextStream>

DeviceInfoModel::DeviceInfoModel()
    : QAbstractTableModel()
{
    rows = 0;
    columns = 5;
    container = std::vector<deviceInfo>();
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
            return QString::number(container[index.row()].serial);
            break;
        case 2:
            return QString(container[index.row()].name.c_str());
            break;
        case 3:
            return QString(container[index.row()].description.c_str());
            break;
        case 4:
            if (container[index.row()].type)
                return QString("Обычный");
            else return QString("bash");
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
        default:
            return QVariant();
        }
    }
    return QVariant();
}

