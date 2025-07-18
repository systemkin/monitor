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
    /*!
     * \brief aconstructor to create form with predefned values of fields
     * \param serial - serial number of device
     * \param name - name of device
     * \param description - description of device
     * \param type - type of devic
     * \param parent
     */
    addDeviceForm(QString serial, QString name, QString description, bool type, QDialog *parent = nullptr);

    /*!
     * \brief function to retrieve type of device tracking written in field of form
     * \return type true/false => bash/file
     */
    bool getType();

    /*!
     * \brief function to retrieve name of device written in field of form
     * \return written name
     */
    QString getName();

    /*!
     * \brief function to retrieve descripton of device written in field of form
     * \return written descripton
     */
    QString getDescription();

    /*!
     * \brief function to retrieve serial number of device written in field of form
     * \return written serial number
     */
    QString getSerial();
    ~addDeviceForm();
    bool discarded;

    /*!
     * \brief function to retrieve device written in fields of form
     * \return object of class device
     */
    device getDevice();
private slots:

    /*!
     * \brief slot to handle accept button clicked
     */
    void on_pushButton_ok_clicked();

    /*!
     * \brief slot to handle decline button clicked
     */
    void on_pushButton_no_clicked();

private:
    Ui::addDeviceForm *ui;
};

#endif // ADDDEVICEFORM_H
