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
     * \brief default constructor
     */
    historyItem() {
        serial = "";
        name = "";
        prev_state = State::Undefined;
        new_state = State::Undefined;
        dateTime = "";

    }

    /*!
     * \brief creates object from json
     * \param historyItem in json format
     * \return object of struct historyItem
     */
    historyItem(QJsonObject jsonHistoryItem) {
        serial = jsonHistoryItem.value("number").toString();
        name = jsonHistoryItem.value("name").toString();
        prev_state = intToState(jsonHistoryItem.value("prev_state").toInt());
        new_state = intToState(jsonHistoryItem.value("new_state").toInt());
        dateTime = jsonHistoryItem.value("dateTime").toString();
    }

    /*!
     * \brief function to create historyItem from JSON with db fields
     * \param JSON - representation of historyItem in db fields
     * \return historyItem object
     */
    static historyItem fromDbJson(QJsonObject jsonHistoryItem) {
        historyItem historyItem;
        historyItem.serial = jsonHistoryItem.value("serial").toString();
        historyItem.name = jsonHistoryItem.value("name").toString();
        historyItem.prev_state = intToState(jsonHistoryItem.value("prev_state").toInt());
        historyItem.new_state = intToState(jsonHistoryItem.value("new_state").toInt());
        historyItem.dateTime = jsonHistoryItem.value("time").toString();
        return historyItem;
    }

    /*!
     * \brief function to convert historyItem to JSON
     * \return JSON representation of historyItem
     */
    QJsonObject toJson() {
        QJsonObject historyItemJson;
        historyItemJson["number"] = serial;
        historyItemJson["name"] = name;
        historyItemJson["prev_state"] = stateToInt(prev_state);
        historyItemJson["new_state"] = stateToInt(new_state);
        historyItemJson["dateTime"] = dateTime;

        return historyItemJson;
    }
};
