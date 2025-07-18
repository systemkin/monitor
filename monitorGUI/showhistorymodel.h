#ifndef SHOWHISTORYMODEL_H
#define SHOWHISTORYMODEL_H

#include <QObject>
#include <QDateTime>
#include <QAbstractTableModel>
#include <QJsonObject>
#include "historyItem.h"
/*!
 * \brief class for modeling data for display on showHistoryForm table
 */
class showHistoryModel  : public QAbstractTableModel
{
    Q_OBJECT
private:
    std::vector<historyItem> container;
    int rows;
    int columns;

public:
    showHistoryModel();
    /*!
     * \brief constructor
     * \param container - vector of historyItem to display
     */
    showHistoryModel(std::vector<historyItem> container);

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
     * \brief function to get copy of model data without data(QModelIndex, int) usage
     * \return vector of historyItem
     */
    std::vector<historyItem> getContainerCopy();
};

#endif // SHOWHISTORYMODEL_H
