#include "COBRA2_ServiceErrors.h"

T_COBRA2_ServiceErrors::T_COBRA2_ServiceErrors(T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, COBRA2_Move *move)
{
    _instructor = Instructor;
    _driver = driver;
    _move = move;

    Errors.Name[0] = "Empty";
    Errors.Name[1] = "Низкое давление масла в двигателе.";
    Errors.Name[2] = "МЗН";
    Errors.Name[3] = "Долго вкл. стартер.";
    Errors.Name[4] = "Выкл. двиг. при T ОЖ более 90 градусов.";
    Errors.Name[5] = "Высокие обороты на пониженной передаче.";
    Errors.Name[6] = "Низкие обороты на повышенной передаче.";
    Errors.Name[7] = "Остановка двигателя.";
    Errors.Name[8] = "Трогание без подачи звукового сигнала.";
    Errors.Name[9] = "Столкновение.";
    Errors.Name[10] = "Низкое давление масла в РК.";
    Errors.Name[11] = "Напряжение АКБ ниже нормы.";
    Errors.Name[12] = "Перегрев двигателя Т ОЖ больше нормы.";
    Errors.Name[13] = "Перегрев двигателя Т дв. больше нормы.";
    Errors.Name[14] = "Пуск двигателя без подачи звукового сигнала.";
    Errors.Name[15] = "Пуск двигателя при незаторможенной машине.";
    Errors.Name[16] = "Начало движения при Т дв. <30°С.";
    Errors.Name[17] = "Движение при открытом люке.";
    Errors.Name[18] = "Пуск двигателя при низкой T ОЖ.";

    Errors.DeltaT[0] = 0;
    Errors.DeltaT[1] = 30;
    Errors.DeltaT[2] = 0;
    Errors.DeltaT[3] = 0;
    Errors.DeltaT[4] = 0;
    Errors.DeltaT[5] = 5;
    Errors.DeltaT[6] = 5;
    Errors.DeltaT[7] = 0;
    Errors.DeltaT[8] = 0;
    Errors.DeltaT[9] = 0;
    Errors.DeltaT[10] = 30;
    Errors.DeltaT[11] = 60;
    Errors.DeltaT[12] = 60;
    Errors.DeltaT[13] = 60;
    Errors.DeltaT[14] = 0;
    Errors.DeltaT[15] = 0;
    Errors.DeltaT[16] = 0;
    Errors.DeltaT[17] = 10;
    Errors.DeltaT[18] = 0;

    FaultInjection.Name[0] = "Unknown";
    FaultInjection.Name[1] = "Отказ тормозной системы";
    FaultInjection.Name[2] = "Низкое давление в шинах";
    FaultInjection.Name[4] = "Упало  давления масла в двигателе";
    FaultInjection.Name[5] = "Поломка насоса гидроусилителя рулевого управления";
    FaultInjection.Name[6] = "Отказ привода сцепления";
    FaultInjection.Name[10] = "Перегрев охлаждающей жидкости в двигателе";
    FaultInjection.Name[11] = "Отказ зарядки аккумуляторных батарей";
    FaultInjection.Name[12] = "Окончание топлива в одном из топливных баков";
    FaultInjection.Name[13] = "Подсос воздуха в систему питания топлива";

    connect(ErrorsTimer,SIGNAL(timeout()),this, SLOT(slotCheckErrors()));
    ErrorsTimer->start();
}

void T_COBRA2_ServiceErrors::slotCheckErrors()
{
    for(int i=0; i<N_ERRORS; i++)
    {
        Errors.Active[i] = false;
        switch(i)
        {
            case 1: // Низкое давление масла в двигателе
                if(_driver->OnBoard->EngineStarted && _driver->OnBoard->Pmdv<1) Errors.Active[i] = true;
            break;
            case 2:

            break;
            case 3: // Долго вкл. стартер
                if(_driver->OnBoard->Time_Start>4) Errors.Active[i] = true;
            break;
            case 4: // Выкл. двиг. при темп. ОЖ более 90 градусов
                if(_move->_engine->Motor->MotorStop && _driver->OnBoard->T_OG>90) Errors.Active[i] = true;
            break;
            case 5: // Высокие обороты на пониженной передаче
                if(_driver->OnBoard->EngineStarted && _driver->OnBoard->Gear<4 && _move->_engine->Motor->Omega>_move->_engine->Motor->OMEGA_XX_MAX) Errors.Active[i] = true;
            break;
            case 6: // Низкие обороты на повышенной передаче
                if(_driver->OnBoard->EngineStarted && _driver->OnBoard->Gear>3 && _move->_engine->Motor->Omega<1000) Errors.Active[i] = true;
            break;
            case 7: // Остановка двигателя
                if(_move->_engine->Motor->MotorStop) Errors.Active[i] = true;
            break;
            case 8: // Трогание без подачи звукового сигнала
                if(_move->Move->Start_Move && !_driver->OnBoard->SignalOnMove) Errors.Active[i] = true;
            break;
            case 9: // Столкновение
                if(_driver->OnBoard->Collision) Errors.Active[i] = true;
            break;
            case 10: // Низкое давление масла в РК
                if(_driver->OnBoard->EngineStarted && _driver->OnBoard->Pmkp<1) Errors.Active[i] = true;
            break;
            case 11: // Напряжение АКБ ниже нормы
                if(_driver->OnBoard->V_akb<18) Errors.Active[i] = true;
            break;
            case 12: // Перегрев температуры ОЖ двигателя больше нормы
                if(_driver->OnBoard->T_OG>95) Errors.Active[i] = true;
            break;
            case 13: // Перегрев температуры двигателя больше нормы
                if(_driver->OnBoard->T_Motor>105) Errors.Active[i] = true;
            break;
            case 14: // Пуск двигателя без подачи звукового сигнала
                if(_move->_engine->Motor->MotorStart && !_driver->OnBoard->SignalOnEngineStarted) Errors.Active[i] = true;
            break;
            case 15: // Пуск двигателя при незаторможенной машине
                if(_move->_engine->Motor->MotorStart && _driver->OnBoard->F_Stop==0) Errors.Active[i] = true;
            break;
            case 16: // Начало движения при температуре двигателя <30°С
                if(_move->Move->Start_Move && _driver->OnBoard->T_Motor<30) Errors.Active[i] = true;
            break;
            case 17:

            break;
            case 18: // Пуск двигателя при низкой температуре ОЖ
                if(_move->_engine->Motor->MotorStart)
                {
                    if(_driver->OnBoard->T_init<-5 && _driver->OnBoard->T_OG<80) Errors.Active[i] = true;
                    if(_driver->OnBoard->T_init<-20 && _driver->OnBoard->T_OG<110) Errors.Active[i] = true;
                }
            break;
        }
        if(Errors.Active[i] && Errors.DeltaT[i]>0) Errors.Time[i] = QTime::currentTime();
        if(Errors.Active[i])
        {
            if(QTime::currentTime() > Errors.Time[i].addSecs(Errors.DeltaT[i])) Errors.Send[i] = true;
        }
    }
    if(!_instructor->Failures->PacketReady) _instructor->FailuresDequeuePacket();
    if(_instructor->Failures->PacketReady)
    {
        if(_instructor->Failures->failureType < N_FAULTS)
        {
            FaultInjection.Active[_instructor->Failures->failureType] = _instructor->Failures->failureActive;
            _instructor->Failures->PacketReady = false;
        }
    }
}
