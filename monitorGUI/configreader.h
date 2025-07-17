#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
struct Config {
    QString host;
    int port;
};
/*!
 * \brief class for readling config of GUI application
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
