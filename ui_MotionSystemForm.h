/********************************************************************************
** Form generated from reading UI file 'MotionSystemForm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTIONSYSTEMFORM_H
#define UI_MOTIONSYSTEMFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MotionSystemForm
{
public:
    QGroupBox *Degrees_GB;
    QLabel *labelDegreesX;
    QLabel *labelDegreesY;
    QLabel *labelDegreesH;
    QGroupBox *Radians_GB;
    QLabel *labelRadiansH;
    QLabel *labelRadiansX;
    QLabel *labelRadiansY;

    void setupUi(QWidget *MotionSystemForm)
    {
        if (MotionSystemForm->objectName().isEmpty())
            MotionSystemForm->setObjectName(QString::fromUtf8("MotionSystemForm"));
        MotionSystemForm->resize(241, 221);
        Degrees_GB = new QGroupBox(MotionSystemForm);
        Degrees_GB->setObjectName(QString::fromUtf8("Degrees_GB"));
        Degrees_GB->setGeometry(QRect(10, 0, 221, 101));
        Degrees_GB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        Degrees_GB->setFlat(false);
        labelDegreesX = new QLabel(Degrees_GB);
        labelDegreesX->setObjectName(QString::fromUtf8("labelDegreesX"));
        labelDegreesX->setGeometry(QRect(30, 30, 91, 16));
        labelDegreesY = new QLabel(Degrees_GB);
        labelDegreesY->setObjectName(QString::fromUtf8("labelDegreesY"));
        labelDegreesY->setGeometry(QRect(30, 50, 91, 16));
        labelDegreesH = new QLabel(Degrees_GB);
        labelDegreesH->setObjectName(QString::fromUtf8("labelDegreesH"));
        labelDegreesH->setGeometry(QRect(30, 70, 91, 16));
        Radians_GB = new QGroupBox(MotionSystemForm);
        Radians_GB->setObjectName(QString::fromUtf8("Radians_GB"));
        Radians_GB->setGeometry(QRect(10, 100, 221, 101));
        labelRadiansH = new QLabel(Radians_GB);
        labelRadiansH->setObjectName(QString::fromUtf8("labelRadiansH"));
        labelRadiansH->setGeometry(QRect(30, 70, 91, 16));
        labelRadiansX = new QLabel(Radians_GB);
        labelRadiansX->setObjectName(QString::fromUtf8("labelRadiansX"));
        labelRadiansX->setGeometry(QRect(30, 30, 91, 16));
        labelRadiansY = new QLabel(Radians_GB);
        labelRadiansY->setObjectName(QString::fromUtf8("labelRadiansY"));
        labelRadiansY->setGeometry(QRect(30, 50, 91, 16));

        retranslateUi(MotionSystemForm);

        QMetaObject::connectSlotsByName(MotionSystemForm);
    } // setupUi

    void retranslateUi(QWidget *MotionSystemForm)
    {
        MotionSystemForm->setWindowTitle(QCoreApplication::translate("MotionSystemForm", "Form", nullptr));
        Degrees_GB->setTitle(QCoreApplication::translate("MotionSystemForm", "\320\223\321\200\320\260\320\264\321\203\321\201\321\213", nullptr));
        labelDegreesX->setText(QCoreApplication::translate("MotionSystemForm", "X:", nullptr));
        labelDegreesY->setText(QCoreApplication::translate("MotionSystemForm", "Y:", nullptr));
        labelDegreesH->setText(QCoreApplication::translate("MotionSystemForm", "H:", nullptr));
        Radians_GB->setTitle(QCoreApplication::translate("MotionSystemForm", "\320\240\320\260\320\264\320\270\320\260\320\275\321\213", nullptr));
        labelRadiansH->setText(QCoreApplication::translate("MotionSystemForm", "H:", nullptr));
        labelRadiansX->setText(QCoreApplication::translate("MotionSystemForm", "X:", nullptr));
        labelRadiansY->setText(QCoreApplication::translate("MotionSystemForm", "Y:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MotionSystemForm: public Ui_MotionSystemForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTIONSYSTEMFORM_H
