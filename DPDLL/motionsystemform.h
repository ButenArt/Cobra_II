#ifndef MOTIONSYSTEMFORM_H
#define MOTIONSYSTEMFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include "MotionSystem_DPDLL.h"

namespace Ui
{
    class MotionSystemForm;
}

class MotionSystemForm : public QWidget
{
    Q_OBJECT

public:
    explicit MotionSystemForm(QWidget *parent = 0, T_DPDLL *dpdll = 0);
    ~MotionSystemForm();
    QTimer *UpdateTimer;
private:
    Ui::MotionSystemForm *ui;
    T_DPDLL *_dpdll;
    void closeEvent(QCloseEvent *event);
private slots:
    void slotMSParamsUpdate();
signals:

};

#endif // MOTIONSYSTEMFORM_H
