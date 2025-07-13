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

class MONITOR_DB_EXPORT MonitorDB : public QObject
{
    Q_OBJECT

public:
    static MonitorDB* getInstance(const QString& host, const QString& name, const QString& username, const QString& password, const int port);

    QJsonObject executeQuery(const QString& query, const QVariantList& params = QVariantList());

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private:
    explicit MonitorDB(const QString& host, const QString& name, const QString& username, const QString& password, const int port, QObject *parent = nullptr);
    ~MonitorDB();

    static QMutex instanceMutex;
    static MonitorDB* instance;

    QMutex dbMutex;
    QSqlDatabase dbConnection;

};

#endif // MONITOR_DB_H
