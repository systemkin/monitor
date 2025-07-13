#ifndef SHOWHISTORYFORM_H
#define SHOWHISTORYFORM_H

#include <QDialog>
#include "mainwindow.h"
#include "showhistorymodel.h"
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
    void on_pushButton_2_clicked();

signals:
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
private:
    QString host;
    int port;
    Ui::showHistoryForm *ui;
    void makeJsonRequest(QJsonObject requestObject);
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
    std::vector<historyItem> container;
    showHistoryModel* model;

    void saveToFile(const std::vector<historyItem>& container, const QString& filename);
    void exportToPDF(const QString &fileName, const std::vector<historyItem>& container);
    void exportToText(const QString &fileName, const std::vector<historyItem>& container);
    void exportToCSV(const QString &fileName, const std::vector<historyItem>& container);
    void exportToHTML(const QString &fileName, const std::vector<historyItem>& container);
    QString generateHTMLTable(const std::vector<historyItem>& container);

    std::vector<historyItem> getContainerCopy();
};

#endif // SHOWHISTORYFORM_H
