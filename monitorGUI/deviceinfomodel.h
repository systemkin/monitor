





#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H


#include <QAbstractTableModel>
#include <QFile>
#include <QJsonObject>

#include "deviceInfo.h"

/*!
 * \brief class for modeling data for display on mainWindow table
 */
class DeviceInfoModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    std::vector<deviceInfo> container;
    int rows;
    int columns;
public:
    DeviceInfoModel();
    /*!
     * \brief constructor
     * \param container - vector of deviceInfo to display
     */
    DeviceInfoModel(std::vector<deviceInfo> container);

    /*!
     * \brief overriden QAbstractTableModel function
     * \param parent
     * \return amount of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief overriden QAbstractTableModel function
     * \param parent
     * \return amount of columns
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief overriden QAbstractTableModel function
     * \param index - place in "table", row and column
     * \param role - ForegroundRole or DisplayRole
     * \return requested data - table item or color (based on role)
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*!
     * \brief overriden QAbstractTableModel function
     * \param section - column of header
     * \param orientation - orientation of header
     * \param role - DisplayRole
     * \return
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /*!
     * \brief function to get item from model data at position
     * \param row - row of requested deviceInfo
     * \return requested deviceInfo
     */
    deviceInfo at(int row);

    /*!
     * \brief function to get copy of model data without data(QModelIndex, int) usage
     * \return vector of historyItem
     */
    std::vector<deviceInfo> getContainerCopy();
};

#endif // DEVICEINFOMODEL_H
