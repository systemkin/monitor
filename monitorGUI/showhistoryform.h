#ifndef SHOWHISTORYFORM_H
#define SHOWHISTORYFORM_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class showHistoryForm;
}

class showHistoryForm : public QDialog
{
    Q_OBJECT

public:
    explicit showHistoryForm(MainWindow *parent = nullptr);
    ~showHistoryForm();

private slots:
    void on_pushButton_clicked();
signals:
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
private:
    QString host;
    int port;
    Ui::showHistoryForm *ui;
    void makeJsonRequest(QJsonObject requestObject);
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
};

#endif // SHOWHISTORYFORM_H
