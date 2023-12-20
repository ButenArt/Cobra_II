#ifndef COBRA2_MAINWINDOW_H
#define COBRA2_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>
#include "Common/SystemTray.h"
#include "COBRA2_manager.h"
#include "DPDLL/motionsystemform.h"
#include "COBRA2_errorswindow.h"
#include "COBRA2_canwindow.h"

namespace Ui
{
    class COBRA2_MainWindow;
}

class COBRA2_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit COBRA2_MainWindow(QWidget *parent = 0);
    ~COBRA2_MainWindow();
    void Init();
private:
    void closeEvent(QCloseEvent *event);
    void CheckSlider(int value, bool &variable1, bool &variable2);
    void CheckWiperSlider(int value, bool &variable1, bool &variable2, bool &variable3);
    void CheckCentralLightAdjustLevelSlider(int value, bool &variable0, bool &variable1, bool &variable2, bool &variable3, bool &variable4);
    Ui::COBRA2_MainWindow *ui;
    SystemTray *systemTray;
    QTimer *NetStatTimer;
    QTimer *ControlTimer;
    T_COBRA2_manager *COBRA2;
    MotionSystemForm *msform;
    COBRA2_CANWindow *canform;
    COBRA2_ErrorsWindow *errorswindow;
private slots:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void on_ExStartButton_clicked();
    void on_ExStopButton_clicked();
    void on_GoButton_clicked();
    void on_IgnitionSlider_sliderReleased();
    void on_IgnitionSlider_valueChanged(int value);
    void on_BatteryButton_pressed();
    void on_BatteryButton_released();
    void on_SignalButton_pressed();
    void on_SignalButton_released();
    void on_WheelsPressureModeButton_pressed();
    void on_WheelsPressureModeButton_released();
    void on_TestButton_pressed();
    void on_TestButton_released();
    void on_RearFogLampButton_pressed();
    void on_RearFogLampButton_released();
    void slotUpdateNetStat();
    void slotCANControl();
    void slotPlaceToTray();
    void slotShowMainWindow();
    void slotApplicationQuit();

signals:
    void signalPlaceToTray();
};

#endif // COBRA2_MAINWINDOW_H
