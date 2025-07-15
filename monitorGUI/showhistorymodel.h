#ifndef SHOWHISTORYMODEL_H
#define SHOWHISTORYMODEL_H

#include <QObject>
#include <QDateTime>
#include <QAbstractTableModel>
#include <QJsonObject>

struct historyItem {
    QString serial;
    QString name;
    int prev_state;
    int new_state;
    QString dateTime;
    /*!
     * \brief creates object from json
     * \param historyItem in json format
     * \return object of struct historyItem
     */
    static historyItem fromJson(QJsonObject jsonHistoryItem) {
        return historyItem({
            jsonHistoryItem.value("serial").toString(),
            jsonHistoryItem.value("name").toString(),
            jsonHistoryItem.value("prev_state").toInt(),
            jsonHistoryItem.value("new_state").toInt(),
            jsonHistoryItem.value("dateTime").toString(),
        });
    }
};
class showHistoryModel  : public QAbstractTableModel
{
    Q_OBJECT
private:
    std::vector<historyItem> container;
    int rows;
    int columns;

public:
    showHistoryModel();
    showHistoryModel(int rows, int columns, std::vector<historyItem> container);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    std::vector<historyItem> getContainerCopy();
};

#endif // SHOWHISTORYMODEL_H
