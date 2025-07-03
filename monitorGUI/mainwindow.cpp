#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "deviceinfomodel.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QByteArray MainWindow::buffer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->tableView->setModel(new DeviceInfoModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::test() {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    connect(socket, &QTcpSocket::connected, this, [socket]() {
        QJsonObject requestObj;
        requestObj["data"] = "test";
        QJsonDocument requestDoc(requestObj);
        socket->write(requestDoc.toJson());
        socket->flush();
    });

    connect(socket, &QTcpSocket::readyRead, this, [socket]() {
        buffer.append(socket->readAll());
    });

    connect(socket, &QTcpSocket::disconnected, this, [this, socket]() {
        QJsonObject firstItem = QJsonDocument::fromJson(buffer).object().value("data").toArray().at(0).toObject();
        QString id = QString::number(firstItem.value("id").toInt());
        ui->label->appendPlainText(QString("%1").arg(id));

        buffer.clear();
        socket->deleteLater();
    });

}

void MainWindow::on_pushButton_clicked()
{
    test();
}

