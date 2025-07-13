#include "showhistoryform.h"

#include "states.h"
#include "ui_showhistoryform.h"
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>

#include <QMessageBox>

showHistoryForm::showHistoryForm(MainWindow *parent)
    : QDialog(parent)
    , ui(new Ui::showHistoryForm)
{
    ui->setupUi(this);
    model = new showHistoryModel;
    ui->tableView->setModel(new showHistoryModel);
    ui->tableView->setColumnWidth(0, 170);
    ui->tableView->setColumnWidth(1, 170);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 300);
    host = parent->getHost();
    port = parent->getPort();
    ui->comboBox->addItems({"Неизвестно", "Не работает", "Авария", "Работает"});

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
            int prevState = historyItemJson.value("prev_state").toInt();
            int newState = historyItemJson.value("new_state").toInt();
            QString dateTime = historyItemJson.value("time").toString();
            historyItem historyItem({serial, name, prevState, newState, dateTime});
            container[i++] = historyItem;

        }
        delete model;
        model = new showHistoryModel(container.size(), 5, container);
        ui->tableView->setModel(model);

    }
}

void showHistoryForm::on_pushButton_clicked()
{
    QJsonObject requestObj;
    requestObj["requestType"] = "showHistory";
    if(ui->checkBox->isChecked()) requestObj["low"] = ui->dateTimeEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    if(ui->checkBox_2->isChecked()) requestObj["high"] = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    if(ui->checkBox_3->isChecked()) requestObj["state"] = ui->comboBox->currentIndex();
    makeJsonRequest(requestObj);
}


void showHistoryForm::on_pushButton_2_clicked()
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
        qDebug() << "1";
        std::vector<historyItem> cont = model->getContainerCopy();
        qDebug() << "2";
        exportToPDF(fileName, cont);
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

void showHistoryForm::exportToPDF(const QString &fileName, const std::vector<historyItem>& container)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    qDebug() << "1";
    QString html = generateHTMLTable(container);
    qDebug() << html;
    doc.setHtml(html);

    doc.print(&printer);
    QMessageBox::information(this, "Информация", "Данные экспортированы в PDF");
}
std::vector<historyItem> showHistoryForm::getContainerCopy() {
    return container;
}
void showHistoryForm::exportToText(const QString &fileName, const std::vector<historyItem>& container)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << QString::fromUtf8("Серийный номер\tНазвание\tПредыдущее состояние\tНовое состояние\tВремя\n");

    for (const auto &device : container) {
        out << device.serial << "\t"
            << device.name << "\t"
            << stateToString(device.prev_state) << "\t"
            << stateToString(device.new_state) << "\t"
            << device.dateTime << "\n";
    }

    file.close();
    QMessageBox::information(this, "Мнформация", "Данные экспортированы в текстовый файл");
}


void showHistoryForm::exportToCSV(const QString &fileName, const std::vector<historyItem>& container)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);
    out.setCodec("UTF-8");

    out << QString::fromUtf8("Серийный номер,Наименование,Предыдущее состояние,Новое состояние,Время\n");

    for (const auto &device : container) {
        out << "\"" << device.serial << "\","
            << "\"" << device.name << "\","
            << "\"" << stateToString(device.prev_state) << "\","
            << "\"" << stateToString(device.new_state) << "\","
            << "\"" << device.dateTime << "\"\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Данные экспортированы в CSV");
}

void showHistoryForm::exportToHTML(const QString &fileName, const std::vector<historyItem>& container)
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

QString showHistoryForm::generateHTMLTable(const std::vector<historyItem>& container)
{
    QString html;
    html += "<!DOCTYPE html>\n<html>\n<head>\n"
            "<meta charset=\"UTF-8\">\n"
            "<title>Device List</title>\n"
            "<style>table {border-collapse: collapse;} "
            "th, td {border: 1px solid black; padding: 5px;}</style>\n"
            "</head>\n<body>\n"
            "<h1>История</h1>\n"
            "<table>\n"
            "<tr><th>Серийный номер</th><th>Название</th>"
            "<th>Предыдущее состояние</th><th>Новое состояние</th><th>Время</th></tr>\n";

    for (const auto &device : container) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td>"
                        "<td>%4</td><td>%5</td></tr>\n")
                    .arg(device.serial.toHtmlEscaped())
                    .arg(device.name.toHtmlEscaped())
                    .arg(stateToString(device.prev_state))
                    .arg(stateToString(device.new_state))
                    .arg(device.dateTime);
    }

    html += "</table>\n</body>\n</html>";
    return html;
}
