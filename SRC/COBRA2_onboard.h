#ifndef COBRA2_ONBOARD_H
#define COBRA2_ONBOARD_H

#include <windows.h>
#include <QObject>
#include <QTimer>
#include "COBRA2.h"
#include "COBRA2_move.h"
#include "COBRA2_audio.h"
#include "COBRA2_ServiceErrors.h"


class T_COBRA2_Onboard : public QObject
{

  Q_OBJECT

  public:
    T_COBRA2_Onboard(T_COBRA2_driver *driver, COBRA2_Move *move, T_COBRA2_ServiceErrors *errors);
    ~T_COBRA2_Onboard();
    void Onboard_Compute(float dT);
    T_COBRA2_audio *_audio;
  private:
    T_COBRA2_driver *_driver;
    COBRA2_Move *_move;
    T_COBRA2_ServiceErrors *_errors;
    QTimer *FlashTimer;
    QTimer *FuelLineTimer;
    bool Flash;                         // мигание
    QThread thread;
    //постоянные характеристики//
    const BYTE V_Akkum_max = 24;        // максимальное напряжение АКБ
    const BYTE V_Board_max = 28;        // максимальное напряжение бортовой сети
    const BYTE SignalDelay = 5;         // время ожидания в секундах после звукового сигнала
  private slots:
    void slotSetFlash();
    void slotFuelLine();
};

#endif // COBRA2_ONBOARD_H
