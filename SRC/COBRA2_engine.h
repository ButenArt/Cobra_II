#ifndef COBRA2_ENGINE_H
#define COBRA2_ENGINE_H

#include <QObject>
#include <QTime>
#include <windows.h>
#include "Common/torquecurve.h"


// Двигатель  л.с.

class COBRA2_Engine : public QObject
{

  Q_OBJECT

  public:
    COBRA2_Engine(int T_init);
    ~COBRA2_Engine();
    void Motor_Reset(int T_init);
    void Motor_SetGas(float gas);
    void Motor_Move(float dT,float M_Fric,float Starter,float J_dop);
    float GetFuelConsumption();      //расход топлива (литров в секунду)
    struct TMotor;
    TMotor *Motor;
  private:
    TorqueCurve torqueCurve;
    QVector<WORD> Ge;                //удельный расход топлива по скоростной характеристике (г/кВ ч)
    WORD GetGe(float motorOmega);
    float GetMomentTable(float motorOmega, float gas);
    float RezultF(float Power, float motorOmega, float stoppedMotorCounterTorque, float FricV, float FricVV);
    float F_Temp();

};


#pragma pack(push, 1)
struct COBRA2_Engine::TMotor
{
    const WORD N_POINTS_Ge = 20;      //количество показателей графика удельного расхода топлива (Ge)
    const WORD STEP_Ge = 100;         //шаг оборотов двигателя для показателей графика удельного расхода топлива (700 .. 2600)
    const float R = 0.85f;             //плотность дизельного топлива (кг/дм3)
    const float K_POWER = 0.7355f;     //коэффициент соотношения 1 л.с./1 кВт
    const int K_SX_REG = -10;          //коэффициент наклона прямой
    const WORD OMEGA_XX_MAX = 2600;   //макс обороты кривой крутящего момента об/м
    const WORD OMEGA_MIN = 700;       //минимальные обороты кривой крутящего момента об/м
    const WORD OMEGA_MAX = 1700;      //обороты максимального крутящего момента
    const WORD OMEGA_BLOCK = 1600;    //обороты при которых блокируется переключение на пониженную передачу
    const WORD POWER_MAX = 260;       //максимальная мощность двигателя л.с.
    const float J_MOTOR = 0.5;        //момент инерции
    const float KT_D = 0.1f;           //коэффицеинт теплоотдачи двигателя
    const float ENGINE_MASS = 800000; //масса двигателя в г
    const WORD M_STOP = 100;          //момент сопротивления двигателя (для неработающего)
    const WORD K_STOP = 1;            //коэф сопротивления вращению (для неработающего)
    const float KT_DV_V = 300;        //коэффициент теплообмена двигатель - воздух
    const float KT_OG_DV = 30000;     //коэффициент теплообмена охлаждяющая жидкость - двигатель
    const float C_steel = 0.46f;       //удельная теплоемкость стали Дж/(г*К)
    const float C_water = 2.09f;       //удельная теплоемкость воды Дж/(г*К)
    const float COOLANT_MASS = 90000; //масса охлаждающей жидкости
    const float K_ENGINE = 0.6f;       //КПД двигателя
    float EngineFeedbackTorque;       //сила торможения двигателем
    float Omega;                      //угловая скорость
    float M_f;                        //Крутящий момент, необходимый для поддержания оборотов
    float Torque;                     //крутящий момент
    float Power;                      //мощность
    float Gas;                        //педаль газа от 0 до 1
    bool Started;                     //двигатель работает
    bool MotorStop;                   //момент остановки двигателя
    bool MotorStart;                  //момент запуска двигателя
    float T_Motor;                    //температура мотора (она же на приборе ОЖ)
    QTime StartTime;                  //время последнего запуска двигателя
};
#pragma pack(pop)


#endif // COBRA2_ENGINE_H
