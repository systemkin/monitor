#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
struct Database{
    QString host = "";
    QString name = "";
    QString username = "";
    QString password = "";
    int port = 0;
};
struct Control {

    /*!
     * \brief time in miliseconds between refreshes
     */
    int time = 0;

    /*!
     * \brief path to bash script
     */
    QString bash = "";

    /*!
     * \brief path to file with states
     */
    QString file = "";
};
struct Config {
    Database database;
    Control control;

    /*!
     * \brief port for this server
     */
    int port;
};
/*!
 * \brief class for readling config of server application
 */
class configReader : QObject
{
    Q_OBJECT
public:
    /*!
     * \brief constructor
     */
    configReader();

    /*!
     * \brief get
     * \param function to read config from file
     * \return readed config
     */
    Config get(QString configFile);
};

#endif // CONFIGREADER_H
