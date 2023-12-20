#ifndef COBRA2_H
#define COBRA2_H

#include <QObject>
#include <QTime>
#include <windows.h>


class T_COBRA2_driver : public QObject
{
    Q_OBJECT
public:
    struct TGauges;
    TGauges *Gauges;                           // стрелочные приборы на приборном щитке
    struct TSwitches;
    TSwitches *Switches;                       // кнопки и переключатели на приборном щитке
    struct TLamps;
    TLamps *Lamps;                             // лампы на приборной доске
    struct TDoping_2;
    TDoping_2 *Doping_2;                       // система пожаротушения "Допинг-2"
    struct TFuse;
    TFuse *Fuse;                               // блок предохранителей
    struct TControls;
    TControls *Controls;                       // рычаги и педали органов управления
    struct TTyreValves;
    TTyreValves *TyreValves;                   // краны управления давлением шин
    struct TEdgeContacts;
    TEdgeContacts *EdgeContacts;               // концевые переключатели
    struct TCompositeSwitch;
    TCompositeSwitch *CompositeSwitch;         // комбинированный подрулевой переключатель
    struct TSidelights;
    TSidelights *Sidelights;                   // освещение и габаритные фонари
    struct TOnBoard;
    TOnBoard *OnBoard;                         // различные параметры бортовых систем

    explicit T_COBRA2_driver(QObject *parent = 0);
    ~T_COBRA2_driver();
    void Init();                               // инициализация параметров
    void SetShutters(float i);                 // устанавливает положение жалюзи
    void SetAirDischarge(float i);             // устанавливает давление воздуха в системе
    void SetGear(int i, bool manual);          // устанавливает текущую передачу КПП
    void SetTransferGear(int i, bool manual);  // устанавливает текущую передачу раздаточной КПП
    float SetStop(float p);                    // устанавливает силу торможения
    void Press_FrontRedu(float i);             // устанавливает давление в редукторе передней оси
    void Press_RearRedu(float i);              // устанавливает давление в редукторе задней оси
    void Tyre_Redu(BYTE wheelNum);             // устанавливает давление в шинах
};

struct T_COBRA2_driver::TGauges
{
    float Speedometer;                         // спидометр
    float Tachometer;                          // тахометр
    float SystemAirPressure;                   // манометр воздушной системы (белая стрелка)
    float BrakeSystemAirPressure;              // манометр давления воздуха в пневмоприводе тормозов (красная стрелка)
    float EngineOilPressure;                   // манометр масла двигателя
    float EngineCoolantTemperature;            // термометр ОЖ
    float FuelLevel;                           // указатель уровня топлива
    float Voltage;                             // напряжение бортовой сети
    float FrontWheelsAirPressure;              // показания прибора манометра шин передней оси (красная стрелка)
    float RearWheelsAirPressure;               // показания прибора манометра шин задней оси (белая стрелка)
};

struct T_COBRA2_driver::TSwitches
{
    bool Ignition_I;                           // замок зажигания в положении I
    bool Ignition_II;                          // замок зажигания в положении II (нефиксируемое)
    bool Ignition_III;                         // замок зажигания в положении III (блокировка руля)
    bool Alarm;                                // выключатель аварийной сигнализации
    bool LeftFuelTank;                         // переключатель баков
    bool RearFogLight;                         // выключатель задних противотуманных фонарей
    bool Diagnostics;                          // выключатель "диагностика"
    bool SMU;                                  // выключатель режима светомаскировки
    bool Sidelights;                           // габариты
    bool CentralLight;                         // центральный свет
    bool ArticulatedLorry;                     // выключатель "Автопоезд"
    bool Preheater;                            // выключатель предпускового подогревателя (ПЖД16)
    bool WheelsAirPressureMode;                // переключатель режимов давления воздуха в шинах
    bool WheelsAirPressurePower;               // выключатель системы регулирования давления воздуха в шинах
    bool Battery;                              // выключатель батарей
    bool CentralLightAdjustLevel0;             // корректор фар положение 0
    bool CentralLightAdjustLevel1;             // корректор фар положение 1
    bool CentralLightAdjustLevel2;             // корректор фар положение 2
    bool CentralLightAdjustLevel3;             // корректор фар положение 3
    bool CentralLightAdjustLevel4;             // корректор фар положение 4
};

struct T_COBRA2_driver::TCompositeSwitch
{
    bool LeftTurnSwitch;                       // переключатель поворота на лево
    bool RightTurnSwitch;                      // переключатель поворота на право
    bool WiperModeA;                           // выключатель стеклоочистителя первая скорость
    bool WiperModeB;                           // выключатель стеклоочистителя вторая скорость
    bool WiperModeC;                           // выключатель стеклоочистителя прерывистый режим
    bool Washer;                               // насос омывателя стекла
    bool DistanceLight;                        // дальний свет фар
    bool Signal;                               // звуковой сигнал
};

struct T_COBRA2_driver::TLamps
{
    bool EngineOilPressLamp;                   // сд. давление масла в двигателе
    bool LorryLamp;                            // сд. фонари автопоезда
    bool WaterLevelLamp;                       // сд. уровень ОЖ
    bool SMULamp;                              // сд. режим светомаскировки
    bool ColdStartLamp;                        // сд. холодный пуск
    bool TurnLamp;                             // сд. указатель поворота
    bool SidelightsLamp;                       // сд. габаритные огни
    bool DiagnosticsLamp;                      // сд. диагностика
    bool TrailerTurnLamp;                      // сд. указатель поворота прицепа
    bool LowFuelLamp;                          // сд. мало топлива
    bool WaterInFuelLamp;                      // сд. наличие воды в топливе
    bool AirFilterLamp;                        // сд. засореный воздушный фильтр
    bool BatteryLamp;                          // сд. батарея
    bool PreheaterLamp;                        // сд. предпусковой подогреватель (ПЖД16)
    bool EngineBrakeLamp;                      // сд. моторный тормоз
    bool InteraxleDiffLockLamp;                // сд. сигнал блокировки межосевого дифференциала
    bool CrossaxleDiffLockLamp;                // сд. сигнал блокировки межколесного дифференциала
    bool HighBeamLamp;                         // сд. дальний свет
    bool HandBrakeLamp;                        // сд. включение стояночной тормозной системы
    bool BrakeFaultLamp;                       // сд. неисправность тормозной системы
    bool WaterOverheatLamp;                    // сд. перегрев ОЖ
    bool WheelsAirPressure_Sand;               // сд. режим "песок" давления воздуха в шинах
    bool WheelsAirPressure_Terrain;            // сд. режим "грунт" давления воздуха в шинах
    bool WheelsAirPressure_Highway;            // сд. режим "шоссе" давления воздуха в шинах
    bool WheelsAirPressure_Fault;              // сд. неисправность или низкое давление в шинах
    bool DashBoardLight;                       // подсветка приборной доски
};

struct T_COBRA2_driver::TDoping_2
{
    bool Power;                                // тумблер запуска генераторов
    bool LeftGreenLamp;                        // сигнальная лампа исправности левого генератора
    bool LeftRedLamp;                          // сигнальная лампа неисправности левого генератора
    bool RightGreenLamp;                       // сигнальная лампа исправности правого генератора
    bool RightRedLamp;                         // сигнальная лампа неисправности правого генератора
};

struct T_COBRA2_driver::TFuse
{
    bool ExternalLights_Ready;                 // габариты
    bool SignalLamp_Ready;                     // сигнальные лампы
    bool HeadLamp_Ready;                       // фары
};

struct T_COBRA2_driver::TEdgeContacts
{
    bool FuelTapLeftTank;                      // топливный кран левого бака
    bool FuelTapRightTank;                     // топливный кран правого бака
};

struct T_COBRA2_driver::TControls
{
    bool EngineBrake;                          // выключатель моторного тормоза
    float Brake;                               // тормоз в %
    float Gas;                                 // газ в %
    float Coupling;                            // сцепление в %
    float Steering;                            // руль в %
    bool Gear_1;                               // КПП 1 передача
    bool Gear_2;                               // КПП 2 передача
    bool Gear_3;                               // КПП 3 передача
    bool Gear_4;                               // КПП 4 передача
    bool Gear_5;                               // КПП 5 передача
    bool Gear_R;                               // КПП задняя передача
    bool TransferGear_Low;                     // раздат. коробка - пониженная
    bool TransferGear_High;                    // раздат. коробка - повышенная
    bool DiffLock;                             // блокировка межосевого дифференциала (сухая дорога/скользкая дорога)
    bool WheelsLock;                           // блокировка межколесного дифференциала
    bool HandBrake;                            // ручной тормоз
};

struct T_COBRA2_driver::TTyreValves
{
    bool FrontAirTapDischarge;                 // выпуск воздуха из шин передняя ось
    bool FrontAirTapPump;                      // накачка шин передняя ось
    bool RearAirTapDischarge;                  // выпуск воздуха из шин задняя ось
    bool RearAirTapPump;                       // накачка шин задняя ось
    bool TyreTap_1;                            // шинный кран 1 колеса
    bool TyreTap_2;                            // шинный кран 2 колеса
    bool TyreTap_3;                            // шинный кран 3 колеса
    bool TyreTap_4;                            // шинный кран 4 колеса
};

struct T_COBRA2_driver::TSidelights
{
    bool Sidelights;                           // фонари габаритов
    bool TurnLeftLights;                       // фонари сигнала левого поворота
    bool TurnRightLights;                      // фонари сигнала правого поворота
    bool StopLights;                           // фонари стоп сигнала
    bool ReverseLight;                         // фонарь заднего хода
    bool CentralLights;                        // фары (ближний свет)
    bool DistanceLight;                        // фары (дальний свет)
    bool RearFogLight;                         // задние противотуманные фонари
    bool LorryLight;                           // фонарь "Автопоезд"
};

struct T_COBRA2_driver::TOnBoard
{
    float Time;                                // время существования в секундах
    bool EngineStarted;                        // двигатель запущен
    bool SteeringBlock;                        // блокировка руля
    bool TransmissionBlock;                    // блокировка КПП
    bool Direction;                            // направление движения (true - вперед, false - назад)
    float F_Stop;                              // сила торможения
    float Omega;                               // обороты двигателя
    float brakeTorque[4];                      // компенсирующий момент тормоза
    float WheelsTorque[4];                     // крутящий момент на колесах в Н.м.
    int Gear;                                  // передача
    int TransferGear;                          // -1 пониженная, 0 нейтральная, 1 повышенная передача
    BYTE Starter;                              // стартер 0-не работает, 1-от АКБ, 2-от воздуха, 3-от обеих
    float Starter_f;                           // сила стартера
    bool BatteryActive;                        // батареи подключены
    float V_akb;                               // заряд аккумуляторных батарей
    float I_board;                             // ток бортовой сети
    float V_board;                             // напряжение бортовой сети
    float Pmdv;                                // давление масла в двигателе
    float Pmkp;                                // давление масла в РК
    float Pvoz;                                // давление воздуха в системе
    bool Generator;                            // генератор
    bool Compressor;                           // работает компрессор
    float Shutters;                            // положение жалюзи 0..1
    bool EngineVentilation;                    // принудительная вентиляция двигателя false - выкл., true - вкл.
    float F_HandBrake;                         // сила стояночного тормоза
    float BrakeSystemAirPressure;              // манометр давления воздуха в пневмоприводе тормозов
    BYTE DriverWiper_mode;                     // стеклоочиститель 0-выключен, 1-первая скорость, 2-вторая скорость, 3-прерывистый режим
    float T_Motor;                             // температура двигателя
    float T_Oil;                               // температура масла
    float T_OG;                                // температура охлождающей жидкости
    float T_Svecha;                            // температура свечи подогревателя
    float LeftFuelTankLevel;                   // уровень топлива левого бака
    float RightFuelTankLevel;                  // уровень топлива правого бака
    float FuelLevel;                           // уровень топлива на приборе
    float FrontWheelsAirPressure;              // показания прибора манометра шин передней оси
    float RearWheelsAirPressure;               // показания прибора манометра шин задней оси
    float Press_FrontReducer;                  // давление в редукторе передней оси в (Па)
    float Press_RearReducer;                   // давление в редукторе задней оси в (Па)
    float Press_tyre[4];                       // давление в шинах в кПа (колеса нумеруются от правого заднего до левого заднего против часовой стрелки)
    BYTE Press_tyre_level[4];                  // уровень давление в шинах (0 - спущено, 1 - песок, 2 - грунт, 3 - шоссе)
    bool StopEngine;                           // моментальная остановка двигателя (МОД)
    bool Signal;                               // звуковой сигнал
    bool LeftTurnSwitch;                       // переключатель поворота на лево
    bool RightTurnSwitch;                      // переключатель поворота на право
    bool SMU;                                  // режим светомаскировки
    bool Preheater;                            // предпусковой подогреватель (ПЖД16)
    bool RearFogLight;                         // выключатель задних противотуманных фонарей
    int FrontManualWheelPump;                  // режим ручной подкачки шин передней оси
    int RearManualWheelPump;                   // режим ручной подкачки шин задней оси
    BYTE WheelsAirPressureMode;                // режим давления воздуха в шинах
    bool WheelsAirPressurePower;               // система регулирования давления воздуха в шинах
    bool EngineBrake;                          // моторный тормоз
    BYTE CentralLight;                         // положение переключателя света
    BYTE CentralLightAdjustLevel;              // положение корректора фар
    //другие параметры, для обмена с другими модулями//
    float T_init;                              // температура окружающего воздуха (задается инструктором)
    float K_T_og_v;                            // коэффициент теплообмена охлаждяющая жидкость - воздух
    QTime SignalTime;                          // время подачи звукового сигнала
    bool SignalOnEngineStarted;                // звуковой сигнал перед пуском двигателя
    bool SignalOnMove;                         // звуковой сигнал перед началом движения
    float Time_Start;                          // время непрерывного нажатия на кнопку стартера
    float Time_Battery;                        // время непрерывного нажатия на кнопку БАТАРЕИ
    //float Time_Preheater;                      // время непрерывного нажатия на кнопку подогревателя ПЖД16
    //float Time_SMU;                            // время непрерывного нажатия на кнопку режима светомаскировки
    //float Time_EngineBrake;                    // время непрерывного нажатия на кнопку моторного тормоза
    //float Time_WheelsPressurePower;            // время непрерывного нажатия на кнопку включения системы регулирования давления воздуха в шинах
    float Time_WheelsPressureMode;             // время непрерывного нажатия на кнопку режима системы регулирования давления воздуха в шинах
    float Time_RearFogLight;                   // время непрерывного нажатия на кнопку выключателя задних противотуманных фонарей
    float Time_Ignition_I;                     // время непрерывного состояния замка зажигания в положении I (в миллисекундах)
    bool Collision;                            // Столкновение
};



#endif // COBRA2_H
