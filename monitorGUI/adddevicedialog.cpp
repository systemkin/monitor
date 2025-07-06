#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"

addDeviceDialog::addDeviceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDeviceDialog)
{
    ui->setupUi(this);
}

addDeviceDialog::~addDeviceDialog()
{
    delete ui;
}
