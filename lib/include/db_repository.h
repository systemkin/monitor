#ifndef DB_REPOSITORY_H
#define DB_REPOSITORY_H

#include <QObject>
#include "monitor_db.h"
#include "device.h"
#include "states.h"
/*!
 * \brief Singleton for viewing and editing database
 */
class MONITOR_DB_EXPORT db_repository : QObject
{
    Q_OBJECT
public:

    static db_repository* getInstance();

    db_repository();
    /*!
     * \brief function for viewing log of devices sates
     * \param requestObject - JSON object that can specify "low" and "high" - dates in ISO format used for filtering. Filed "state" ca be used for tthe same purpose
     * \return JSON Object with field "status", that can either be "fail" or "success". field "data" is a JSON-array of log lines.
     */
    QJsonObject getLog(QJsonObject requestObject);

    /*!
     * \brief function for viewing states of tracking devices
     * \return JSON object with fields "status" and "data". Data maps to JSON-array of requested data (device + id), "status" can be "fail" or "success"
     */
    QJsonObject getStates();

    /*!
     * \brief function for adding device to tracking
     * \param device - device to add
     * \return JSON-object with "status", that can be "fail" or "success". and "cause" if "status" is "fail".
     */
    QJsonObject addDevice(device device);

    /*!
     * \brief function for editing existing device
     * \param device - id of device to edit
     * \param device - new data of device
     * \return JSON-object with "status", that can be "fail" or "success". and "cause" if "status" is "fail".
     */
    QJsonObject editDevice(int id, device device);
    /*!
     * \brief function for deleting devices from tracking
     * \param id
     * \return JSON-object with "status", that can be "fail" or "success". and "cause" if "status" is "fail".
     */
    QJsonObject deleteDevice(int id);

    /*!
     * \brief function for completely deletig all trackable devices and replacing them with new ones
     * \param devices - vector of devices to add
     * \return JSON-object with "status", that can be "fail" or "success". and "cause" if "status" is "fail".
     */
    QJsonObject changeDevices(std::vector<device> devices);

    QJsonObject updateState(int id, State state);

private:

    static db_repository* instance;
};

#endif // DB_REPOSITORY_H
