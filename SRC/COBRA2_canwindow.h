#ifndef COBRA2_CANWINDOW_H
#define COBRA2_CANWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include "COBRA2_controls.h"

namespace Ui
{
    class COBRA2_CANWindow;
}

class COBRA2_CANWindow : public QWidget
{
    Q_OBJECT

public:
    explicit COBRA2_CANWindow(QWidget *parent = 0, T_COBRA2_controls *controls = 0);
    ~COBRA2_CANWindow();
    QTimer *UpdateTimer;
private:
    Ui::COBRA2_CANWindow *ui;
    T_COBRA2_controls *_controls;
    QString Byte2Bin(BYTE val);
    void closeEvent(QCloseEvent *event);
private slots:
    void slotParamsUpdate();
signals:

};

#endif // COBRA2_CANWINDOW_H
