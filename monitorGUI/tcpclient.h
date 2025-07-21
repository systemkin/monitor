#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QMutex>
#include <QAbstractSocket>
/*!
 * \brief class for making JSON requests over tcp
 */
class tcpClient : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief constructor
     * \param host - tcp server host adress
     * \param port - tcp server port
     */
    tcpClient(QString host, int port);
    /*!
     * \brief function for making requests
     * \param requestObject - JSON request object
     */
    void makeJsonRequest(QJsonObject requestObject);
private:
    /*!
     * \brief tcp server host
     */
    QString host;

    /*!
     * \brief tcp server port
     */
    int port;

    /*!
     * \brief mutex for making connection
     */
    QMutex connectionMutex;
signals:
    /*!
     * \brief signal which is emmited after getting all response data from server
     * \param requestObject - object, used in makeJsonRequest(QJsonObject)
     * \param responseDoc - JSON document response from server
     */
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);

    /*!
     * \brief signal which is emitted on connection errors. Usually disconnections or when client cant establist connection
     */
    void errorOccurred();
};

#endif // TCPCLIENT_H
