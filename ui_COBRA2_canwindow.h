/********************************************************************************
** Form generated from reading UI file 'COBRA2_canwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COBRA2_CANWINDOW_H
#define UI_COBRA2_CANWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_COBRA2_CANWindow
{
public:
    QGroupBox *CAN_GB;
    QListWidget *lst_VID;
    QLabel *lbl_VID;
    QListWidget *lst_MAB;
    QLabel *lbl_MAB;
    QListWidget *lst_IN1;
    QListWidget *lst_OUT1;
    QLabel *lbl_OUT1;
    QLabel *lbl_IN1;
    QLabel *lbl_IN2;
    QListWidget *lst_IN2;
    QListWidget *lst_ENUM1;
    QLabel *lbl_ENUM1;
    QListWidget *lst_Units;
    QListWidget *lst_IN3;
    QLabel *lbl_IN3;
    QListWidget *lst_OUT2;
    QLabel *lbl_OUT2;
    QListWidget *lst_POTENTIOMETER1;
    QLabel *lbl_POTENTIOMETER1;

    void setupUi(QWidget *COBRA2_CANWindow)
    {
        if (COBRA2_CANWindow->objectName().isEmpty())
            COBRA2_CANWindow->setObjectName(QString::fromUtf8("COBRA2_CANWindow"));
        COBRA2_CANWindow->resize(721, 856);
        CAN_GB = new QGroupBox(COBRA2_CANWindow);
        CAN_GB->setObjectName(QString::fromUtf8("CAN_GB"));
        CAN_GB->setGeometry(QRect(20, 10, 681, 821));
        lst_VID = new QListWidget(CAN_GB);
        lst_VID->setObjectName(QString::fromUtf8("lst_VID"));
        lst_VID->setGeometry(QRect(350, 50, 310, 211));
        lbl_VID = new QLabel(CAN_GB);
        lbl_VID->setObjectName(QString::fromUtf8("lbl_VID"));
        lbl_VID->setGeometry(QRect(350, 30, 31, 16));
        lst_MAB = new QListWidget(CAN_GB);
        lst_MAB->setObjectName(QString::fromUtf8("lst_MAB"));
        lst_MAB->setGeometry(QRect(20, 50, 310, 211));
        lbl_MAB = new QLabel(CAN_GB);
        lbl_MAB->setObjectName(QString::fromUtf8("lbl_MAB"));
        lbl_MAB->setGeometry(QRect(20, 30, 31, 16));
        lst_IN1 = new QListWidget(CAN_GB);
        lst_IN1->setObjectName(QString::fromUtf8("lst_IN1"));
        lst_IN1->setGeometry(QRect(20, 290, 80, 150));
        lst_OUT1 = new QListWidget(CAN_GB);
        lst_OUT1->setObjectName(QString::fromUtf8("lst_OUT1"));
        lst_OUT1->setGeometry(QRect(350, 290, 80, 150));
        lbl_OUT1 = new QLabel(CAN_GB);
        lbl_OUT1->setObjectName(QString::fromUtf8("lbl_OUT1"));
        lbl_OUT1->setGeometry(QRect(350, 270, 31, 16));
        lbl_IN1 = new QLabel(CAN_GB);
        lbl_IN1->setObjectName(QString::fromUtf8("lbl_IN1"));
        lbl_IN1->setGeometry(QRect(20, 270, 21, 16));
        lbl_IN2 = new QLabel(CAN_GB);
        lbl_IN2->setObjectName(QString::fromUtf8("lbl_IN2"));
        lbl_IN2->setGeometry(QRect(136, 270, 21, 16));
        lst_IN2 = new QListWidget(CAN_GB);
        lst_IN2->setObjectName(QString::fromUtf8("lst_IN2"));
        lst_IN2->setGeometry(QRect(136, 290, 80, 150));
        lst_ENUM1 = new QListWidget(CAN_GB);
        lst_ENUM1->setObjectName(QString::fromUtf8("lst_ENUM1"));
        lst_ENUM1->setGeometry(QRect(20, 470, 310, 50));
        lbl_ENUM1 = new QLabel(CAN_GB);
        lbl_ENUM1->setObjectName(QString::fromUtf8("lbl_ENUM1"));
        lbl_ENUM1->setGeometry(QRect(20, 450, 31, 16));
        lst_Units = new QListWidget(CAN_GB);
        lst_Units->setObjectName(QString::fromUtf8("lst_Units"));
        lst_Units->setGeometry(QRect(20, 550, 640, 251));
        lst_IN3 = new QListWidget(CAN_GB);
        lst_IN3->setObjectName(QString::fromUtf8("lst_IN3"));
        lst_IN3->setGeometry(QRect(250, 290, 80, 150));
        lbl_IN3 = new QLabel(CAN_GB);
        lbl_IN3->setObjectName(QString::fromUtf8("lbl_IN3"));
        lbl_IN3->setGeometry(QRect(250, 270, 21, 16));
        lst_OUT2 = new QListWidget(CAN_GB);
        lst_OUT2->setObjectName(QString::fromUtf8("lst_OUT2"));
        lst_OUT2->setGeometry(QRect(460, 290, 80, 150));
        lbl_OUT2 = new QLabel(CAN_GB);
        lbl_OUT2->setObjectName(QString::fromUtf8("lbl_OUT2"));
        lbl_OUT2->setGeometry(QRect(460, 270, 31, 16));
        lst_POTENTIOMETER1 = new QListWidget(CAN_GB);
        lst_POTENTIOMETER1->setObjectName(QString::fromUtf8("lst_POTENTIOMETER1"));
        lst_POTENTIOMETER1->setGeometry(QRect(350, 470, 310, 50));
        lbl_POTENTIOMETER1 = new QLabel(CAN_GB);
        lbl_POTENTIOMETER1->setObjectName(QString::fromUtf8("lbl_POTENTIOMETER1"));
        lbl_POTENTIOMETER1->setGeometry(QRect(350, 450, 91, 16));

        retranslateUi(COBRA2_CANWindow);

        QMetaObject::connectSlotsByName(COBRA2_CANWindow);
    } // setupUi

    void retranslateUi(QWidget *COBRA2_CANWindow)
    {
        COBRA2_CANWindow->setWindowTitle(QCoreApplication::translate("COBRA2_CANWindow", "Form", nullptr));
        CAN_GB->setTitle(QString());
        lbl_VID->setText(QCoreApplication::translate("COBRA2_CANWindow", "VID", nullptr));
        lbl_MAB->setText(QCoreApplication::translate("COBRA2_CANWindow", "MAB", nullptr));
        lbl_OUT1->setText(QCoreApplication::translate("COBRA2_CANWindow", "OUT1", nullptr));
        lbl_IN1->setText(QCoreApplication::translate("COBRA2_CANWindow", "IN1", nullptr));
        lbl_IN2->setText(QCoreApplication::translate("COBRA2_CANWindow", "IN2", nullptr));
        lbl_ENUM1->setText(QCoreApplication::translate("COBRA2_CANWindow", "ENUM", nullptr));
        lbl_IN3->setText(QCoreApplication::translate("COBRA2_CANWindow", "IN3", nullptr));
        lbl_OUT2->setText(QCoreApplication::translate("COBRA2_CANWindow", "OUT2", nullptr));
        lbl_POTENTIOMETER1->setText(QCoreApplication::translate("COBRA2_CANWindow", "POTENTIOMETER", nullptr));
    } // retranslateUi

};

namespace Ui {
    class COBRA2_CANWindow: public Ui_COBRA2_CANWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COBRA2_CANWINDOW_H
