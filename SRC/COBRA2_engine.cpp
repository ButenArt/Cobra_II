#include "COBRA2_engine.h"

COBRA2_Engine::COBRA2_Engine(int T_init)
{
    Ge << 216 << 215 << 210 << 206 << 200 << 198 << 197 << 197 << 197 << 198 << 199 << 200 << 203 << 205 << 208 << 210 << 213 << 217 << 220 << 224;
    Motor = new TMotor();
    Motor->EngineFeedbackTorque = 0;
    Motor->Omega = 0;
    Motor->M_f = 0;
    Motor->Torque = 0;
    Motor->Power = 0;
    Motor->Gas = 0;
    Motor->Started = false;
    Motor->MotorStop = false;
    Motor->MotorStart = false;
    Motor->T_Motor = T_init;
    Motor->StartTime = QTime::currentTime();
    torqueCurve.init(":/resource/COBRA2_torque.csv");
}

COBRA2_Engine::~COBRA2_Engine()
{
    delete Motor;
}

void COBRA2_Engine::Motor_Reset(int T_init)
{
    Motor->EngineFeedbackTorque = 0;
    Motor->Omega = 0;
    Motor->M_f = 0;
    Motor->Torque = 0;
    Motor->Power = 0;
    Motor->Gas = 0;
    Motor->Started = false;
    Motor->MotorStop = false;
    Motor->MotorStart = false;
    Motor->T_Motor = T_init;
    Motor->StartTime = QTime::currentTime();
    torqueCurve.init(":/resource/COBRA2_torque.csv");
}

void COBRA2_Engine::Motor_SetGas(float gas)
{
    if(gas<0) gas = 0;
    if(gas>1) gas = 1;
    Motor->Gas = gas;
}

WORD COBRA2_Engine::GetGe(float motorOmega)
{
    if(motorOmega < Motor->OMEGA_MIN) return 0;
    if(motorOmega > Motor->OMEGA_XX_MAX) return Ge[Motor->N_POINTS_Ge-1];
    int i = (int)(motorOmega - Motor->OMEGA_MIN)/Motor->STEP_Ge;
    if(i < 0) return 0;
    if(i >= Motor->N_POINTS_Ge) i = Motor->N_POINTS_Ge-1;
    return Ge[i];
}

float COBRA2_Engine::GetFuelConsumption()
{
    float Q = Motor->Power * GetGe(Motor->Omega) * Motor->K_POWER / 1000 * Motor->R / 3600;
    return Q;
}

float COBRA2_Engine::GetMomentTable(float motorOmega, float gas)
{
    if(gas < 0) gas = 0;
    if(gas > 1) gas = 1;

    if(motorOmega <= 0)
        return 0;

    //обороты от макс. при текущем газе
    int desiredOmega = gas * Motor->OMEGA_XX_MAX;

    Motor->Torque = torqueCurve.getTorque(round(motorOmega));

    //Пропорциональный регулятор оборотов
    auto motorOmegaPID = Motor->K_SX_REG * (motorOmega - desiredOmega);

    //qDebug() << motorOmega << "  " << desiredOmega << "  " << motorOmegaPID << "  " << Motor->Torque << "  " << (motorOmegaPID > Motor->Torque);

    if(motorOmegaPID > Motor->Torque)
        return Motor->Torque;
    else
        return motorOmegaPID;
}

//результирующая от действия силы стартера и трения
float COBRA2_Engine::RezultF(float inputTorque,float motorOmega,float stoppedMotorCounterTorque,float resistanceCoef,float FricVV = 0)
{
    float absInputTorque,absCounterTorque,resistanceTorque,res;

    absCounterTorque = std::abs(stoppedMotorCounterTorque);
    resistanceTorque = resistanceCoef * motorOmega + sign(motorOmega) * (FricVV*motorOmega*motorOmega+stoppedMotorCounterTorque);

    absInputTorque = std::abs(inputTorque);

    if(motorOmega == 0)
    {
        if(absInputTorque <= absCounterTorque)
            res = 0;
        else
            res = sign(inputTorque) * (absInputTorque - absCounterTorque);
    }
    else
        res = inputTorque - resistanceTorque;

    //qDebug() << inputTorque << "  " << motorOmega << "  " << stoppedMotorCounterTorque << "  " << resistanceCoef << "  " << FricVV << "  " << res;

    return res;
}

//Сила сопротивления неработающего двигателя от начальной температуры двигателя
float COBRA2_Engine::F_Temp()
{
    return 50 * std::exp(1.5 * (100 - Motor->T_Motor) / 60);
}

//движение за время dT с моментом от сцепления M_Fric, с моментом от стартера Starter, дополнительным моментом инерции J_dop
void COBRA2_Engine::Motor_Move(float dT, float M_Fric, float Starter, float J_dop)
{
    //даем 2 секунды на событие запуска двигателя
    if(Motor->StartTime.addSecs(2)<QTime::currentTime()) Motor->MotorStart = false;

    float torqueToMove = 0;

    //вычисление крутящего момента
    if (Motor->Started)
        torqueToMove = M_Fric + Motor->M_f;
    else
        torqueToMove = RezultF(M_Fric + Starter, Motor->Omega, Motor->M_STOP + F_Temp(), Motor->K_STOP, 0);

    Motor->Omega = Motor->Omega + torqueToMove / (Motor->J_MOTOR + J_dop) * dT;

    //qDebug() << M_Fric << "  " << Starter << "  " << Motor->Omega << "  " << Motor->M_STOP << "  " << F_Temp() << "  " << Motor->K_STOP << "  " << torqueToMove;

    if (Motor->Started)
        Motor->M_f = GetMomentTable(Motor->Omega, Motor->Gas);
    else
    {
        Motor->M_f = 0;
        Motor->Torque = 0;
    }

    if (Motor->Omega < 0)
        Motor->Omega = 0;

    if (Motor->Omega > Motor->OMEGA_XX_MAX)
        Motor->Omega = Motor->OMEGA_XX_MAX;

    if(Motor->Started)
    {
        Motor->Power = (Motor->Omega * Motor->Torque) / 9549 * 1.36;
        if (Motor->Power > Motor->POWER_MAX) Motor->Power = Motor->POWER_MAX;
        if (Motor->Omega < Motor->OMEGA_MIN)
        {
            Motor->Started = false;
            Motor->MotorStop = true;
            Motor->MotorStart = false;
        }
    }
    else
    {
        Motor->Power = 0;
        if ((Motor->Omega > Motor->OMEGA_MIN) && (Motor->Gas > Motor->OMEGA_MIN / Motor->OMEGA_XX_MAX))
        {
            Motor->StartTime = QTime::currentTime();
            Motor->Started = true;
            Motor->MotorStart = true;
            Motor->MotorStop = false;
        }
    }
}
