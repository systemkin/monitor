#ifndef MONITOR_DB_H
#define MONITOR_DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QMutex>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#if defined(MONITOR_DB_BUILD)
#define MONITOR_DB_EXPORT Q_DECL_EXPORT
#else
#define MONITOR_DB_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \brief class that directly communicates with database
 */
class MONITOR_DB_EXPORT MonitorDB : public QObject
{
    Q_OBJECT

public:

    /*!
     * \brief constructor
     * \param host - db host adress
     * \param name - db name
     * \param username - db username
     * \param password - db password
     * \param port - dp port
     * \param parent
     */
    explicit MonitorDB(const QString& host, const QString& name, const QString& username, const QString& password, const int port, QObject *parent = nullptr);

    ~MonitorDB();


    /*!
     * \brief function to execure query
     * \param query - QString SQL query
     * \param params - list of query parameters
     * \return JSON object of database response
     */
    QJsonObject executeQuery(const QString& query, const QVariantList& params = QVariantList());

private:

    /*!
     * \brief mutex for executing queries
     */
    QMutex dbMutex;

    /*!
     * \brief connection to database
     */
    QSqlDatabase dbConnection;
};

#endif // MONITOR_DB_H
