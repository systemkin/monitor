#include "mainwindow.h"
#include "adddeviceform.h"
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

    QFile config("C:\\Users\\alexej\\Documents\\monitor\\configGUI.xml");
    if (!config.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open config file";
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&config)) {
        qDebug() << "Failed to parse XML config file";
        config.close();
        return;
    }
    config.close();

    QDomElement root = doc.documentElement();
    QDomElement connection = root.firstChildElement("connection");

    if (connection.isNull()) {
        qDebug() << "No field 'connection' in file";
        return;
    }
    QDomElement hostElem = connection.firstChildElement("host");
    QDomElement portElem = connection.firstChildElement("port");

    if (!hostElem.isNull()) {
        host = hostElem.text();
    } else {
        qDebug() << "No field 'connection.host' in file";
        return;
    }

    if (portElem.isNull()) {
        qDebug() << "No field 'connection.port' in file";
        return;
    }

    bool ok;
    port = portElem.text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid field 'connection.port in file";
        return;
    }

    connect(this, &MainWindow::requestCompleted, this, &MainWindow::onRequestCompleted);

    ui->setupUi(this);
    model = new DeviceInfoModel();
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setColumnWidth(1, 200);

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
            int state = deviceJson.value("state").toInt();
            deviceInfo device({id, serial, name, description, type, state});
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
        if (response.value("status") == "success")
            QMessageBox::information(nullptr, "Информация","Успешно удалено",QMessageBox::Ok);
        else if ((response.value("status") == "fail") &&  (response.value("message") == "Already deleted"))
            QMessageBox::information(nullptr, "Информация","Уже удалено (обновите чтобы увидеть изменения)",QMessageBox::Ok);
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
        deviceInfo device;
        in >> device.serial
            >> device.name
            >> device.description
            >> device.type;

        QJsonObject obj;
        obj["serial"] = device.serial;
        obj["name"] = device.name;
        obj["description"] = device.description;
        obj["type"] = device.type;

        requestData.append(obj);
    }

    file.close();

    requestObj["requestData"] = requestData;
    makeJsonRequest(requestObj);
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

        out << device.id << "\t"
            << device.serial << "\t"
            << device.name << "\t"
            << device.description << "\t"
            << (device.type ?  QString::fromUtf8("bash") :  QString::fromUtf8("Файл")) << "\t"
            << stateToString(device.state) << "\n";
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

        out << "\"" << device.id << "\","
            << "\"" << device.serial << "\","
            << "\"" << device.name << "\","
            << "\"" << device.description << "\","
            << "\"" << (device.type ? "bash" : QString::fromUtf8("Файл")) << "\","
            << "\"" << stateToString(device.state) << "\"\n";
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
