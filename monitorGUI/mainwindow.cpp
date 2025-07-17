#include "mainwindow.h"
#include "adddeviceform.h"
#include "configreader.h"
#include "deviceinfomodel.h"
#include "showhistoryform.h"
#include "states.h"


#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QDomDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new DeviceInfoModel();
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setColumnWidth(1, 200);



    configReader reader;
    config = reader.get("/home/alexej/monitor/configGUI.xml");

    client = new tcpClient(config.host, config.port);
    connect(client, &tcpClient::requestCompleted,
            this, &MainWindow::onRequestCompleted);

    connect(client, &tcpClient::errorOccurred, this, &MainWindow::onConnectionError);

    QJsonObject requestObj;
    requestObj["requestType"] = "getStates";
    client->makeJsonRequest(requestObj);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{

    QJsonObject requestObj;
    requestObj["requestType"] = "getStates";


    client->makeJsonRequest(requestObj);
}
void MainWindow::onConnectionError() {
    QMessageBox::critical(nullptr, "Ошибка","Не удалось получить информацию от сервера",QMessageBox::Ok);
}
void MainWindow::onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc) {
    QJsonObject responseObject = responseDoc.object();
    if (responseObject["status"] != "success") {
        qDebug() << responseObject;
        QMessageBox::critical(nullptr, "Ошибка","Не удалось получить информацию от сервера",QMessageBox::Ok);
        return;
    }
    if (requestObject["requestType"] == "getStates") {

        QJsonObject response = responseDoc.object();
        QJsonArray responseArray = response.value("data").toArray();
        std::vector<deviceInfo> container = std::vector<deviceInfo>(responseArray.size());
        int i = 0;
        for (const QJsonValue &value : std::as_const(responseArray)) {
            deviceInfo device = deviceInfo(value.toObject());
            container[i++] = device;
        }
        delete model;
        model = new DeviceInfoModel(container.size(), 6, container);
        ui->tableView->setModel(model);
    } else if (requestObject["requestType"] == "addDevice") {
        QJsonObject response = responseDoc.object();
        if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация","Успешно добавлено",QMessageBox::Ok);
        return;
        if ((response.value("status") == "fail") && ((response.value("message") == "Already exists") ))
            QMessageBox::critical(nullptr, "Ошибка","Серийный номер уже отслеживается",QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "editDevice") {
        QJsonObject response = responseDoc.object();
        if ((response.value("status") == "fail") && ((response.value("message") == "Already exists") ))
            QMessageBox::critical(nullptr, "Ошибка","Серийный номер уже отслеживается",QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "deleteDevice") {
        QJsonObject response = responseDoc.object();
        if ((response.value("status") == "success") && (response.value("message") == "Already deleted"))
            QMessageBox::information(nullptr, "Информация","Уже удалено (обновите чтобы увидеть изменения)",QMessageBox::Ok);
        else if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация", "Успешно удалено", QMessageBox::Ok);
        return;

    } else if (requestObject["requestType"] == "changeDevices") {
        QJsonObject response = responseDoc.object();
        if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация","Успешно",QMessageBox::Ok);
        else if ((response.value("status") == "fail") &&  (response.value("message") == "Already deleted"))
            QMessageBox::information(nullptr, "Информация","Непредвиденная ошибка, проверьте файл", QMessageBox::Ok);
        return;

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
    QJsonObject device = form.getDevice().toJson();

    requestObj["requestData"] = device;
    client->makeJsonRequest(requestObj);
}


void MainWindow::on_buttonChange_clicked()
{

    QList<QModelIndex> list( ui->tableView->selectionModel()->selectedRows() );
    if(list.size() != 1) {
        QMessageBox::information(nullptr, "Информация","Выделите строку",QMessageBox::Ok);
        return;
    }
    int row = list.at(0).row();
    deviceInfo deviceInfo = model->at(row);

    addDeviceForm form = addDeviceForm(deviceInfo.serial, deviceInfo.name, deviceInfo.description, deviceInfo.type);
    form.exec();
    if (form.discarded) return;

    QJsonObject requestObj;
    requestObj["requestType"] = "editDevice";

    QJsonObject requestData;
    requestData["device"] = form.getDevice().toJson();
    requestData["id"] = deviceInfo.id;


    requestObj["requestData"] = requestData;
    client->makeJsonRequest(requestObj);
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
    client->makeJsonRequest(requestObj);
}


void MainWindow::on_buttonAdd_2_clicked()
{
    showHistoryForm *form = new showHistoryForm(client, this);
    form->exec();
}

void MainWindow::on_buttonExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Выберите файл",
        QDir::homePath(),
        "Monitor file (*.mof)"
    );

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::critical(nullptr, "Ошибка","Не удалось открыть файл",QMessageBox::Ok);
            return;
        }
        std::vector<deviceInfo> cont = model->getContainerCopy();
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        out << static_cast<quint32>(cont.size());

        for (const auto& device : cont) {
            out << device.serial
                << device.name
                << device.description
                << device.type;
        }

        file.close();
        return;
    }


}


void MainWindow::on_buttonAdd_3_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::critical(this, "Внимание", "Это действие удалит существующие данные", QMessageBox::Ok | QMessageBox::No);
    if (reply != QMessageBox::Ok) {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
        "Выберите файл",
        QDir::homePath(),
        "Monitor file (*.mof)"
    );

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Ошибка","Не удалось открыть файл",QMessageBox::Ok);
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    quint32 size;
    in >> size;


    QJsonObject requestObj;
    requestObj["requestType"] = "changeDevices";
    QJsonArray requestData;

    for (quint32 i = 0; i < size; ++i) {
        QString serial;
        QString name;
        QString description;
        bool type;
        in >> serial
            >> name
            >> description
            >> type;
        device device(serial, name, description, type);
        QJsonObject obj = device.toJson();
        requestData.append(obj);
    }

    file.close();

    requestObj["requestData"] = requestData;
    qDebug() << requestObj;
    client->makeJsonRequest(requestObj);
    return;
}


void MainWindow::on_buttonExport_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Выберите файл",
        QDir::homePath(),
        "PDF документ (*.pdf);;Текстовый файл (.txt);;Excel-файл (.csv);;Html-страница (.html)"
    );
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(nullptr, "Ошибка","Не удалось открыть файл",QMessageBox::Ok);
        return;
    }

    if (fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        exportToPDF(fileName, model->getContainerCopy());
    } else if (fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        exportToText(fileName, model->getContainerCopy());
    } else if (fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        exportToCSV(fileName, model->getContainerCopy());
    } else if (fileName.endsWith(".html", Qt::CaseInsensitive)) {
        exportToHTML(fileName, model->getContainerCopy());
    } else {
        QMessageBox::warning(this, "Ошибка", "Неизвестный формат файла");
        return;
    }
}
void MainWindow::exportToPDF(const QString &fileName, const std::vector<deviceInfo>& container)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    QString html = generateHTMLTable(container);
    doc.setHtml(html);

    doc.print(&printer);
    QMessageBox::information(this, "Информация", "Данные экспортированы в PDF");
}

void MainWindow::exportToText(const QString &fileName, const std::vector<deviceInfo>& container)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << QString::fromUtf8("id\tСерийный номер\tНазвание\tОписание\tТип\tСостояние\n");

    for (const auto &device : container) {

        out << device.toText();
    }

    file.close();
    QMessageBox::information(this, "Мнформация", "Данные экспортированы в текстовый файл");
}


void MainWindow::exportToCSV(const QString &fileName, const std::vector<deviceInfo>& container)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);
    out.setCodec("UTF-8");

    out << QString::fromUtf8("id,Серийный номер,Наименование,Описание,Тип,Состояние\n");

    for (const auto &device : container) {

        out << device.toCSV();
    }

    file.close();
    QMessageBox::information(this, "Успех", "Данные экспортированы в CSV");
}

void MainWindow::exportToHTML(const QString &fileName, const std::vector<deviceInfo>& container)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << generateHTMLTable(container);

    file.close();
    QMessageBox::information(this, "Успех", "Данные экспортированы в HTML");
}

QString MainWindow::generateHTMLTable(const std::vector<deviceInfo>& container)
{
    QString html;
    html += "<!DOCTYPE html>\n<html>\n<head>\n"
            "<meta charset=\"UTF-8\">\n"
            "<title>Device List</title>\n"
            "<style>table {border-collapse: collapse;} "
            "th, td {border: 1px solid black; padding: 5px;}</style>\n"
            "</head>\n<body>\n"
            "<h1>Список устройств</h1>\n"
            "<table>\n"
            "<tr><th>ID</th><th>Серийный номер</th><th>Название</th>"
            "<th>Описание</th><th>Тип</th><th>Состояние</th></tr>\n";

    for (const auto &device : container) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td>"
                        "<td>%4</td><td>%5</td><td>%6</td></tr>\n")
                    .arg(device.id)
                    .arg(device.serial.toHtmlEscaped())
                    .arg(device.name.toHtmlEscaped())
                    .arg(device.description.toHtmlEscaped())
                    .arg(device.type ? "bash" : "Файл")
                    .arg(stateToString(device.state));
    }

    html += "</table>\n</body>\n</html>";
    return html;
}
