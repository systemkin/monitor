#ifndef REFRESHERLIB_H
#define REFRESHERLIB_H

#include "refresherLib_global.h"
#include <QObject>
#include <QTimer>
#include "monitor_db.h"
#include <QFile>
#include "states.h"
/*!
 * \brief class for refreshing states in database. Can be configured via constructor and start(int)
 */
class REFRESHERLIB_EXPORT  Refresher : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief constructor
     * \param path to file with states
     * \param path to bash script
     * \param parent - Qt thing
     */
    explicit Refresher(QString dataPath, QString bashPath, QObject* parent);
    /*!
     * \brief start timer for refreshing
     * \param time - time in ms
     */
    void start(int time);
    /*!
     * \brief stop refreshing
     */
    void stop();
    ~Refresher();

private slots:
    /*!
     * \brief function, called on timer to refresh states on db for tracking devices
     */
    void refreshStates();

private:
    /*!
     * \brief time in miliseconds between refreshes
     */
    QTimer* timer;

    /*!
     * \brief path to file with states
     */
    QString dataPath;

    /*!
     * \brief path to bash script
     */
    QString bashPath;

    /*!
     * \brief function for getting state from data file or bash, depends on "type"
     * \param serial - serial number of device
     * \param type - type of device
     * \return current state of device
     */
    State getState(QString serial, bool type);
};

#endif // REFRESHERLIB_H
