/********************************************************************************
** Form generated from reading UI file 'COBRA2_errorswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COBRA2_ERRORSWINDOW_H
#define UI_COBRA2_ERRORSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_COBRA2_ErrorsWindow
{
public:
    QGroupBox *ErrorsGB;
    QListWidget *ErrorsListWidget;

    void setupUi(QWidget *COBRA2_ErrorsWindow)
    {
        if (COBRA2_ErrorsWindow->objectName().isEmpty())
            COBRA2_ErrorsWindow->setObjectName(QString::fromUtf8("COBRA2_ErrorsWindow"));
        COBRA2_ErrorsWindow->resize(381, 331);
        ErrorsGB = new QGroupBox(COBRA2_ErrorsWindow);
        ErrorsGB->setObjectName(QString::fromUtf8("ErrorsGB"));
        ErrorsGB->setGeometry(QRect(10, 10, 361, 311));
        ErrorsListWidget = new QListWidget(ErrorsGB);
        ErrorsListWidget->setObjectName(QString::fromUtf8("ErrorsListWidget"));
        ErrorsListWidget->setGeometry(QRect(10, 30, 341, 271));

        retranslateUi(COBRA2_ErrorsWindow);

        QMetaObject::connectSlotsByName(COBRA2_ErrorsWindow);
    } // setupUi

    void retranslateUi(QWidget *COBRA2_ErrorsWindow)
    {
        COBRA2_ErrorsWindow->setWindowTitle(QCoreApplication::translate("COBRA2_ErrorsWindow", "Form", nullptr));
        ErrorsGB->setTitle(QCoreApplication::translate("COBRA2_ErrorsWindow", "Errors", nullptr));
    } // retranslateUi

};

namespace Ui {
    class COBRA2_ErrorsWindow: public Ui_COBRA2_ErrorsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COBRA2_ERRORSWINDOW_H
