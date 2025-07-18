#include "adddeviceform.h"
#include "ui_adddeviceform.h"
#include <QMessageBox>
addDeviceForm::addDeviceForm(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::addDeviceForm)
{
    ui->setupUi(this);
    ui->comboBox->addItems({"bash", "Файл"});

}

addDeviceForm::addDeviceForm(QString serial, QString name, QString description, bool type, QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::addDeviceForm) {
    ui->setupUi(this);
    if (type)
        ui->comboBox->setCurrentIndex(0);
    else ui->comboBox->setCurrentIndex(1);
    ui->lineEdit_serial->setText(serial);
    ui->lineEdit_name->setText(name);
    ui->lineEdit_description->setText(description);

    ui->comboBox->addItems({"bash", "Файл"});
    if (type) ui->comboBox->setCurrentIndex(0);
    else ui->comboBox->setCurrentIndex(1);
}


addDeviceForm::~addDeviceForm()
{
    delete ui;
}
bool addDeviceForm::getType() {
    return (ui->comboBox->currentIndex() == 0);
}
QString addDeviceForm::getSerial() {
    return ui->lineEdit_serial->text();
}
QString addDeviceForm::getName() {
    return ui->lineEdit_name->text();
}
QString addDeviceForm::getDescription() {
    return ui->lineEdit_description->text();
}
device addDeviceForm::getDevice() {
    return device(this->getSerial(), this->getName(), this->getDescription(), this->getType());
}

void addDeviceForm::on_pushButton_ok_clicked()
{
    if (this->getName().contains(",") || this->getSerial().contains(",") || this->getDescription().contains(",")) {
        QMessageBox::critical(this, "Ошибка", "Наименование, описание или серийный номер не может содержать символ ','");
        return;
    }
    discarded = false;
    this->close();
}


void addDeviceForm::on_pushButton_no_clicked()
{
    discarded = true;
    this->close();
}

