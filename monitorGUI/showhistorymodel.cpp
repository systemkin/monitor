#include "showhistorymodel.h"
#include <QDebug>
#include <QBrush>

showHistoryModel::showHistoryModel()
    : QAbstractTableModel()
{
    rows = 0;
    columns = 5;
    container = std::vector<historyItem>();
}

showHistoryModel::showHistoryModel(int rows, int columns, std::vector<historyItem> container) {

    this->rows = rows;
    this->columns = columns;
    this->container = container;
}

int showHistoryModel::rowCount(const QModelIndex & /*parent*/) const {
    return rows;
}

int showHistoryModel::columnCount(const QModelIndex & /*parent*/) const {
    return columns;
}

QVariant showHistoryModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0:
            return QString(container[index.row()].serial);
            break;
        case 1:
            return QString(container[index.row()].name);
            break;
        case 2:
            switch(container[index.row()].prev_state) {
            case 0:
                return QString("Неизвестно");
                break;
            case 1:
                return QString("Не работает");
                break;
            case 2:
                return QString("Авария");
                break;
            case 3:
                return QString("Работает");
                break;
            }
            break;
        case 3:
            switch(container[index.row()].new_state) {
            case 0:
                return QString("Неизвестно");
                break;
            case 1:
                return QString("Не работает");
                break;
            case 2:
                return QString("Авария");
                break;
            case 3:
                return QString("Работает");
                break;
            }
            break;
        case 4:
            return QString(container[index.row()].dateTime);
            break;

        }
    }
    if ((role == Qt::ForegroundRole) && (index.column() == 2))  {
        switch(container[index.row()].prev_state) {
        case 0: return QBrush(Qt::gray);
        case 1: return QBrush(QColor(255, 165, 0));
        case 2: return QBrush(Qt::red);
        case 3: return QBrush(QColor(0, 125, 0));
        }
    }
    if ((role == Qt::ForegroundRole) && (index.column() == 3))  {
        switch(container[index.row()].new_state) {
        case 0: return QBrush(Qt::gray);
        case 1: return QBrush(QColor(255, 165, 0));
        case 2: return QBrush(Qt::red);
        case 3: return QBrush(QColor(0, 125, 0));
        }
    }
    return QVariant();
}
QVariant showHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((role == Qt::DisplayRole) || (orientation == Qt::Vertical))
    {
        switch(section)
        {
        case 0:
        {
            return QString("Серийный номер");
        }
        case 1:
        {
            return QString("Наименование");
        }
        case 2:
        {
            return QString("Предыдущее состояние");
        }
        case 3:
        {
            return QString("Новое состояние");
        }
        case 4:
        {
            return QString("Время");
        }
        default:
            return QVariant();
        }
    }
    return QVariant();
}
std::vector<historyItem> showHistoryModel::getContainerCopy() {
    qDebug() << "3";
    return container;
}
