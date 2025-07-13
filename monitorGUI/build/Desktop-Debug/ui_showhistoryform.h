/********************************************************************************
** Form generated from reading UI file 'showhistoryform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWHISTORYFORM_H
#define UI_SHOWHISTORYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showHistoryForm
{
public:
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBox;
    QDateTimeEdit *dateTimeEdit;
    QCheckBox *checkBox_2;
    QDateTimeEdit *dateTimeEdit_2;
    QCheckBox *checkBox_3;
    QComboBox *comboBox;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *showHistoryForm)
    {
        if (showHistoryForm->objectName().isEmpty())
            showHistoryForm->setObjectName(QString::fromUtf8("showHistoryForm"));
        showHistoryForm->resize(1131, 617);
        label = new QLabel(showHistoryForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 570, 16, 41));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        verticalLayoutWidget = new QWidget(showHistoryForm);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 1111, 591));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(verticalLayoutWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        checkBox = new QCheckBox(verticalLayoutWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        horizontalLayout->addWidget(checkBox);

        dateTimeEdit = new QDateTimeEdit(verticalLayoutWidget);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));

        horizontalLayout->addWidget(dateTimeEdit);

        checkBox_2 = new QCheckBox(verticalLayoutWidget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(checkBox_2);

        dateTimeEdit_2 = new QDateTimeEdit(verticalLayoutWidget);
        dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));

        horizontalLayout->addWidget(dateTimeEdit_2);

        checkBox_3 = new QCheckBox(verticalLayoutWidget);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        horizontalLayout->addWidget(checkBox_3);

        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_2->addWidget(pushButton_2);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_2->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(showHistoryForm);

        QMetaObject::connectSlotsByName(showHistoryForm);
    } // setupUi

    void retranslateUi(QWidget *showHistoryForm)
    {
        showHistoryForm->setWindowTitle(QCoreApplication::translate("showHistoryForm", "Form", nullptr));
        label->setText(QString());
        checkBox->setText(QCoreApplication::translate("showHistoryForm", "\320\241", nullptr));
        checkBox_2->setText(QCoreApplication::translate("showHistoryForm", "\320\237\320\276", nullptr));
        checkBox_3->setText(QCoreApplication::translate("showHistoryForm", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", nullptr));
        pushButton_2->setText(QCoreApplication::translate("showHistoryForm", "\320\237\320\265\321\207\320\260\321\202\321\214", nullptr));
        pushButton->setText(QCoreApplication::translate("showHistoryForm", "\320\227\320\260\320\277\321\200\320\276\321\201\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showHistoryForm: public Ui_showHistoryForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWHISTORYFORM_H
