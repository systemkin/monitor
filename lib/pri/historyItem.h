#include <QJsonObject>
#include <QObject>

/*!
 * \brief struct for representation and deserealization history records
 */
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
