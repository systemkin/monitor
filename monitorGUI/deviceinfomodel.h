





#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H


#include <QAbstractTableModel>
#include <QFile>

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
