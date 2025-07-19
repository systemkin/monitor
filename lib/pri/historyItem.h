#include "states.h"
#include <QJsonObject>
#include <QObject>

/*!
 * \brief struct for representation and deserealization history records
 */
struct historyItem {
    QString serial;
    QString name;
    State prev_state;
    State new_state;
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
            intToState(jsonHistoryItem.value("prev_state").toInt()),
            intToState(jsonHistoryItem.value("new_state").toInt()),
            jsonHistoryItem.value("time").toString(),
        });
    }
};
