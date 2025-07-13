#include "monitor_db.h"
#include <QDebug>

QMutex MonitorDB::instanceMutex;
MonitorDB* MonitorDB::instance = nullptr;



MonitorDB* MonitorDB::getInstance(const QString& host, const QString& name, const QString& username, const QString& password, const int port)
{
    QMutexLocker locker(&instanceMutex);

    if (!instance) instance = new MonitorDB(host, name, username, password, port);
    return instance;

}

MonitorDB::MonitorDB(const QString& host, const QString& name, const QString& username, const QString& password, int port, QObject *parent) : QObject(parent) {
    dbConnection = QSqlDatabase::addDatabase("QPSQL");
    dbConnection.setHostName(host);
    dbConnection.setDatabaseName(name);
    dbConnection.setUserName(username);
    dbConnection.setPassword(password);
    dbConnection.setPort(port);

    dbConnection.open();
}

MonitorDB::~MonitorDB()
{
    QMutexLocker locker(&instanceMutex);
    QMutexLocker locker2(&dbMutex);
    if (instance) {
        delete instance;
    }
    dbConnection.close();
}

QJsonObject MonitorDB::executeQuery(const QString& query, const QVariantList& params)
{
    QMutexLocker locker(&dbMutex);
    QJsonObject result;

    if (!dbConnection.isOpen()) {
        result["status"] = "error";
        result["message"] = "Database connection is not open";
        return result;
    }

    QSqlQuery sql(dbConnection);
    sql.prepare(query);

    for (int i = 0; i < params.size(); ++i) {
        sql.bindValue(i, params[i]);
    }

    if (!sql.exec()) {
        result["status"] = "error";
        result["message"] = sql.lastError().text();
        return result;
    }
    result["status"] = "success";

    if (!sql.isSelect()) return result;

    QJsonArray queryResult;
    while (sql.next()) {
        QJsonObject line;
        for (int i = 0; i < sql.record().count(); ++i) {
            line[sql.record().fieldName(i)] = QJsonValue::fromVariant(sql.value(i));
        }
        queryResult.append(line);
    }
    result["data"] = queryResult;
    result["rowsAffected"] = sql.numRowsAffected();


    return result;
}

bool MonitorDB::beginTransaction()
{
    QMutexLocker locker(&dbMutex);
    if (!dbConnection.isOpen()) return false;

    QSqlQuery sql(dbConnection);
    return sql.exec("BEGIN");
}

bool MonitorDB::commitTransaction()
{
    QMutexLocker locker(&dbMutex);
    if (!dbConnection.isOpen()) return false;

    QSqlQuery sql(dbConnection);
    return sql.exec("COMMIT");
}

bool MonitorDB::rollbackTransaction()
{
    QMutexLocker locker(&dbMutex);
    if (!dbConnection.isOpen()) return false;

    QSqlQuery sql(dbConnection);
    return sql.exec("ROLLBACK");
}
