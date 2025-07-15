#ifndef ADDDEVICEFORM_H
#define ADDDEVICEFORM_H

#include <QDialog>
#include "device.h"
namespace Ui {
class addDeviceForm;
}

class addDeviceForm : public QDialog
{
    Q_OBJECT

public:
    explicit addDeviceForm(QDialog *parent = nullptr);
    addDeviceForm(QString serial, QString name, QString description, bool type, QDialog *parent = nullptr);
    bool getType();
    QString getName();
    QString getDescription();
    QString getSerial();
    ~addDeviceForm();
    bool discarded;

    /*!
     * \brief returns device written in fields of form
     * \return object of class device
     */
    device getDevice();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addDeviceForm *ui;
};

#endif // ADDDEVICEFORM_H
