#include "adddeviceform.h"
#include "ui_adddeviceform.h"

addDeviceForm::addDeviceForm(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::addDeviceForm)
{
    ui->setupUi(this);
    ui->spinBox->setMaximum(1);
    ui->spinBox->setMinimum(0);
}

addDeviceForm::addDeviceForm(QString serial, QString name, QString description, bool type, QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::addDeviceForm) {
    ui->setupUi(this);
    ui->spinBox->setValue(type);
    ui->lineEdit->setText(serial);
    ui->lineEdit_2->setText(name);
    ui->lineEdit_3->setText(description);
}


addDeviceForm::~addDeviceForm()
{
    delete ui;
}
bool addDeviceForm::getType() {
    return ui->spinBox->value();
}
QString addDeviceForm::getSerial() {
    return ui->lineEdit->text();
}
QString addDeviceForm::getName() {
    return ui->lineEdit_2->text();
}
QString addDeviceForm::getDescription() {
    return ui->lineEdit_3->text();
}

void addDeviceForm::on_pushButton_2_clicked()
{
    discarded = false;
    this->close();
}


void addDeviceForm::on_pushButton_clicked()
{
    discarded = true;
    this->close();
}

