





#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H


#include <QAbstractTableModel>
#include <QFile>

struct deviceInfo {
    long long id;
    long long serial;
    std::string name;
    std::string description;
    bool type;
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

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;


    bool reloadData();

};

#endif // DEVICEINFOMODEL_H
