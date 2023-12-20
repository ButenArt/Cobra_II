#include "COBRA2_onboard.h"

T_COBRA2_Onboard::T_COBRA2_Onboard(T_COBRA2_driver *driver, COBRA2_Move *move, T_COBRA2_ServiceErrors *errors)
{
    _driver = driver;
    _move = move;
    _errors = errors;

    Flash = false;

    FlashTimer = new QTimer(this);
    FlashTimer->setInterval(500);
    connect(FlashTimer,SIGNAL(timeout()),this, SLOT(slotSetFlash()));
    FlashTimer->start();

    FuelLineTimer = new QTimer(this);
    FuelLineTimer->setInterval(1000);
    connect(FuelLineTimer,SIGNAL(timeout()),this, SLOT(slotFuelLine()));
    FuelLineTimer->start();

    _audio = new T_COBRA2_audio();
    _audio->moveToThread(&thread);
    connect(&thread, &QThread::finished, _audio, &QObject::deleteLater);
    thread.start();
}

T_COBRA2_Onboard::~T_COBRA2_Onboard()
{
    delete FlashTimer;
    delete FuelLineTimer;
    delete _audio;
    thread.quit();
    thread.wait();
}

void T_COBRA2_Onboard::slotSetFlash()
{
    Flash = !Flash;
}

void T_COBRA2_Onboard::slotFuelLine()
{
    if(_driver->OnBoard->EngineStarted)
    {
        // уровень топлива
        if(_driver->OnBoard->LeftFuelTankLevel > 0)
        {
            _driver->OnBoard->LeftFuelTankLevel = _driver->OnBoard->LeftFuelTankLevel - _move->_engine->GetFuelConsumption();
            if(_driver->OnBoard->LeftFuelTankLevel < 0) _driver->OnBoard->LeftFuelTankLevel = 0;
        }
        else
        {
            _driver->OnBoard->RightFuelTankLevel = _driver->OnBoard->RightFuelTankLevel - _move->_engine->GetFuelConsumption();
            if(_driver->OnBoard->RightFuelTankLevel < 0) _driver->OnBoard->RightFuelTankLevel = 0;
        }
    }
}

void T_COBRA2_Onboard::Onboard_Compute(float dT)
{
    float p1 = 0;
    float p3 = 0;
    float tempx = 0;
    float Kp = 0;

    // время с момента начала упражнения
    _driver->OnBoard->Time = _driver->OnBoard->Time + dT;

    // лампы внутренней сигнализации
    _driver->Lamps->EngineOilPressLamp = false;            // сд. давление масла в двигателе
    _driver->Lamps->LorryLamp = false;                     // сд. фонари автопоезда
    _driver->Lamps->WaterLevelLamp = false;                // сд. уровень ОЖ
    _driver->Lamps->SMULamp = false;                       // сд. режим светомаскировки
    _driver->Lamps->ColdStartLamp = false;                 // сд. холодный пуск
    _driver->Lamps->TurnLamp = false;                      // сд. указатель поворота
    _driver->Lamps->SidelightsLamp = false;                // сд. габаритные огни
    _driver->Lamps->DiagnosticsLamp = false;               // сд. диагностика
    _driver->Lamps->TrailerTurnLamp = false;               // сд. указатель поворота прицепа
    _driver->Lamps->LowFuelLamp = false;                   // сд. мало топлива
    _driver->Lamps->WaterInFuelLamp = false;               // сд. наличие воды в топливе
    _driver->Lamps->AirFilterLamp = false;                 // сд. засореный воздушный фильтр
    _driver->Lamps->BatteryLamp = false;                   // сд. батарея
    _driver->Lamps->PreheaterLamp = false;                 // сд. предпусковой подогреватель (ПЖД16)
    _driver->Lamps->EngineBrakeLamp = false;               // сд. моторный тормоз
    _driver->Lamps->InteraxleDiffLockLamp = false;         // сд. сигнал блокировки межосевого дифференциала
    _driver->Lamps->CrossaxleDiffLockLamp = false;         // сд. сигнал блокировки межколесного дифференциала
    _driver->Lamps->HighBeamLamp = false;                  // сд. дальний свет
    _driver->Lamps->HandBrakeLamp = false;                 // сд. включение стояночной тормозной системы
    _driver->Lamps->BrakeFaultLamp = false;                // сд. неисправность тормозной системы
    _driver->Lamps->WaterOverheatLamp = false;             // сд. перегрев ОЖ
    _driver->Lamps->WheelsAirPressure_Sand = false;        // сд. режим "песок" давления воздуха в шинах
    _driver->Lamps->WheelsAirPressure_Terrain = false;     // сд. режим "грунт" давления воздуха в шинах
    _driver->Lamps->WheelsAirPressure_Highway = false;     // сд. режим "шоссе" давления воздуха в шинах
    _driver->Lamps->WheelsAirPressure_Fault = false;       // сд. неисправность или низкое давление в шинах
    _driver->Doping_2->LeftGreenLamp = false;              // сд. исправность левого генератора системы пожаротушения
    _driver->Doping_2->RightGreenLamp = false;             // сд. исправность правого генератора системы пожаротушения
    _driver->Doping_2->LeftRedLamp = false;                // сд. неисправность левого генератора системы пожаротушения
    _driver->Doping_2->RightRedLamp = false;               // сд. неисправность правого генератора системы пожаротушения
    _driver->Lamps->DashBoardLight = false;                // подсветка приборной доски

    // электромагниты
    _driver->OnBoard->SteeringBlock = true;                // блокировка руля
    _driver->OnBoard->TransmissionBlock = false;           // блокировка КПП

    // фары и габариты
    _driver->Sidelights->Sidelights = false;               // фонари габаритов
    _driver->Sidelights->TurnLeftLights = false;           // фонари сигнала левого поворота
    _driver->Sidelights->TurnRightLights = false;          // фонари сигнала правого поворота
    _driver->Sidelights->StopLights = false;               // фонари стоп сигнала
    _driver->Sidelights->ReverseLight = false;             // фонарь заднего хода
    _driver->Sidelights->CentralLights = false;            // фары (ближний свет)
    _driver->Sidelights->DistanceLight = false;            // фары (дальний свет)
    _driver->Sidelights->RearFogLight = false;             // заднеие противотуманные фонари
    _driver->Sidelights->LorryLight = false;               // фонарь "Автопоезд"

    // звуки
    _audio->Play(0, _driver->OnBoard->Signal);
    _audio->Play(1, _driver->OnBoard->Starter == 1);
    _audio->Play(2, _move->_engine->Motor->MotorStart);
    _audio->Play(3, _driver->OnBoard->EngineStarted);
    _audio->Play(8, _driver->OnBoard->EngineStarted);

    if(!_driver->OnBoard->EngineStarted)
    {
        // события нажатия кнопки сигнала
        _driver->OnBoard->SignalOnEngineStarted = false;
        _driver->OnBoard->SignalOnMove = false;
    }

    // блокировка руля
    if(_driver->OnBoard->EngineStarted) _driver->OnBoard->SteeringBlock = false; else _driver->OnBoard->SteeringBlock = true;
    if(_errors->FaultInjection.Active[NO_STEER_HYDRAULIC]) _driver->OnBoard->SteeringBlock = true;

    // блокировка КПП
    if(_driver->OnBoard->EngineStarted)
    {
        if(_move->_transmission->Transmission->Coupling > 0.2f) _driver->OnBoard->TransmissionBlock = true; else _driver->OnBoard->TransmissionBlock = false;
    }
    else _driver->OnBoard->TransmissionBlock = false;

    //стартер не работает
    _driver->OnBoard->Starter = 0;

    // генератор
    if(_driver->OnBoard->EngineStarted) _driver->OnBoard->Generator = true; else _driver->OnBoard->Generator = false;

    // МОД
//    if(_driver->Switches->Ignition_I || _driver->Switches->Ignition_II)
//    {
//        _driver->OnBoard->StopEngine = false;
//        _driver->OnBoard->Time_Ignition_I = _driver->OnBoard->Time_Ignition_I + dT;
//    }
//    else
//    {
//        _driver->OnBoard->StopEngine = true;
//        _driver->OnBoard->Time_Ignition_I = 0;
//    }

    //нажата кнопка запуска двигателя
    if(_driver->Switches->Ignition_II && _driver->OnBoard->BatteryActive && !_driver->OnBoard->EngineStarted) _driver->OnBoard->Starter = 1;

    //вычисление заряда батарей
    if(_driver->Switches->Battery) _driver->OnBoard->Time_Battery = _driver->OnBoard->Time_Battery + dT; else _driver->OnBoard->Time_Battery = 0;
    if(_driver->OnBoard->Time_Battery>0 && _driver->OnBoard->Time_Battery<=dT) _driver->OnBoard->BatteryActive = !_driver->OnBoard->BatteryActive;
    if(_driver->OnBoard->BatteryActive)
    {
        if(!_driver->OnBoard->Generator)
        {
            _driver->OnBoard->I_board = 0;
            _driver->OnBoard->V_board = _driver->OnBoard->V_akb;
        }
        if(_driver->OnBoard->Generator && !_errors->FaultInjection.Active[NO_BATTERY_CHARGING])
        {
            _driver->OnBoard->I_board = 0;
            _driver->OnBoard->V_akb = _driver->OnBoard->V_akb + static_cast<quint8>(_driver->OnBoard->Generator)*dT/200.0f;  // АКБ зaряжается
            if(_driver->OnBoard->V_akb > V_Akkum_max) _driver->OnBoard->V_akb = V_Akkum_max;
            _driver->OnBoard->V_board = V_Board_max;
        }
        if(_driver->OnBoard->Starter == 1 || _driver->OnBoard->Starter == 3 || _errors->FaultInjection.Active[NO_BATTERY_CHARGING]/* || _driver->Switches->EFD*/)
        {
           _driver->OnBoard->V_akb = _driver->OnBoard->V_akb - dT / 10.0f;          // АКБ разряжается
           if(_driver->OnBoard->V_akb < 0) _driver->OnBoard->V_akb = 0;
           _driver->OnBoard->I_board = -5;
           _driver->OnBoard->V_board = _driver->OnBoard->V_akb;
        }
    }
    else
    {
       _driver->OnBoard->I_board = 0;
       _driver->OnBoard->V_board = 0;
    }

    // подогреватель ПЖД16
    if(_driver->OnBoard->BatteryActive)
    {
        if(_driver->Switches->Preheater) _driver->OnBoard->Preheater = true; else _driver->OnBoard->Preheater = false;
    }
    else _driver->OnBoard->Preheater = false;

    // светомаскировка
    if(_driver->OnBoard->BatteryActive)
    {
        if(_driver->Switches->SMU) _driver->OnBoard->SMU = true; else _driver->OnBoard->SMU = false;
    }
    else _driver->OnBoard->SMU = false;

    // моторный тормоз
    if(_driver->OnBoard->BatteryActive)
    {
        if(_driver->Controls->EngineBrake) _driver->OnBoard->EngineBrake = true; else _driver->OnBoard->EngineBrake = false;
    }
    else _driver->OnBoard->EngineBrake = false;

    // включение системы регулирования давления воздуха в шинах
    if(_driver->OnBoard->BatteryActive && (_driver->Switches->Ignition_I || _driver->Switches->Ignition_II))
    {
        if(_driver->Switches->WheelsAirPressurePower) _driver->OnBoard->WheelsAirPressurePower = true; else _driver->OnBoard->WheelsAirPressurePower = false;
    }
    else _driver->OnBoard->WheelsAirPressurePower = false;

    // задние противотуманные фонари
    if(_driver->OnBoard->BatteryActive)
    {
        if(_driver->Switches->RearFogLight) _driver->OnBoard->Time_RearFogLight = _driver->OnBoard->Time_RearFogLight + dT; else _driver->OnBoard->Time_RearFogLight = 0;
        if(_driver->OnBoard->Time_RearFogLight>0 && _driver->OnBoard->Time_RearFogLight<=dT) _driver->OnBoard->RearFogLight = !_driver->OnBoard->RearFogLight;
    }
    else _driver->OnBoard->RearFogLight = false;

    // показание амперметра в зависимости от тангажа
    if(_driver->OnBoard->EngineStarted && _move->Move->bodyVelocity > 0.5f)
    {
        if(_move->Move->Pitch > 5.0f) _driver->OnBoard->I_board = -5.0f; else _driver->OnBoard->I_board = _driver->OnBoard->I_board;
    }

    //вычисление давления воздушного баллона и шин
    if((_driver->OnBoard->Pvoz > 800.0f) || !_driver->OnBoard->EngineStarted) _driver->OnBoard->Compressor = false;  // выключаем компрессор
    if((_driver->OnBoard->Pvoz < 650.0f) && _driver->OnBoard->EngineStarted) _driver->OnBoard->Compressor = true;    // включаем компрессор
    if(_driver->OnBoard->Compressor) _driver->OnBoard->Pvoz = _driver->OnBoard->Pvoz + 10.0f * dT;
    if(_driver->OnBoard->Pvoz < 0) _driver->OnBoard->Pvoz = 0;

    // режим системы регулирования давления воздуха в шинах
    if(_driver->OnBoard->WheelsAirPressurePower)
    {
        if(_driver->Switches->WheelsAirPressureMode) _driver->OnBoard->Time_WheelsPressureMode = _driver->OnBoard->Time_WheelsPressureMode + dT; else _driver->OnBoard->Time_WheelsPressureMode = 0;
        if(_driver->OnBoard->Time_WheelsPressureMode>0 && _driver->OnBoard->Time_WheelsPressureMode<=dT) _driver->OnBoard->WheelsAirPressureMode++;
        if(_driver->OnBoard->WheelsAirPressureMode > 3) _driver->OnBoard->WheelsAirPressureMode = 1;
        if(_driver->OnBoard->Pvoz > 550.0f)
        {
            switch(_driver->OnBoard->WheelsAirPressureMode)
            {
                case 1:
                    // передняя ось
                    if(_driver->OnBoard->Press_FrontReducer > 9.1f) _driver->Press_FrontRedu(-0.01f);
                    if(_driver->OnBoard->Press_FrontReducer < 9.1f) _driver->Press_FrontRedu(+0.01f);
                    // задняя ось
                    if(_driver->OnBoard->Press_RearReducer > 9.1f) _driver->Press_RearRedu(-0.01f);
                    if(_driver->OnBoard->Press_RearReducer < 9.1f) _driver->Press_RearRedu(+0.01f);
                break;
                case 2:
                    // передняя ось
                    if(_driver->OnBoard->Press_FrontReducer > 30.1f) _driver->Press_FrontRedu(-0.01f);
                    if(_driver->OnBoard->Press_FrontReducer < 30.1f) _driver->Press_FrontRedu(+0.01f);
                    // задняя ось
                    if(_driver->OnBoard->Press_RearReducer > 35.1f) _driver->Press_RearRedu(-0.01f);
                    if(_driver->OnBoard->Press_RearReducer < 35.1f) _driver->Press_RearRedu(+0.01f);
                break;
                case 3:
                    // передняя ось
                    if(_driver->OnBoard->Press_FrontReducer > 40.1f) _driver->Press_FrontRedu(-0.01f);
                    if(_driver->OnBoard->Press_FrontReducer < 40.1f) _driver->Press_FrontRedu(+0.01f);
                    // задняя ось
                    if(_driver->OnBoard->Press_RearReducer > 49.6f) _driver->Press_RearRedu(-0.01f);
                    if(_driver->OnBoard->Press_RearReducer < 49.6f) _driver->Press_RearRedu(+0.01f);
                break;
            }
        }
        if(_move->Move->bodyVelocity > 15.0f && (_driver->OnBoard->Press_FrontReducer < 30.0f || _driver->OnBoard->Press_RearReducer < 35.0f)) _driver->Lamps->WheelsAirPressure_Fault = true;   // превышена скорость 15км/ч на пониженом давлении шин
        if(_move->Move->bodyVelocity > 30.0f && ((_driver->OnBoard->Press_FrontReducer >= 30.0f && _driver->OnBoard->Press_FrontReducer < 40.0f) || (_driver->OnBoard->Press_RearReducer >= 35.0f && _driver->OnBoard->Press_RearReducer < 49.5f))) _driver->Lamps->WheelsAirPressure_Fault = true;   // превышена скорость 30км/ч  на пониженом давлении шин
    }

    // ручная регулировка давления воздуха в шинах
    if(_driver->OnBoard->Pvoz > 550.0f)
    {
       // накачка шин передней оси
       if(_driver->TyreValves->FrontAirTapPump) _driver->Press_FrontRedu(+0.01f);
       // накачка шин задней оси
       if(_driver->TyreValves->RearAirTapPump) _driver->Press_RearRedu(+0.01f);
    }
    // выпуск воздуха передней оси
    if(_driver->TyreValves->FrontAirTapDischarge) _driver->Press_FrontRedu(-0.01f);
    // выпуск воздуха задней оси
    if(_driver->TyreValves->RearAirTapDischarge) _driver->Press_RearRedu(-0.01f);
    // давление в шинах
    _driver->Tyre_Redu(4);  // 4 шина
    _driver->Tyre_Redu(3);  // 3 шина
    _driver->Tyre_Redu(2);  // 2 шина
    _driver->Tyre_Redu(1);  // 1 шина
    // краны шин передней оси
    if(!_driver->TyreValves->TyreTap_1 && !_driver->TyreValves->TyreTap_2) _driver->OnBoard->FrontWheelsAirPressure = 0; else _driver->OnBoard->FrontWheelsAirPressure = _driver->OnBoard->Press_FrontReducer;
    // краны шин задней оси
    if(!_driver->TyreValves->TyreTap_3 && !_driver->TyreValves->TyreTap_4) _driver->OnBoard->RearWheelsAirPressure = 0; else _driver->OnBoard->RearWheelsAirPressure = _driver->OnBoard->Press_RearReducer;

    // манометр давления воздуха в пневмоприводе тормозов
    if(_driver->OnBoard->EngineStarted)
    {
        if(_driver->Controls->Brake < 5.0f) _driver->OnBoard->BrakeSystemAirPressure = 0;
        if(_driver->Controls->Brake >= 5.0f) _driver->OnBoard->BrakeSystemAirPressure = (_driver->OnBoard->Pvoz / 100.0f) * _driver->Controls->Brake;
    }
    else _driver->OnBoard->BrakeSystemAirPressure = 0;

    //вычисление силы стартера
    if(_driver->OnBoard->V_board < 12.0f) p1 = 0.4f*_driver->OnBoard->V_akb/V_Akkum_max;
                 else p1 = 3000.0f*_driver->OnBoard->V_akb/V_Akkum_max;
    p3 = _driver->OnBoard->Pvoz;

    // стартер 0-не работает, 1-от АКБ, 2-от воздуха, 3-от обеих
    switch(_driver->OnBoard->Starter)
    {
      case 1:
        _driver->OnBoard->Starter_f = p1;
      break;
      case 2:
        _driver->OnBoard->Starter_f = p3;
      break;
      case 3:
        _driver->OnBoard->Starter_f = qMax(p1,p3);
      break;
      default:
        _driver->OnBoard->Starter_f = 0;
      break;
    }

    // следим за временем непрерывного нажатия на кнопку стартера и ошибками связанными со стартером
    if(_driver->OnBoard->Starter > 0) _driver->OnBoard->Time_Start = _driver->OnBoard->Time_Start + dT; else _driver->OnBoard->Time_Start = 0;

    if(_driver->OnBoard->EngineStarted)
    {
      // вычисление температур
      float MAX_TEMP = 81.5f;
      tempx = (_move->_engine->Motor->Omega/_move->_engine->Motor->OMEGA_XX_MAX+_driver->OnBoard->Starter/3.0f);
      if(_errors->FaultInjection.Active[COOLANT_OVERHEAT])
      {
          MAX_TEMP = 110.5f;
          tempx = 899.0f;
      }
      if(_driver->OnBoard->EngineVentilation) _driver->OnBoard->K_T_og_v = (3.0*_driver->OnBoard->Shutters+3.0)*1000.0; else _driver->OnBoard->K_T_og_v = (3.0*_driver->OnBoard->Shutters+3.0)*15.0*(1.0+_move->_engine->Motor->Omega/100.0);
      // температура двигателя
      _driver->OnBoard->T_Motor = _driver->OnBoard->T_Motor+((1.0-_move->_engine->Motor->K_ENGINE)/_move->_engine->Motor->K_ENGINE*_move->_engine->Motor->Power*736.0+_move->_engine->Motor->KT_DV_V*(_driver->OnBoard->T_init-_driver->OnBoard->T_Motor)+tempx*_move->_engine->Motor->KT_OG_DV*(_driver->OnBoard->T_OG-_driver->OnBoard->T_Motor))*dT/_move->_engine->Motor->ENGINE_MASS/_move->_engine->Motor->C_steel;
      // температура ОЖ
      if(_driver->OnBoard->T_OG > MAX_TEMP) _driver->OnBoard->T_OG = _driver->OnBoard->T_OG+(tempx*_move->_engine->Motor->KT_OG_DV*(_driver->OnBoard->T_Motor-_driver->OnBoard->T_OG)+_driver->OnBoard->K_T_og_v*(1.0+_move->_engine->Motor->Omega/1000.0)*(_driver->OnBoard->T_init-_driver->OnBoard->T_OG))*dT/_move->_engine->Motor->COOLANT_MASS/_move->_engine->Motor->C_water;
      else _driver->OnBoard->T_OG = _driver->OnBoard->T_OG+(tempx*_move->_engine->Motor->KT_OG_DV*(_driver->OnBoard->T_Motor-_driver->OnBoard->T_OG)+_driver->OnBoard->K_T_og_v*(1.0+_move->_engine->Motor->Omega/1000.0)*(_driver->OnBoard->T_OG-_driver->OnBoard->T_init))*dT/_move->_engine->Motor->COOLANT_MASS/_move->_engine->Motor->C_water;
    }
    else
    {
      _driver->OnBoard->T_Motor = _driver->OnBoard->T_Motor - 0.001f;
      if(_driver->OnBoard->T_Motor <= _driver->OnBoard->T_init) _driver->OnBoard->T_Motor = _driver->OnBoard->T_init;
      _driver->OnBoard->T_OG = _driver->OnBoard->T_OG - 0.001f;
      if(_driver->OnBoard->T_OG <= _driver->OnBoard->T_init) _driver->OnBoard->T_OG = _driver->OnBoard->T_init;
    }

    // температура свечи
    _driver->OnBoard->T_Svecha = _driver->OnBoard->T_Svecha+(_driver->OnBoard->T_init-_driver->OnBoard->T_Svecha)*dT/(dT*1000);

    // Запуск ЭФУ, разогрев свечи
/*    if(_driver->Switches->EFD)
    {
      _driver->OnBoard->T_Svecha = _driver->OnBoard->T_Svecha+(500-_driver->OnBoard->T_Svecha)*_driver->OnBoard->V_akb/V_Akkum_max/500;  // нажат т. Свеча
      _driver->OnBoard->V_akb = _driver->OnBoard->V_akb-0.005;          // АКБ разряжается
    }*/

    // давления масла двигателя
    tempx = 400.0f;
    if(_errors->FaultInjection.Active[LOW_ENGINE_OIL])
        _driver->OnBoard->Pmdv = _driver->OnBoard->Pmdv + float(0.65f-_driver->OnBoard->Pmdv)*dT;  // отказ - низкое давление масла
    else
        _driver->OnBoard->Pmdv = _driver->OnBoard->Pmdv+(_move->_engine->Motor->Omega/qAbs(tempx-_driver->OnBoard->Pmdv*100.0f)-_driver->OnBoard->Pmdv)*dT/0.4f;
    if(!_driver->OnBoard->EngineStarted && _driver->OnBoard->Pmdv < 0.1f) _driver->OnBoard->Pmdv = 0;

    // давления масла РК
    if(_driver->OnBoard->Gear <= 0) Kp = 2.0f/23.0f; else Kp = (19.0f+qAbs(_driver->OnBoard->Gear))/23.0f;
    _driver->OnBoard->Pmkp = _driver->OnBoard->Pmkp+(_move->_engine->Motor->Omega/tempx*Kp-_driver->OnBoard->Pmkp)*dT;
    if(!_driver->OnBoard->EngineStarted && _driver->OnBoard->Pmkp < 0.1) _driver->OnBoard->Pmkp = 0;

    // выключатель стеклоочистителя
    _driver->OnBoard->DriverWiper_mode = 0;
    if(_driver->OnBoard->BatteryActive)
    {
        if(_driver->CompositeSwitch->WiperModeA) _driver->OnBoard->DriverWiper_mode = 1;
        if(_driver->CompositeSwitch->WiperModeB) _driver->OnBoard->DriverWiper_mode = 2;
        if(_driver->CompositeSwitch->WiperModeC) _driver->OnBoard->DriverWiper_mode = 3;
    }

    // жалюзи открыты
    _driver->OnBoard->Shutters = 1.0f;

    // звуковой сигнал
    if(_driver->OnBoard->BatteryActive && !_driver->OnBoard->SMU)
    {
        if(_driver->CompositeSwitch->Signal) _driver->OnBoard->Signal = true; else _driver->OnBoard->Signal = false;
    }
    else _driver->OnBoard->Signal = false;

    if(_driver->OnBoard->Signal) _driver->OnBoard->SignalTime = QTime::currentTime().addSecs(SignalDelay);
    if(_driver->OnBoard->SignalTime>QTime::currentTime() && qAbs(_move->Move->bodyVelocity) > 1.0f) _driver->OnBoard->SignalOnMove = true;
    if(_driver->OnBoard->SignalTime>QTime::currentTime() && _move->_engine->Motor->Started) _driver->OnBoard->SignalOnEngineStarted = true;

    // поворотники
    _driver->OnBoard->RightTurnSwitch = _driver->CompositeSwitch->RightTurnSwitch;
    _driver->OnBoard->LeftTurnSwitch = _driver->CompositeSwitch->LeftTurnSwitch;

    // выбор левого бака
    if(_driver->Switches->LeftFuelTank) _driver->OnBoard->FuelLevel = _driver->OnBoard->LeftFuelTankLevel; else _driver->OnBoard->FuelLevel = _driver->OnBoard->RightFuelTankLevel;

    // Режим ручной подкачки шин передней оси
    if(!_driver->TyreValves->FrontAirTapPump && !_driver->TyreValves->FrontAirTapDischarge) _driver->OnBoard->FrontManualWheelPump = 0;
    if(_driver->TyreValves->FrontAirTapPump) _driver->OnBoard->FrontManualWheelPump = 1;
    if(_driver->TyreValves->FrontAirTapDischarge) _driver->OnBoard->FrontManualWheelPump = -1;

    // Режим ручной подкачки шин задней оси
    if(!_driver->TyreValves->RearAirTapPump && !_driver->TyreValves->RearAirTapDischarge) _driver->OnBoard->RearManualWheelPump = 0;
    if(_driver->TyreValves->RearAirTapPump) _driver->OnBoard->RearManualWheelPump = 1;
    if(_driver->TyreValves->RearAirTapDischarge) _driver->OnBoard->RearManualWheelPump = -1;

    // переключатель света
    if(!_driver->Switches->Sidelights && !_driver->Switches->CentralLight) _driver->OnBoard->CentralLight = 0;
    if(_driver->Switches->Sidelights) _driver->OnBoard->CentralLight = 1;
    if(_driver->Switches->CentralLight) _driver->OnBoard->CentralLight = 2;

    // корректор фар
    if(_driver->Switches->CentralLightAdjustLevel0) _driver->OnBoard->CentralLightAdjustLevel = 0;
    if(_driver->Switches->CentralLightAdjustLevel1) _driver->OnBoard->CentralLightAdjustLevel = 1;
    if(_driver->Switches->CentralLightAdjustLevel2) _driver->OnBoard->CentralLightAdjustLevel = 2;
    if(_driver->Switches->CentralLightAdjustLevel3) _driver->OnBoard->CentralLightAdjustLevel = 3;
    if(_driver->Switches->CentralLightAdjustLevel4) _driver->OnBoard->CentralLightAdjustLevel = 4;

    // габариты, фары и сигнальные лампы
    if(_driver->OnBoard->BatteryActive && _driver->OnBoard->V_board > 5.0f)
    {
        if(_driver->Fuse->ExternalLights_Ready) // габариты
        {
            if(_driver->OnBoard->Gear == -1 && !_driver->OnBoard->SMU) _driver->Sidelights->ReverseLight = true;
            if(_driver->Switches->ArticulatedLorry && !_driver->OnBoard->SMU) _driver->Sidelights->LorryLight = true;
            if(_driver->Switches->Sidelights && !_driver->OnBoard->SMU) _driver->Sidelights->Sidelights = true;
            if((_driver->OnBoard->LeftTurnSwitch || _driver->Switches->Alarm) && Flash && !_driver->OnBoard->SMU) _driver->Sidelights->TurnLeftLights = true; else _driver->Sidelights->TurnLeftLights = false;
            if((_driver->OnBoard->RightTurnSwitch || _driver->Switches->Alarm) && Flash && !_driver->OnBoard->SMU) _driver->Sidelights->TurnRightLights = true; else _driver->Sidelights->TurnRightLights = false;
            if(_driver->Controls->Brake > 0 && !_driver->OnBoard->SMU) _driver->Sidelights->StopLights = true; else _driver->Sidelights->StopLights = false;
        }
        if(_driver->Fuse->SignalLamp_Ready) // сигнальные лампы
        {
            if(_driver->Switches->ArticulatedLorry && !_driver->OnBoard->SMU) _driver->Lamps->LorryLamp = true; else _driver->Lamps->LorryLamp = false;                     // сигнал включения знака автопоезда
            if(_driver->Switches->Sidelights) _driver->Lamps->DashBoardLight = true;                                                                                        // подсветка приборной доски
            if(_driver->Switches->Sidelights && !_driver->OnBoard->SMU) _driver->Lamps->SidelightsLamp = true; else _driver->Lamps->SidelightsLamp = false;                 // сигнал включения габаритов
            if(_driver->Controls->WheelsLock) _driver->Lamps->CrossaxleDiffLockLamp = true; else _driver->Lamps->CrossaxleDiffLockLamp = false;                             // сигнал блокировки межколесного дифференциала
            if(_driver->Controls->DiffLock) _driver->Lamps->InteraxleDiffLockLamp = true; else _driver->Lamps->InteraxleDiffLockLamp = false;                               // сигнал блокировки межосевого дифференциала
            if(_driver->CompositeSwitch->DistanceLight) _driver->Lamps->HighBeamLamp = true; else _driver->Lamps->HighBeamLamp = false;                                     // дальний свет
            if((_driver->OnBoard->RightTurnSwitch || _driver->OnBoard->LeftTurnSwitch || _driver->Switches->Alarm) && Flash) _driver->Lamps->TurnLamp = true; else _driver->Lamps->TurnLamp = false;    // поворотники
            if(_driver->Controls->HandBrake && Flash) _driver->Lamps->HandBrakeLamp = true; else _driver->Lamps->HandBrakeLamp = false;                                     // ручной тормоз
            if(_driver->OnBoard->FuelLevel < 12.0f) _driver->Lamps->LowFuelLamp = true; else _driver->Lamps->LowFuelLamp = false;                                           // мало топлива
            if(_driver->OnBoard->WheelsAirPressurePower && (_driver->OnBoard->Press_FrontReducer < 7.0f || _driver->OnBoard->Press_RearReducer < 7.0f) && Flash) _driver->Lamps->WheelsAirPressure_Fault = true;   // неисправность системы подкачки шин
            if(_driver->OnBoard->Preheater) _driver->Lamps->PreheaterLamp = true; else _driver->Lamps->PreheaterLamp = false;                                               // предпусковой подогреватель ПЖД16
            if(_driver->OnBoard->SMU) _driver->Lamps->SMULamp = true; else _driver->Lamps->SMULamp = false;                                                                 // светомаскировка
            if(_driver->OnBoard->EngineBrake) _driver->Lamps->EngineBrakeLamp = true; else _driver->Lamps->EngineBrakeLamp = false;                                         // моторный тормоз
            if(_errors->FaultInjection.Active[NO_BRAKE]) _driver->Lamps->BrakeFaultLamp = true; else _driver->Lamps->BrakeFaultLamp = false;                                // неисправность тормозов
            if(_errors->FaultInjection.Active[NO_BATTERY_CHARGING]) _driver->Lamps->BatteryLamp = true; else _driver->Lamps->BatteryLamp = false;                           // отсутствие зарадки батарей
            _driver->Doping_2->LeftGreenLamp = true;                                                                                                                        // исправность левого генератора системы пожаротушения
            _driver->Doping_2->RightGreenLamp = true;                                                                                                                       // исправность правого генератора системы пожаротушения
            if(_driver->OnBoard->WheelsAirPressurePower)
            {
                switch(_driver->OnBoard->WheelsAirPressureMode)                                                                                                                 // режим системы регулирования давления воздуха в шинах
                {
                    case 1:
                        _driver->Lamps->WheelsAirPressure_Sand = true;                                                                                                          // песок
                    break;
                    case 2:
                        _driver->Lamps->WheelsAirPressure_Terrain = true;                                                                                                       // грунт
                    break;
                    case 3:
                        _driver->Lamps->WheelsAirPressure_Highway = true;                                                                                                       // шоссе
                    break;
                }
            }
            if(_driver->OnBoard->EngineStarted)
            {
                if(_driver->OnBoard->Pmdv < 0.7f) _driver->Lamps->EngineOilPressLamp = true; else _driver->Lamps->EngineOilPressLamp = false;                               // низкое давление масла в двигателе
                if(_driver->OnBoard->T_OG > 103.0f) _driver->Lamps->WaterOverheatLamp = true; else _driver->Lamps->WaterOverheatLamp = false;                               // подогрев воды в двигателе
            }
            if(!_driver->OnBoard->EngineStarted)
            {
                if(_driver->Switches->Ignition_I)
                {
                    if(_driver->OnBoard->Time_Ignition_I < 2.0f) _driver->Lamps->EngineOilPressLamp = true;                                                                 // низкое давление масла в двигателе
                    if(_driver->OnBoard->Time_Ignition_I < 2.0f) _driver->Lamps->ColdStartLamp = true;                                                                      // холодный пуск
                    if(_driver->OnBoard->Time_Ignition_I < 2.0f) _driver->Lamps->DiagnosticsLamp = true;                                                                    // диагностика
                    _driver->Lamps->BatteryLamp = true;                                                                                                                     // батарея
                    if(_driver->OnBoard->T_OG < -15.0f) _driver->Lamps->ColdStartLamp = true;                                                                               // холодный пуск
                }
            }
            if(_driver->Switches->Diagnostics)  // диагностика
            {
                _driver->Lamps->DiagnosticsLamp = true;        // диагностика
            }
        }
        if(_driver->Fuse->HeadLamp_Ready) // фары
        {
            if(_driver->Switches->CentralLight)
            {
                if(!_driver->OnBoard->SMU) _driver->Sidelights->Sidelights = true;
                _driver->Sidelights->CentralLights = true;
                if(_driver->OnBoard->RearFogLight && !_driver->OnBoard->SMU) _driver->Sidelights->RearFogLight = true;
                if(_driver->CompositeSwitch->DistanceLight) _driver->Sidelights->DistanceLight = true;
            }
        }
    }

    // сила стояночного тормоза
    if(_driver->Controls->HandBrake)
    {
        _driver->OnBoard->F_HandBrake = 50;
        _move->_transmission->Transmission->HandBrake = 2500;
    }
    else
    {
        _driver->OnBoard->F_HandBrake = 0;
        _move->_transmission->Transmission->HandBrake = 0;
    }

    // общая сила торможения
    _driver->OnBoard->F_Stop = _driver->OnBoard->F_HandBrake + _driver->Controls->Brake;

    // щиток приборов
    _driver->Gauges->Speedometer = qAbs(_move->Move->bodyVelocity);                // спидометр
    _driver->Gauges->Tachometer = _move->_engine->Motor->Omega;                    // тахометр
    _driver->Gauges->SystemAirPressure = _driver->OnBoard->Pvoz;                   // манометр воздушной системы (белая стрелка)
    _driver->Gauges->BrakeSystemAirPressure = _driver->OnBoard->BrakeSystemAirPressure;      // манометр давления воздуха в пневмоприводе тормозов (красная стрелка)
    _driver->Gauges->FrontWheelsAirPressure = _driver->OnBoard->FrontWheelsAirPressure;      // манометр шин передней оси (красная стрелка)
    _driver->Gauges->RearWheelsAirPressure = _driver->OnBoard->RearWheelsAirPressure;        // манометр шин задней оси (белая стрелка)
    _driver->Gauges->EngineOilPressure = _driver->OnBoard->Pmdv*10.0f;             // манометр масла двигателя
    _driver->Gauges->EngineCoolantTemperature = _driver->OnBoard->T_OG;            // термометр ОЖ
    _driver->Gauges->FuelLevel = _driver->OnBoard->FuelLevel/68.0f*100.0f;         // указатель уровня топлива
    _driver->Gauges->Voltage = _driver->OnBoard->V_board;                          // напряжение бортовой сети

    // обороты двигателя
    _driver->OnBoard->Omega = _move->_engine->Motor->Omega;

    // задать частоту дискретизации звука работающего двигателя в зависимости от частоты оборотов
    _audio->SetSampleRate(3,8,_audio->GetEngineSampleRate(_driver->OnBoard->Omega));

    if(!_driver->OnBoard->BatteryActive && !_driver->Switches->Ignition_I && !_driver->Switches->Ignition_II)
    {
        // спидометр
        _driver->Gauges->Speedometer = 0;
        // тахометр
        _driver->Gauges->Tachometer = 0;
        // манометр давления воздуха в пневмоприводе тормозов
        //_driver->Gauges->SystemAirPressure = 0;
        // манометр давления воздуха в пневмоприводе тормозов
        //_driver->Gauges->BrakeSystemAirPressure = 0;
        // манометр шин передняя ось
        _driver->Gauges->FrontWheelsAirPressure = 0;
        // манометр шин задняя ось
        _driver->Gauges->RearWheelsAirPressure = 0;
        // манометр масла двигателя
        _driver->Gauges->EngineOilPressure = 0;
        // вольтметр
        _driver->Gauges->Voltage = 0;
        // термометр ОЖ
        _driver->Gauges->EngineCoolantTemperature = 0;
        // указатель уровня топлива
        _driver->Gauges->FuelLevel = 0;
     }

    if(!_driver->OnBoard->EngineStarted)
    {

    }

}
