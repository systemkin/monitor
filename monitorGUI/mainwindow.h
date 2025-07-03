#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./ui_mainwindow.h"

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
signals:
    void requestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
private slots:
    void on_pushButton_clicked();
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
private:
    QString host;
    int port;
    Ui::MainWindow *ui;

    void makeJsonRequest(QJsonObject requestObject);
};
#endif // MAINWINDOW_H
