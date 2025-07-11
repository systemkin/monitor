#ifndef REFRESHER_H
#define REFRESHER_H

#include <QObject>
#include <QTimer>
#include "monitor_db.h"
#include <QFile>

class Refresher : public QObject
{
    Q_OBJECT
public:
    explicit Refresher(MonitorDB* dbm, QFile* dataFile, QObject* parent = nullptr);
    void start(int time);
    void stop();
    ~Refresher();

private slots:
    void refreshStates();

private:
    QTimer* m_timer;
    MonitorDB* m_dbm;
    QFile* m_dataFile;
    int getState(QString serial, bool type);
};

#endif // REFRESHER_H
