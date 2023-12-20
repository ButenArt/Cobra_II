#include "COBRA2.h"

T_COBRA2_driver::T_COBRA2_driver(QObject *parent) : QObject(parent)
{
    Gauges = new TGauges();
    Gauges->Speedometer = 0;                      // спидометр
    Gauges->Tachometer = 0;                       // тахометр
    Gauges->SystemAirPressure = 0;                // манометр воздушной системы (белая стрелка)
    Gauges->BrakeSystemAirPressure = 0;           // манометр давления воздуха в пневмоприводе тормозов (красная стрелка)
    Gauges->EngineOilPressure = 0;                // манометр масла двигателя
    Gauges->EngineCoolantTemperature = 0;         // термометр ОЖ
    Gauges->FuelLevel = 0;                        // указатель уровня топлива
    Gauges->Voltage = 0;                          // напряжение бортовой сети
    Gauges->FrontWheelsAirPressure = 0;           // показания прибора манометра шин передней оси (красная стрелка)
    Gauges->RearWheelsAirPressure = 0;            // показания прибора манометра шин задней оси (белая стрелка)

    Switches = new TSwitches();
    Switches->Ignition_I = false;                 // замок зажигания в положении I
    Switches->Ignition_II = false;                // замок зажигания в положении II (нефиксируемое)
    Switches->Ignition_III = false;               // замок зажигания в положении III (блокировка руля)
    Switches->Alarm = false;                      // выключатель аварийной сигнализации
    Switches->LeftFuelTank = false;               // переключатель баков
    Switches->RearFogLight = false;               // выключатель задних противотуманных фонарей
    Switches->Diagnostics = false;                // выключатель "диагностика"
    Switches->SMU = false;                        // выключатель режима светомаскировки
    Switches->Sidelights = false;                 // габариты
    Switches->CentralLight = false;               // центральный свет
    Switches->ArticulatedLorry = false;           // выключатель "Автопоезд"
    Switches->Preheater = false;                  // выключатель предпускового подогревателя (ПЖД16)
    Switches->WheelsAirPressureMode = false;      // переключатель режимов давления воздуха в шинах
    Switches->WheelsAirPressurePower = false;     // выключатель системы регулирования давления воздуха в шинах
    Switches->Battery = false;                    // выключатель батарей
    Switches->CentralLightAdjustLevel0 = true;    // корректор фар положение 0
    Switches->CentralLightAdjustLevel1 = false;   // корректор фар положение 1
    Switches->CentralLightAdjustLevel2 = false;   // корректор фар положение 2
    Switches->CentralLightAdjustLevel3 = false;   // корректор фар положение 3
    Switches->CentralLightAdjustLevel4 = false;   // корректор фар положение 4

    CompositeSwitch = new TCompositeSwitch();
    CompositeSwitch->LeftTurnSwitch = false;      // переключатель поворота на лево
    CompositeSwitch->RightTurnSwitch = false;     // переключатель поворота на право
    CompositeSwitch->WiperModeA = false;          // выключатель стеклоочистителя первая скорость
    CompositeSwitch->WiperModeB = false;          // выключатель стеклоочистителя вторая скорость
    CompositeSwitch->WiperModeC = false;          // выключатель стеклоочистителя прерывистый режим
    CompositeSwitch->Washer = false;              // насос омывателя стекла
    CompositeSwitch->DistanceLight = false;       // дальний свет фар
    CompositeSwitch->Signal = false;              // звуковой сигнал

    Lamps = new TLamps();
    Lamps->EngineOilPressLamp = false;            // сд. давление масла в двигателе
    Lamps->LorryLamp = false;                     // сд. фонари автопоезда
    Lamps->WaterLevelLamp = false;                // сд. уровень ОЖ
    Lamps->SMULamp = false;                       // сд. режим светомаскировки
    Lamps->ColdStartLamp = false;                 // сд. холодный пуск
    Lamps->TurnLamp = false;                      // сд. указатель поворота
    Lamps->SidelightsLamp = false;                // сд. габаритные огни
    Lamps->DiagnosticsLamp = false;               // сд. диагностика
    Lamps->TrailerTurnLamp = false;               // сд. указатель поворота прицепа
    Lamps->LowFuelLamp = false;                   // сд. мало топлива
    Lamps->WaterInFuelLamp = false;               // сд. наличие воды в топливе
    Lamps->AirFilterLamp = false;                 // сд. засореный воздушный фильтр
    Lamps->BatteryLamp = false;                   // сд. батарея
    Lamps->PreheaterLamp = false;                 // сд. предпусковой подогреватель (ПЖД16)
    Lamps->EngineBrakeLamp = false;               // сд. моторный тормоз
    Lamps->InteraxleDiffLockLamp = false;         // сд. сигнал блокировки межосевого дифференциала
    Lamps->CrossaxleDiffLockLamp = false;         // сд. сигнал блокировки межколесного дифференциала
    Lamps->HighBeamLamp = false;                  // сд. дальний свет
    Lamps->HandBrakeLamp = false;                 // сд. включение стояночной тормозной системы
    Lamps->BrakeFaultLamp = false;                // сд. неисправность тормозной системы
    Lamps->WaterOverheatLamp = false;             // сд. перегрев ОЖ
    Lamps->WheelsAirPressure_Sand = false;        // сд. режим "песок" давления воздуха в шинах
    Lamps->WheelsAirPressure_Terrain = false;     // сд. режим "грунт" давления воздуха в шинах
    Lamps->WheelsAirPressure_Highway = false;     // сд. режим "шоссе" давления воздуха в шинах
    Lamps->WheelsAirPressure_Fault = false;       // сд. неисправность или низкое давление в шинах
    Lamps->DashBoardLight = false;                // подсветка приборной доски

    Doping_2 = new TDoping_2();
    Doping_2->Power = false;                      // тумблер запуска генераторов
    Doping_2->LeftGreenLamp = false;              // сигнальная лампа исправности левого генератора
    Doping_2->LeftRedLamp = false;                // сигнальная лампа неисправности левого генератора
    Doping_2->RightGreenLamp = false;             // сигнальная лампа исправности правого генератора
    Doping_2->RightRedLamp = false;               // сигнальная лампа неисправности правого генератора

    Fuse = new TFuse();
    Fuse->ExternalLights_Ready = true;            // габариты
    Fuse->SignalLamp_Ready = true;                // сигнальные лампы
    Fuse->HeadLamp_Ready = true;                  // фары

    EdgeContacts = new TEdgeContacts();
    EdgeContacts->FuelTapLeftTank = true;         // топливный кран левого бака
    EdgeContacts->FuelTapRightTank = true;        // топливный кран правого бака

    Controls = new TControls();
    Controls->EngineBrake = false;                // выключатель моторного тормоза
    Controls->Brake = 0;                          // тормоз в %
    Controls->Gas = 0;                            // газ в %
    Controls->Coupling = 0;                       // сцепление в %
    Controls->Steering = 0;                       // руль в %
    Controls->Gear_1 = false;                     // КПП 1 передача
    Controls->Gear_2 = false;                     // КПП 2 передача
    Controls->Gear_3 = false;                     // КПП 3 передача
    Controls->Gear_4 = false;                     // КПП 4 передача
    Controls->Gear_5 = false;                     // КПП 5 передача
    Controls->Gear_R = false;                     // КПП задняя передача
    Controls->TransferGear_Low = false;           // раздат. коробка - пониженная
    Controls->TransferGear_High = false;          // раздат. коробка - повышенная
    Controls->DiffLock = false;                   // блокировка межосевого дифференциала (сухая дорога/скользкая дорога)
    Controls->WheelsLock = false;                 // блокировка межколесного дифференциала
    Controls->HandBrake = false;                  // ручной тормоз

    TyreValves = new TTyreValves();
    TyreValves->FrontAirTapDischarge = false;     // выпуск воздуха из шин передняя ось
    TyreValves->FrontAirTapPump = false;          // накачка шин передняя ось
    TyreValves->RearAirTapDischarge = false;      // выпуск воздуха из шин задняя ось
    TyreValves->RearAirTapPump = false;           // накачка шин задняя ось
    TyreValves->TyreTap_1 = true;                 // шинный кран 1 колеса
    TyreValves->TyreTap_2 = true;                 // шинный кран 2 колеса
    TyreValves->TyreTap_3 = true;                 // шинный кран 3 колеса
    TyreValves->TyreTap_4 = true;                 // шинный кран 4 колеса

    Sidelights = new TSidelights();
    Sidelights->Sidelights = false;               // фонари габаритов
    Sidelights->TurnLeftLights = false;           // фонари сигнала левого поворота
    Sidelights->TurnRightLights = false;          // фонари сигнала правого поворота
    Sidelights->StopLights = false;               // фонари стоп сигнала
    Sidelights->ReverseLight = false;             // фонарь заднего хода
    Sidelights->CentralLights = false;            // фары (ближний свет)
    Sidelights->DistanceLight = false;            // фары (дальний свет)
    Sidelights->RearFogLight = false;             // задние противотуманные фонари
    Sidelights->LorryLight = false;               // фонарь "Автопоезд"

    OnBoard = new TOnBoard();
    OnBoard->Time = 0;                            // время существования в секундах
    OnBoard->EngineStarted = false;               // двигатель запущен
    OnBoard->SteeringBlock = true;                // блокировка руля
    OnBoard->TransmissionBlock = false;           // блокировка КПП
    OnBoard->Direction = true;                    // направление движения (true - вперед, false - назад)
    OnBoard->F_Stop = 0;                          // сила торможения
    OnBoard->Omega = 0;                           // обороты двигателя
    for(int i=0; i<4; i++) { OnBoard->brakeTorque[i] = 0; }  // компенсирующий момент тормоза
    for(int i=0; i<4; i++) { OnBoard->WheelsTorque[i] = 0; } // крутящий момент на колесах в Н.м.
    OnBoard->Gear = 0;                            // передача
    OnBoard->TransferGear = 0;                    // -1 пониженная, 0 нейтральная, 1 повышенная передача
    OnBoard->Starter = 0;                         // стартер 0-не работает, 1-от АКБ, 2-от воздуха, 3-от обеих
    OnBoard->Starter_f = 0;                       // сила стартера
    OnBoard->BatteryActive = false;               // батареи подключены
    OnBoard->V_akb = 24.0f;                       // заряд аккумуляторных батарей
    OnBoard->I_board = 0;                         // ток бортовой сети
    OnBoard->V_board = 0;                         // напряжение бортовой сети
    OnBoard->Pmdv = 0;                            // давление масла в двигателе
    OnBoard->Pmkp = 0;                            // давление масла в РК
    OnBoard->Pvoz = 700.0f;                       // давление воздуха в системе
    OnBoard->Generator = false;                   // генератор
    OnBoard->Compressor = false;                  // работает компрессор
    OnBoard->Shutters = 0;                        // положение жалюзи 0..1
    OnBoard->EngineVentilation = true;            // принудительная вентиляция двигателя false - выкл., true - вкл.
    OnBoard->F_HandBrake = 0;                     // сила стояночного тормоза
    OnBoard->BrakeSystemAirPressure = 7.0f;       // манометр давления воздуха в пневмоприводе тормозов
    OnBoard->DriverWiper_mode = 0;                // стеклоочиститель 0-выключен, 1-первая скорость, 2-вторая скорость, 3-прерывистый режим
    OnBoard->T_init = 20.0f;                      // температура окружающего воздуха (задается инструктором)
    OnBoard->T_Motor = OnBoard->T_init;           // температура двигателя
    OnBoard->T_Oil = OnBoard->T_init;             // температура масла
    OnBoard->T_OG = OnBoard->T_init;              // температура охлождающей жидкости
    OnBoard->T_Svecha = OnBoard->T_init;          // температура свечи подогревателя
    OnBoard->LeftFuelTankLevel = 68.0f;           // уровень топлива левого бака
    OnBoard->RightFuelTankLevel = 68.0f;          // уровень топлива правого бака
    OnBoard->FuelLevel = 0;                       // уровень топлива на приборе
    OnBoard->FrontWheelsAirPressure = 0;          // показания прибора манометра шин передней оси в (Па)
    OnBoard->RearWheelsAirPressure = 0;           // показания прибора манометра шин задней оси в (Па)
    OnBoard->Press_FrontReducer = 40.1f;          // давление в редукторе передней оси в (Па)
    OnBoard->Press_RearReducer = 49.6f;           // давление в редукторе задней оси в (Па)
    for(int i=0; i<4; i++) { OnBoard->Press_tyre[i] = 40.0f; }    // давление в шинах в (Па) (колеса нумеруются от правого заднего до левого заднего против часовой стрелки)
    for(int i=0; i<4; i++) { OnBoard->Press_tyre_level[i] = 3; }  // уровень давление в шинах (0 - спущено, 1 - песок, 2 - грунт, 3 - шоссе)
    OnBoard->StopEngine = false;                  // моментальная остановка двигателя (МОД)
    OnBoard->Signal = false;                      // звуковой сигнал
    OnBoard->LeftTurnSwitch = false;              // переключатель поворота на лево
    OnBoard->RightTurnSwitch = false;             // переключатель поворота на право
    OnBoard->SMU = false;                         // режим светомаскировки
    OnBoard->Preheater = false;                   // предпусковой подогреватель (ПЖД16)
    OnBoard->RearFogLight = false;                // выключатель задних противотуманных фонарей
    OnBoard->FrontManualWheelPump = 0;            // режим ручной подкачки шин передней оси
    OnBoard->RearManualWheelPump = 0;             // режим ручной подкачки шин задней оси
    OnBoard->WheelsAirPressureMode = 3;           // режим давления воздуха в шинах
    OnBoard->WheelsAirPressurePower = false;      // система регулирования давления воздуха в шинах
    OnBoard->EngineBrake = false;                 // моторный тормоз
    OnBoard->CentralLight = 0;                    // положение переключателя света
    OnBoard->CentralLightAdjustLevel = 0;         // положение корректора фар
    //другие параметры, для обмена с другими модулями//
    OnBoard->K_T_og_v = 300.0f;                   // коэффициент теплообмена охлаждяющая жидкость - воздух
    OnBoard->SignalTime = QTime::currentTime();   // время подачи звукового сигнала
    OnBoard->SignalOnEngineStarted = false;       // звуковой сигнал перед пуском двигателя
    OnBoard->SignalOnMove = false;                // звуковой сигнал перед началом движения
    OnBoard->Time_Start = 0;                      // время непрерывного нажатия на кнопку стартера
    OnBoard->Time_Battery = 0;                    // время непрерывного нажатия на кнопку БАТАРЕИ
    //OnBoard->Time_Preheater = 0;                  // время непрерывного нажатия на кнопку подогревателя ПЖД16
    //OnBoard->Time_SMU = 0;                        // время непрерывного нажатия на кнопку режима светомаскировки
    //OnBoard->Time_EngineBrake = 0;                // время непрерывного нажатия на кнопку моторного тормоза
    //OnBoard->Time_WheelsPressurePower = 0;        // время непрерывного нажатия на кнопку включения системы регулирования давления воздуха в шинах
    OnBoard->Time_WheelsPressureMode = 0;         // время непрерывного нажатия на кнопку режима системы регулирования давления воздуха в шинах
    OnBoard->Time_RearFogLight = 0;               // время непрерывного нажатия на кнопку выключателя задних противотуманных фонарей
    OnBoard->Time_Ignition_I = 0;                 // время непрерывного состояния замка зажигания в положении I (в миллисекундах)
    OnBoard->Collision = false;                   // Столкновение
}

T_COBRA2_driver::~T_COBRA2_driver()
{
    delete Gauges;
    delete Switches;
    delete CompositeSwitch;
    delete Lamps;
    delete Doping_2;
    delete Fuse;
    delete EdgeContacts;
    delete Controls;
    delete TyreValves;
    delete Sidelights;
    delete OnBoard;
}

void T_COBRA2_driver::Init()
{
    Gauges->Speedometer = 0;                      // спидометр
    Gauges->Tachometer = 0;                       // тахометр
    Gauges->SystemAirPressure = 0;                // манометр воздушной системы (белая стрелка)
    Gauges->BrakeSystemAirPressure = 0;           // манометр давления воздуха в пневмоприводе тормозов (красная стрелка)
    Gauges->EngineOilPressure = 0;                // манометр масла двигателя
    Gauges->EngineCoolantTemperature = 0;         // термометр ОЖ
    Gauges->FuelLevel = 0;                        // указатель уровня топлива
    Gauges->Voltage = 0;                          // напряжение бортовой сети
    Gauges->FrontWheelsAirPressure = 0;           // показания прибора манометра шин передней оси (красная стрелка)
    Gauges->RearWheelsAirPressure = 0;            // показания прибора манометра шин задней оси (белая стрелка)

    Switches->Ignition_I = false;                 // замок зажигания в положении I
    Switches->Ignition_II = false;                // замок зажигания в положении II (нефиксируемое)
    Switches->Ignition_III = false;               // замок зажигания в положении III (блокировка руля)
    Switches->Alarm = false;                      // выключатель аварийной сигнализации
    Switches->LeftFuelTank = false;               // переключатель баков
    Switches->RearFogLight = false;               // выключатель задних противотуманных фонарей
    Switches->Diagnostics = false;                // выключатель "диагностика"
    Switches->SMU = false;                        // выключатель режима светомаскировки
    Switches->Sidelights = false;                 // габариты
    Switches->CentralLight = false;               // центральный свет
    Switches->ArticulatedLorry = false;           // выключатель "Автопоезд"
    Switches->Preheater = false;                  // выключатель предпускового подогревателя (ПЖД16)
    Switches->WheelsAirPressureMode = false;      // переключатель режимов давления воздуха в шинах
    Switches->WheelsAirPressurePower = false;     // выключатель системы регулирования давления воздуха в шинах
    Switches->Battery = false;                    // выключатель батарей
    Switches->CentralLightAdjustLevel0 = true;    // корректор фар положение 0
    Switches->CentralLightAdjustLevel1 = false;   // корректор фар положение 1
    Switches->CentralLightAdjustLevel2 = false;   // корректор фар положение 2
    Switches->CentralLightAdjustLevel3 = false;   // корректор фар положение 3
    Switches->CentralLightAdjustLevel4 = false;   // корректор фар положение 4

    CompositeSwitch->LeftTurnSwitch = false;      // переключатель поворота на лево
    CompositeSwitch->RightTurnSwitch = false;     // переключатель поворота на право
    CompositeSwitch->WiperModeA = false;          // выключатель стеклоочистителя первая скорость
    CompositeSwitch->WiperModeB = false;          // выключатель стеклоочистителя вторая скорость
    CompositeSwitch->WiperModeC = false;          // выключатель стеклоочистителя прерывистый режим
    CompositeSwitch->Washer = false;              // насос омывателя стекла
    CompositeSwitch->DistanceLight = false;       // дальний свет фар
    CompositeSwitch->Signal = false;              // звуковой сигнал

    Lamps->EngineOilPressLamp = false;            // сд. давление масла в двигателе
    Lamps->LorryLamp = false;                     // сд. фонари автопоезда
    Lamps->WaterLevelLamp = false;                // сд. уровень ОЖ
    Lamps->SMULamp = false;                       // сд. режим светомаскировки
    Lamps->ColdStartLamp = false;                 // сд. холодный пуск
    Lamps->TurnLamp = false;                      // сд. указатель поворота
    Lamps->SidelightsLamp = false;                // сд. габаритные огни
    Lamps->DiagnosticsLamp = false;               // сд. диагностика
    Lamps->TrailerTurnLamp = false;               // сд. указатель поворота прицепа
    Lamps->LowFuelLamp = false;                   // сд. мало топлива
    Lamps->WaterInFuelLamp = false;               // сд. наличие воды в топливе
    Lamps->AirFilterLamp = false;                 // сд. засореный воздушный фильтр
    Lamps->BatteryLamp = false;                   // сд. батарея
    Lamps->PreheaterLamp = false;                 // сд. предпусковой подогреватель (ПЖД16)
    Lamps->EngineBrakeLamp = false;               // сд. моторный тормоз
    Lamps->InteraxleDiffLockLamp = false;         // сд. сигнал блокировки межосевого дифференциала
    Lamps->CrossaxleDiffLockLamp = false;         // сд. сигнал блокировки межколесного дифференциала
    Lamps->HighBeamLamp = false;                  // сд. дальний свет
    Lamps->HandBrakeLamp = false;                 // сд. включение стояночной тормозной системы
    Lamps->BrakeFaultLamp = false;                // сд. неисправность тормозной системы
    Lamps->WaterOverheatLamp = false;             // сд. перегрев ОЖ
    Lamps->WheelsAirPressure_Sand = false;        // сд. режим "песок" давления воздуха в шинах
    Lamps->WheelsAirPressure_Terrain = false;     // сд. режим "грунт" давления воздуха в шинах
    Lamps->WheelsAirPressure_Highway = false;     // сд. режим "шоссе" давления воздуха в шинах
    Lamps->WheelsAirPressure_Fault = false;       // сд. неисправность или низкое давление в шинах
    Lamps->DashBoardLight = false;                // подсветка приборной доски

    Doping_2->Power = false;                      // тумблер запуска генераторов
    Doping_2->LeftGreenLamp = false;              // сигнальная лампа исправности левого генератора
    Doping_2->LeftRedLamp = false;                // сигнальная лампа неисправности левого генератора
    Doping_2->RightGreenLamp = false;             // сигнальная лампа исправности правого генератора
    Doping_2->RightRedLamp = false;               // сигнальная лампа неисправности правого генератора

    Fuse->ExternalLights_Ready = true;            // габариты
    Fuse->SignalLamp_Ready = true;                // сигнальные лампы
    Fuse->HeadLamp_Ready = true;                  // фары

    EdgeContacts->FuelTapLeftTank = true;         // топливный кран левого бака
    EdgeContacts->FuelTapRightTank = true;        // топливный кран правого бака

    Controls->EngineBrake = false;                // выключатель моторного тормоза
    Controls->Brake = 0;                          // тормоз в %
    Controls->Gas = 0;                            // газ в %
    Controls->Coupling = 0;                       // сцепление в %
    Controls->Steering = 0;                       // руль в %
    Controls->Gear_1 = false;                     // КПП 1 передача
    Controls->Gear_2 = false;                     // КПП 2 передача
    Controls->Gear_3 = false;                     // КПП 3 передача
    Controls->Gear_4 = false;                     // КПП 4 передача
    Controls->Gear_5 = false;                     // КПП 5 передача
    Controls->Gear_R = false;                     // КПП задняя передача
    Controls->TransferGear_Low = false;           // раздат. коробка - пониженная
    Controls->TransferGear_High = false;          // раздат. коробка - повышенная
    Controls->DiffLock = false;                   // блокировка межосевого дифференциала (сухая дорога/скользкая дорога)
    Controls->WheelsLock = false;                 // блокировка межколесного дифференциала
    Controls->HandBrake = false;                  // ручной тормоз

    TyreValves->FrontAirTapDischarge = false;     // выпуск воздуха из шин передняя ось
    TyreValves->FrontAirTapPump = false;          // накачка шин передняя ось
    TyreValves->RearAirTapDischarge = false;      // выпуск воздуха из шин задняя ось
    TyreValves->RearAirTapPump = false;           // накачка шин задняя ось
    TyreValves->TyreTap_1 = true;                 // шинный кран 1 колеса
    TyreValves->TyreTap_2 = true;                 // шинный кран 2 колеса
    TyreValves->TyreTap_3 = true;                 // шинный кран 3 колеса
    TyreValves->TyreTap_4 = true;                 // шинный кран 4 колеса

    Sidelights->Sidelights = false;               // фонари габаритов
    Sidelights->TurnLeftLights = false;           // фонари сигнала левого поворота
    Sidelights->TurnRightLights = false;          // фонари сигнала правого поворота
    Sidelights->StopLights = false;               // фонари стоп сигнала
    Sidelights->ReverseLight = false;             // фонарь заднего хода
    Sidelights->CentralLights = false;            // фары (ближний свет)
    Sidelights->DistanceLight = false;            // фары (дальний свет)
    Sidelights->RearFogLight = false;             // задние противотуманные фонари
    Sidelights->LorryLight = false;               // фонарь "Автопоезд"

    OnBoard->Time = 0;                            // время существования в секундах
    OnBoard->EngineStarted = false;               // двигатель запущен
    OnBoard->SteeringBlock = true;                // блокировка руля
    OnBoard->TransmissionBlock = false;           // блокировка КПП
    OnBoard->Direction = true;                    // направление движения (true - вперед, false - назад)
    OnBoard->F_Stop = 0;                          // сила торможения
    OnBoard->Omega = 0;                           // обороты двигателя
    for(int i=0; i<4; i++) { OnBoard->brakeTorque[i] = 0; }  // компенсирующий момент тормоза
    for(int i=0; i<4; i++) { OnBoard->WheelsTorque[i] = 0; } // крутящий момент на колесах в Н.м.
    OnBoard->Gear = 0;                            // передача
    OnBoard->TransferGear = 0;                    // -1 пониженная, 0 нейтральная, 1 повышенная передача
    OnBoard->Starter = 0;                         // стартер 0-не работает, 1-от АКБ, 2-от воздуха, 3-от обеих
    OnBoard->Starter_f = 0;                       // сила стартера
    OnBoard->BatteryActive = false;               // батареи подключены
    OnBoard->V_akb = 24.0f;                       // заряд аккумуляторных батарей
    OnBoard->I_board = 0;                         // ток бортовой сети
    OnBoard->V_board = 0;                         // напряжение бортовой сети
    OnBoard->Pmdv = 0;                            // давление масла в двигателе
    OnBoard->Pmkp = 0;                            // давление масла в РК
    OnBoard->Pvoz = 700.0f;                       // давление воздуха в системе
    OnBoard->Generator = false;                   // генератор
    OnBoard->Compressor = false;                  // работает компрессор
    OnBoard->Shutters = 0;                        // положение жалюзи 0..1
    OnBoard->EngineVentilation = true;            // принудительная вентиляция двигателя false - выкл., true - вкл.
    OnBoard->F_HandBrake = 0;                     // сила стояночного тормоза
    OnBoard->BrakeSystemAirPressure = 7.0f;       // манометр давления воздуха в пневмоприводе тормозов
    OnBoard->DriverWiper_mode = 0;                // стеклоочиститель 0-выключен, 1-первая скорость, 2-вторая скорость, 3-прерывистый режим
    OnBoard->T_init = 20.0f;                      // температура окружающего воздуха (задается инструктором)
    OnBoard->T_Motor = OnBoard->T_init;           // температура двигателя
    OnBoard->T_Oil = OnBoard->T_init;             // температура масла
    OnBoard->T_OG = OnBoard->T_init;              // температура охлождающей жидкости
    OnBoard->T_Svecha = OnBoard->T_init;          // температура свечи подогревателя
    OnBoard->LeftFuelTankLevel = 68.0f;           // уровень топлива левого бака
    OnBoard->RightFuelTankLevel = 68.0f;          // уровень топлива правого бака
    OnBoard->FuelLevel = 0;                       // уровень топлива на приборе
    OnBoard->FrontWheelsAirPressure = 0;          // показания прибора манометра шин передней оси в (Па)
    OnBoard->RearWheelsAirPressure = 0;           // показания прибора манометра шин задней оси в (Па)
    OnBoard->Press_FrontReducer = 40.1f;          // давление в редукторе передней оси в (Па)
    OnBoard->Press_RearReducer = 49.6f;           // давление в редукторе задней оси в (Па)
    for(int i=0; i<4; i++) { OnBoard->Press_tyre[i] = 40.0f; }    // давление в шинах в (Па) (колеса нумеруются от правого заднего до левого заднего против часовой стрелки)
    for(int i=0; i<4; i++) { OnBoard->Press_tyre_level[i] = 3; }  // уровень давление в шинах (0 - спущено, 1 - песок, 2 - грунт, 3 - шоссе)
    OnBoard->StopEngine = false;                  // моментальная остановка двигателя (МОД)
    OnBoard->Signal = false;                      // звуковой сигнал
    OnBoard->LeftTurnSwitch = false;              // переключатель поворота на лево
    OnBoard->RightTurnSwitch = false;             // переключатель поворота на право
    OnBoard->SMU = false;                         // режим светомаскировки
    OnBoard->Preheater = false;                   // предпусковой подогреватель (ПЖД16)
    OnBoard->RearFogLight = false;                // выключатель задних противотуманных фонарей
    OnBoard->FrontManualWheelPump = 0;            // режим ручной подкачки шин передней оси
    OnBoard->RearManualWheelPump = 0;             // режим ручной подкачки шин задней оси
    OnBoard->WheelsAirPressureMode = 3;           // режим давления воздуха в шинах
    OnBoard->WheelsAirPressurePower = false;      // система регулирования давления воздуха в шинах
    OnBoard->EngineBrake = false;                 // моторный тормоз
    OnBoard->CentralLight = 0;                    // положение переключателя света
    OnBoard->CentralLightAdjustLevel = 0;         // положение корректора фар
    //другие параметры, для обмена с другими модулями//
    OnBoard->K_T_og_v = 300.0f;                   // коэффициент теплообмена охлаждяющая жидкость - воздух
    OnBoard->SignalTime = QTime::currentTime();   // время подачи звукового сигнала
    OnBoard->SignalOnEngineStarted = false;       // звуковой сигнал перед пуском двигателя
    OnBoard->SignalOnMove = false;                // звуковой сигнал перед началом движения
    OnBoard->Time_Start = 0;                      // время непрерывного нажатия на кнопку стартера
    OnBoard->Time_Battery = 0;                    // время непрерывного нажатия на кнопку БАТАРЕИ
    //OnBoard->Time_Preheater = 0;                  // время непрерывного нажатия на кнопку подогревателя ПЖД16
    //OnBoard->Time_SMU = 0;                        // время непрерывного нажатия на кнопку режима светомаскировки
    //OnBoard->Time_EngineBrake = 0;                // время непрерывного нажатия на кнопку моторного тормоза
    //OnBoard->Time_WheelsPressurePower = 0;        // время непрерывного нажатия на кнопку включения системы регулирования давления воздуха в шинах
    OnBoard->Time_WheelsPressureMode = 0;         // время непрерывного нажатия на кнопку режима системы регулирования давления воздуха в шинах
    OnBoard->Time_RearFogLight = 0;               // выключатель задних противотуманных фонарей
    OnBoard->Time_Ignition_I = 0;                 // время непрерывного состояния замка зажигания в положении I (в миллисекундах)
    OnBoard->Collision = false;                   // Столкновение
}


void T_COBRA2_driver::SetShutters(float i)
{
    //if(!Switches->ShuttersDrive_Ready) return;
    if(i==0) OnBoard->Shutters = 0; else OnBoard->Shutters = OnBoard->Shutters + i;
    if(OnBoard->Shutters < 0) OnBoard->Shutters = 0;
    if(OnBoard->Shutters > 1) OnBoard->Shutters = 1;
}

void T_COBRA2_driver::SetAirDischarge(float i)
{
    OnBoard->Pvoz = OnBoard->Pvoz + i;
}

void T_COBRA2_driver::SetGear(int i, bool manual)
{
    if(i!=OnBoard->Gear)
    {
        if((i>=-1) && (i<=5)) OnBoard->Gear = i;
    }
    if(manual)
    {
        if(OnBoard->Gear == 1) Controls->Gear_1 = true; else Controls->Gear_1 = false;
        if(OnBoard->Gear == 2) Controls->Gear_2 = true; else Controls->Gear_2 = false;
        if(OnBoard->Gear == 3) Controls->Gear_3 = true; else Controls->Gear_3 = false;
        if(OnBoard->Gear == 4) Controls->Gear_4 = true; else Controls->Gear_4 = false;
        if(OnBoard->Gear == 5) Controls->Gear_5 = true; else Controls->Gear_5 = false;
        if(OnBoard->Gear == -1) Controls->Gear_R = true; else Controls->Gear_R = false;
    }
    if(OnBoard->Gear == -1) OnBoard->Direction = false; else OnBoard->Direction = true;
}

void T_COBRA2_driver::SetTransferGear(int i, bool manual)
{
    if(i!=OnBoard->TransferGear)
    {
        if((i>=-1) && (i<=1)) OnBoard->TransferGear = i;
    }
    if(manual)
    {
        if(OnBoard->TransferGear == -1) Controls->TransferGear_Low = true; else Controls->TransferGear_Low = false;
        if(OnBoard->TransferGear == 1) Controls->TransferGear_High = true; else Controls->TransferGear_High = false;
    }
}

float T_COBRA2_driver::SetStop(float p)
{
    p = p + OnBoard->F_Stop;
    if(p<0) p = 0;
    if(p>100) p = 100;
    return p;
}

void T_COBRA2_driver::Press_FrontRedu(float i)
{
    OnBoard->Press_FrontReducer = OnBoard->Press_FrontReducer + i;
    if(OnBoard->Press_FrontReducer < 0) OnBoard->Press_FrontReducer = 0;
    if(OnBoard->Press_FrontReducer > 50.0f) OnBoard->Press_FrontReducer = 50.0f;
    if(i > 0)
    {
        OnBoard->Pvoz = OnBoard->Pvoz - i;
    }
}

void T_COBRA2_driver::Press_RearRedu(float i)
{
    OnBoard->Press_RearReducer = OnBoard->Press_RearReducer + i;
    if(OnBoard->Press_RearReducer < 0) OnBoard->Press_RearReducer = 0;
    if(OnBoard->Press_RearReducer > 50.0f) OnBoard->Press_RearReducer = 50.0f;
    if(i > 0)
    {
        OnBoard->Pvoz = OnBoard->Pvoz - i;
    }
}

void T_COBRA2_driver::Tyre_Redu(BYTE wheelNum)
{
    float temp = 0;
    OnBoard->Press_tyre_level[wheelNum-1] = 0;
    switch(wheelNum)
    {
        case 1:
            if(TyreValves->TyreTap_1)
            {
                temp = OnBoard->Press_FrontReducer - OnBoard->Press_tyre[0];
                if(temp < 0 && OnBoard->Press_FrontReducer <= 0) temp = 0;
                OnBoard->Press_tyre[0] = OnBoard->Press_tyre[0] + temp/10.0f;
                if(OnBoard->Press_tyre[0] >= 9.0f && OnBoard->Press_tyre[0] < 30.0f) OnBoard->Press_tyre_level[0] = 1;
                if(OnBoard->Press_tyre[0] >= 30.0f && OnBoard->Press_tyre[0] < 40.0f) OnBoard->Press_tyre_level[0] = 2;
                if(OnBoard->Press_tyre[0] >= 40.0f && OnBoard->Press_tyre[0] < 51.0f) OnBoard->Press_tyre_level[0] = 3;
            }
        break;
        case 2:
            if(TyreValves->TyreTap_2)
            {
                temp = OnBoard->Press_FrontReducer - OnBoard->Press_tyre[1];
                if(temp < 0 && OnBoard->Press_FrontReducer <= 0) temp = 0;
                OnBoard->Press_tyre[1] = OnBoard->Press_tyre[1] + temp/10.0f;
                if(OnBoard->Press_tyre[1] >= 9.0f && OnBoard->Press_tyre[1] < 30.0f) OnBoard->Press_tyre_level[1] = 1;
                if(OnBoard->Press_tyre[1] >= 30.0f && OnBoard->Press_tyre[1] < 40.0f) OnBoard->Press_tyre_level[1] = 2;
                if(OnBoard->Press_tyre[1] >= 40.0f && OnBoard->Press_tyre[1] < 51.0f) OnBoard->Press_tyre_level[1] = 3;
            }
        break;
        case 3:
            if(TyreValves->TyreTap_3)
            {
                temp = OnBoard->Press_RearReducer - OnBoard->Press_tyre[2];
                if(temp < 0 && OnBoard->Press_RearReducer <= 0) temp = 0;
                OnBoard->Press_tyre[2] = OnBoard->Press_tyre[2] + temp/10.0f;
                if(OnBoard->Press_tyre[2] >= 9.0f && OnBoard->Press_tyre[2] < 35.0f) OnBoard->Press_tyre_level[2] = 1;
                if(OnBoard->Press_tyre[2] >= 35.0f && OnBoard->Press_tyre[2] < 49.5f) OnBoard->Press_tyre_level[2] = 2;
                if(OnBoard->Press_tyre[2] >= 49.5f && OnBoard->Press_tyre[2] < 51.0f) OnBoard->Press_tyre_level[2] = 3;
            }
        break;
        case 4:
            if(TyreValves->TyreTap_4)
            {
                temp = OnBoard->Press_RearReducer - OnBoard->Press_tyre[3];
                if(temp < 0 && OnBoard->Press_RearReducer <= 0) temp = 0;
                OnBoard->Press_tyre[3] = OnBoard->Press_tyre[3] + temp/10.0f;
                if(OnBoard->Press_tyre[3] >= 9.0f && OnBoard->Press_tyre[3] < 35.0f) OnBoard->Press_tyre_level[3] = 1;
                if(OnBoard->Press_tyre[3] >= 35.0f && OnBoard->Press_tyre[3] < 49.5f) OnBoard->Press_tyre_level[3] = 2;
                if(OnBoard->Press_tyre[3] >= 49.5f && OnBoard->Press_tyre[3] < 51.0f) OnBoard->Press_tyre_level[3] = 3;
            }
        break;
    }
    if(OnBoard->Press_tyre[wheelNum-1] < 0) OnBoard->Press_tyre[wheelNum-1] = 0;
    if(OnBoard->Press_tyre[wheelNum-1] > 50.0f) OnBoard->Press_tyre[wheelNum-1] = 50.0f;
}
