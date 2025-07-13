#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class addDeviceDialog;
}
QT_END_NAMESPACE

class addDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDeviceDialog(QWidget *parent = nullptr);
    ~addDeviceDialog();

private:
    Ui::addDeviceDialog *ui;
};

#endif // ADDDEVICEDIALOG_H
