#include "mainwindow.h"
#include "adddeviceform.h"
#include "deviceinfomodel.h"
#include "showhistoryform.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    host = "127.0.0.1";
    port = 12345;

    connect(this, &MainWindow::requestCompleted, this, &MainWindow::onRequestCompleted);

    ui->setupUi(this);
    model = new DeviceInfoModel();
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    QJsonObject requestObj;
    requestObj["requestType"] = "getStates";
    makeJsonRequest(requestObj);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeJsonRequest(QJsonObject requestObject) {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(host, port);

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
    QJsonObject requestObj;
    requestObj["requestType"] = "getStates";
    makeJsonRequest(requestObj);
}

void MainWindow::onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc) {
    if (requestObject["requestType"] == "getStates") {
        QJsonObject response = responseDoc.object();
        QJsonArray responseArray = response.value("data").toArray();
        std::vector<deviceInfo> container = std::vector<deviceInfo>(responseArray.size());
        int i = 0;
        for (const QJsonValue &value : std::as_const(responseArray)) {


            QJsonObject deviceJson = value.toObject();
            int id = deviceJson.value("id").toInt();
            QString serial = deviceJson.value("serial").toString();
            QString name = deviceJson.value("name").toString();
            QString description = deviceJson.value("description").toString();
            bool type = deviceJson.value("type").toBool();
            int state = deviceJson.value("state").toString().toInt();
            deviceInfo device({id, serial, name, description, type, state});
            container[i++] = device;
        }
        model = new DeviceInfoModel(container.size(), 6, container);
        ui->tableView->setModel(model);
    } else if (requestObject["requestType"] == "addDevice") {
        QJsonObject response = responseDoc.object();
        if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация","Успешно добавлено",QMessageBox::Ok);
        return;
        if ((response.value("status") == "fail") && ((response.value("cause") == "Already exists") ))
            QMessageBox::critical(nullptr, "Ошибка","Серийный номер уже отслеживается",QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "editDevice") {
        QJsonObject response = responseDoc.object();
        if ((response.value("status") == "fail") && ((response.value("cause") == "Already exists") ))
            QMessageBox::critical(nullptr, "Ошибка","Серийный номер уже отслеживается",QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "deleteDevice") {
        QJsonObject response = responseDoc.object();
        if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация","Успешно удалено",QMessageBox::Ok);
        else if ((response.value("status") == "fail") &&  (response.value("cause") == "Already deleted"))
            QMessageBox::information(nullptr, "Информация","Уже удалено (обновите чтобы увидеть изменения)",QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "changeDevices") {


    } else if (requestObject["requestType"] == "showHistory") {


    }

}

void MainWindow::on_buttonAdd_clicked()
{
    addDeviceForm form = addDeviceForm();
    form.exec();
    if (form.discarded) return;

    QJsonObject requestObj;
    requestObj["requestType"] = "addDevice";
    QJsonObject device;
    device["name"] = form.getName();
    device["description"] = form.getDescription();
    device["serial"] = form.getSerial();
    device["type"] = form.getType();
    requestObj["requestData"] = device;
    makeJsonRequest(requestObj);
}


void MainWindow::on_buttonChange_clicked()
{

    QList<QModelIndex> list( ui->tableView->selectionModel()->selectedRows() );
    if(list.size() != 1) {
        return;
    }
    int row = list.at(0).row();
    deviceInfo deviceInfo = model->at(row);


    addDeviceForm form = addDeviceForm(deviceInfo.serial, deviceInfo.name, deviceInfo.description, deviceInfo.type);
    form.exec();
    if (form.discarded) return;

    QJsonObject requestObj;
    requestObj["requestType"] = "editDevice";
    QJsonObject device;
    device["id"] = deviceInfo.id;
    device["name"] = form.getName();
    device["description"] = form.getDescription();
    device["serial"] = form.getSerial();
    device["type"] = form.getType();
    requestObj["requestData"] = device;
    makeJsonRequest(requestObj);
}

void MainWindow::on_buttonDelete_clicked()
{
    QList<QModelIndex> list( ui->tableView->selectionModel()->selectedRows() );
    int row = list.at(0).row();
    deviceInfo deviceInfo = model->at(row);
    QJsonObject requestObj;
    requestObj["requestType"] = "deleteDevice";
    QJsonObject device;
    device["id"] = deviceInfo.id;
    requestObj["requestData"] = device;
    makeJsonRequest(requestObj);
}


void MainWindow::on_buttonAdd_2_clicked()
{
    showHistoryForm *form = new showHistoryForm(this);
    form->exec();
}

QString MainWindow::getHost() {
    return host;
}
int MainWindow::getPort() {
    return port;
}
