#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./ui_mainwindow.h"
#include "deviceinfomodel.h"
#include "tcpcommunicator.h"

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getHost();
    int getPort();
signals:
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
private slots:
    void on_pushButton_clicked();
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
    void on_buttonAdd_clicked();

    void on_buttonChange_clicked();

    void on_buttonDelete_clicked();

    void on_buttonAdd_2_clicked();

    void on_buttonExport_clicked();

    void on_buttonAdd_3_clicked();

    void on_buttonExport_2_clicked();

private:
    QString host;
    int port;
    Ui::MainWindow *ui;

    void makeJsonRequest(QJsonObject requestObject);

    DeviceInfoModel* model;

    void saveToFile(const std::vector<deviceInfo>& container, const QString& filename);
    void exportToPDF(const QString &fileName, const std::vector<deviceInfo>& container);
    void exportToText(const QString &fileName, const std::vector<deviceInfo>& container);
    void exportToCSV(const QString &fileName, const std::vector<deviceInfo>& container);
    void exportToHTML(const QString &fileName, const std::vector<deviceInfo>& container);
    QString generateHTMLTable(const std::vector<deviceInfo>& container);
};
#endif // MAINWINDOW_H
