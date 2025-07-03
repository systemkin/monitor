#include "mainwindow.h"
#include "deviceinfomodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    host = "127.0.0.1";
    port = 12345;

    connect(this, &MainWindow::requestCompleted, this, &MainWindow::onRequestCompleted);

    ui->setupUi(this);
    ui->tableView->setModel(new DeviceInfoModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeJsonRequest(QJsonObject requestObject) {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    ui->label_2->appendPlainText(QString("connected to " + host));

    connect(socket, &QTcpSocket::connected, this, [socket, requestObject]() {
        QJsonDocument requestDoc(requestObject);
        QByteArray requestData = requestDoc.toJson();
        QByteArray lengthPrefix = QByteArray::number(requestData.size()) + "\n";

        socket->write(lengthPrefix + requestData);
        socket->flush();
    });

    QByteArray *buffer = new QByteArray();
    connect(socket, &QTcpSocket::readyRead, this, [socket, buffer]() {
        buffer->append(socket->readAll());
    });

    connect(socket, &QTcpSocket::disconnected, this, [this, socket, requestObject, buffer]() {

        emit requestCompleted(requestObject, QJsonDocument::fromJson(*buffer));

        socket->deleteLater();
    });

}

void MainWindow::on_pushButton_clicked()
{
    ui->label_2->appendPlainText(QString("pushedButton "));
    QJsonObject requestObj;
    requestObj["requestType"] = "getStates";
    makeJsonRequest(requestObj);
}

void MainWindow::onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc) {
    if (requestObject["requestType"] == "getStates") {
        QString id = QString(responseDoc.object().value("data").toString());
        ui->label->appendPlainText(id);

    } else if (requestObject["requestType"] == "addDevice") {


    } else if (requestObject["requestType"] == "editDevice") {


    } else if (requestObject["requestType"] == "deleteDevice") {


    } else if (requestObject["requestType"] == "changeDevices") {


    } else if (requestObject["requestType"] == "showHistory") {


    }

}
