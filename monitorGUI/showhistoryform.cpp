#include "showhistoryform.h"
#include "showhistorymodel.h"
#include "ui_showhistoryform.h"

showHistoryForm::showHistoryForm(MainWindow *parent)
    : QDialog(parent)
    , ui(new Ui::showHistoryForm)
{
    ui->setupUi(this);
    ui->tableView->setModel(new showHistoryModel);
    host = parent->getHost();
    port = parent->getPort();

    ui->spinBox->setMaximum(3);
    ui->spinBox->setMinimum(0);

    connect(this, &showHistoryForm::requestCompleted, this, &showHistoryForm::onRequestCompleted);
}

showHistoryForm::~showHistoryForm()
{
    delete ui;
}
void showHistoryForm::makeJsonRequest(QJsonObject requestObject) {
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

void showHistoryForm::onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc) {
    if (requestObject["requestType"] == "showHistory") {
        QJsonObject response = responseDoc.object();
        QJsonArray responseArray = response.value("data").toArray();
                    qDebug() << responseArray;
        std::vector<historyItem> container = std::vector<historyItem>(responseArray.size());
        int i = 0;
        for (const QJsonValue &value : std::as_const(responseArray)) {

            QJsonObject historyItemJson = value.toObject();
            QString serial = historyItemJson.value("serial").toString();
            QString name = historyItemJson.value("name").toString();
            int prevState = historyItemJson.value("prev_state").toString().toInt();
            int newState = historyItemJson.value("new_state").toString().toInt();
            QString dateTime = historyItemJson.value("time").toString();
            historyItem historyItem({serial, name, prevState, newState, dateTime});
            container[i++] = historyItem;

        }
        ui->tableView->setModel(new showHistoryModel(container.size(), 5, container));

    }
}

void showHistoryForm::on_pushButton_clicked()
{
    QJsonObject requestObj;
    requestObj["requestType"] = "showHistory";
    if(ui->checkBox->isChecked()) requestObj["low"] = ui->dateTimeEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    if(ui->checkBox_2->isChecked()) requestObj["high"] = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    if(ui->checkBox_3->isChecked()) requestObj["state"] = ui->spinBox->value();
    makeJsonRequest(requestObj);
}

