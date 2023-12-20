#ifndef COBRA2_MOVE_H
#define COBRA2_MOVE_H

#include <windows.h>
#include <QObject>
#include "COBRA2_engine.h"
#include "COBRA2_transmission.h"


class COBRA2_Move : public QObject
{

  Q_OBJECT

  public:
    COBRA2_Move(COBRA2_Engine *engine, COBRA2_Transmission *transmission);
    ~COBRA2_Move();
    COBRA2_Engine *_engine;
    COBRA2_Transmission *_transmission;
    struct TMove;
    TMove *Move;
    void Init_position(int T_init);
    void Movement(float dT, float Starter);     //движение за время dT с моментом от сцепления M_Fric, с моментом от стартера Starter, коэф теплообмена ОЖ с воздухом KT_og_v
  private:
    qreal calculatePitchTorque();
};


#pragma pack(push, 1)
struct COBRA2_Move::TMove
{
    const BYTE WHEELS_NUM = 4;              //количество колес
    const float PI = 4.*atan(1.);           //число Пи
    const float WheelR = 0.5588f;           //радиус колеса в м.
    const float BODY_MASS = 7600;           //масса в кг
    const float MAB_VIBRATIONS = 0.05f;     //значение погрешности MAB энкодеров при диапазоне 0..1
    const float CORRECTION_INTERVAL = 1.6f; //коэффициент погрешности для вычисления точного интервала времени
    bool Moving;                            //движение с включенным двигателем
    bool Start_Move;                        //момент начала движения
    bool Stop_Move;                         //момент остановки
    float Kilometrage;                      //пробег
    float bodyVelocity;                     //скорость
    float RealCoupling;                     //сцепление
    float RealBrake;                        //тормоз
    float torqueMultiplier;                 //коэффициент передачи крутящего момента от двигателя на колесо
    float Pitch;                            //угол наклона
    float WheelsTorque[4];                  //крутящий момент на колесах
    float WheelsSpeed[4];                   //скорость на колесах в Км.ч.
    float brakeTorque[4];                   //компенсирующий момент тормоза
    float compensationTorque[4];            //компенсирующий крутящий момент колеса
    float engineTorque;                     //противомомент двигателя
};
#pragma pack(pop)


#endif // COBRA2_MOVE_H
