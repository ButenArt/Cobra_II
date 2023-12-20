#ifndef COBRA2_ERRORSWINDOW_H
#define COBRA2_ERRORSWINDOW_H

#include "QWidget"
#include <QTimer>
#include <QEvent.h>
#include "COBRA2_manager.h"

namespace Ui
{
    class COBRA2_ErrorsWindow;
}

class COBRA2_ErrorsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit COBRA2_ErrorsWindow(QWidget *parent = 0);
    ~COBRA2_ErrorsWindow();
    T_COBRA2_manager *_COBRA2;
    QTimer *UpdateTimer;
private:
    Ui::COBRA2_ErrorsWindow *ui;
    void closeEvent(QCloseEvent *event);
private slots:
    void slotErrorsUpdate();
signals:

};

#endif // COBRA2_ERRORSWINDOW_H
