/********************************************************************************
** Form generated from reading UI file 'adddeviceform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDEVICEFORM_H
#define UI_ADDDEVICEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addDeviceForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QLabel *label_5;

    void setupUi(QWidget *addDeviceForm)
    {
        if (addDeviceForm->objectName().isEmpty())
            addDeviceForm->setObjectName(QString::fromUtf8("addDeviceForm"));
        addDeviceForm->resize(439, 197);
        formLayoutWidget = new QWidget(addDeviceForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 20, 421, 161));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(formLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_2 = new QLineEdit(formLayoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_2);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_3 = new QLineEdit(formLayoutWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_3);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        pushButton = new QPushButton(formLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        formLayout->setWidget(5, QFormLayout::LabelRole, pushButton);

        pushButton_2 = new QPushButton(formLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButton_2);

        comboBox = new QComboBox(formLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, comboBox);

        label_5 = new QLabel(addDeviceForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(170, 0, 121, 16));

        retranslateUi(addDeviceForm);

        QMetaObject::connectSlotsByName(addDeviceForm);
    } // setupUi

    void retranslateUi(QWidget *addDeviceForm)
    {
        addDeviceForm->setWindowTitle(QCoreApplication::translate("addDeviceForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("addDeviceForm", "\320\241\320\265\321\200\320\270\320\271\320\275\321\213\320\271 \320\275\320\276\320\274\320\265\321\200", nullptr));
        label_2->setText(QCoreApplication::translate("addDeviceForm", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        label_3->setText(QCoreApplication::translate("addDeviceForm", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265", nullptr));
        label_4->setText(QCoreApplication::translate("addDeviceForm", "\320\242\320\270\320\277", nullptr));
        pushButton->setText(QCoreApplication::translate("addDeviceForm", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButton_2->setText(QCoreApplication::translate("addDeviceForm", "\320\236\320\272", nullptr));
        label_5->setText(QCoreApplication::translate("addDeviceForm", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\276\320\261\320\276\321\200\321\203\320\264\320\276\320\262\320\260\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class addDeviceForm: public Ui_addDeviceForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDEVICEFORM_H
