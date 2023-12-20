#include "COBRA2_imitator_tcp_client.h"


T_COBRA2_TCP_Client_ControlsImitator::T_COBRA2_TCP_Client_ControlsImitator(T_COBRA2_ControlsImitator *ControlsImitator, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, TSettingsFile *ini, TLogging *log, TReplay *replay)
{
    _ControlsImitator = ControlsImitator;
    _Instructor = Instructor;
    _driver = driver;
    _ini = ini;
    _log = log;
    _replay = replay;

    TxInterval = _ini->LanTxInterval();
    hostaddr = _ini->IP_ControlsImitator();
    tcp_port = _ini->PORT_ControlsImitator();

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

T_COBRA2_TCP_Client_ControlsImitator::~T_COBRA2_TCP_Client_ControlsImitator()
{
    TimerTCPSend->stop();
}

void T_COBRA2_TCP_Client_ControlsImitator::slotReadTcpData()
{
    //ReadCommonTcpData();
}

void T_COBRA2_TCP_Client_ControlsImitator::slotSendTcpData()
{
    SendControlsDataPacket();
}

void T_COBRA2_TCP_Client_ControlsImitator::SendControlsDataPacket()
{
    float EngineOilPressure = _driver->Gauges->EngineOilPressure/10.0f;
    float Tachometer = _driver->Gauges->Tachometer/100.0f;
    float Speedometer = _driver->Gauges->Speedometer;
    float FuelLevel = _driver->Gauges->FuelLevel/100.0f;
    float EngineCoolantTemperature = _driver->Gauges->EngineCoolantTemperature;
    float Voltage = _driver->Gauges->Voltage;
    float SystemAirPressure = _driver->Gauges->SystemAirPressure/100.0f;
    float BrakeSystemAirPressure = _driver->Gauges->BrakeSystemAirPressure/100.0f;
    float RearWheelsAirPressure = _driver->Gauges->RearWheelsAirPressure/10.0f;
    float FrontWheelsAirPressure = _driver->Gauges->FrontWheelsAirPressure/10.0f;

    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<bool>(_driver->Lamps->LorryLamp);                 // Автопоезд
        data_stream << static_cast<bool>(_driver->Lamps->WaterLevelLamp);            // Уровень ОЖ
        data_stream << static_cast<bool>(_driver->Lamps->SMULamp);                   // Светомаскировка
        data_stream << static_cast<bool>(_driver->Lamps->EngineOilPressLamp);        // Давление масла
        data_stream << static_cast<bool>(_driver->Lamps->TurnLamp);                  // Поворотники
        data_stream << static_cast<bool>(_driver->Lamps->SidelightsLamp);            // Габаритные огни
        data_stream << static_cast<bool>(_driver->Lamps->TrailerTurnLamp);           // Поворотники прицепа
        data_stream << static_cast<bool>(_driver->Lamps->DiagnosticsLamp);           // Диагностика
        data_stream << static_cast<bool>(_driver->Lamps->ColdStartLamp);             // Холодный пуск
        data_stream << static_cast<bool>(_driver->Lamps->BrakeFaultLamp);            // Неисправность тормозов
        data_stream << static_cast<bool>(_driver->Lamps->HandBrakeLamp);             // Ручной тормоз
        data_stream << static_cast<bool>(_driver->Lamps->HighBeamLamp);              // Дальний свет
        data_stream << static_cast<bool>(_driver->Lamps->WaterInFuelLamp);           // Вода в топливе
        data_stream << static_cast<bool>(_driver->Lamps->AirFilterLamp);             // Засорен воздушный фильтр
        data_stream << static_cast<bool>(_driver->Lamps->BatteryLamp);               // Отсутствует зарядка батареи
        data_stream << static_cast<bool>(_driver->Lamps->LowFuelLamp);               // Низкий уровень топлива
        data_stream << static_cast<qint8>(_driver->OnBoard->CentralLight);           // Центральный переключатель света
        data_stream << static_cast<bool>(_driver->Switches->SMU);                    // Светомаскировка
        data_stream << static_cast<bool>(_driver->Switches->Alarm);                  // Аварийная сигнализация
        data_stream << static_cast<bool>(_driver->Switches->LeftFuelTank);           // Переключатель баков
        data_stream << static_cast<float>(EngineOilPressure);                        // Давление масла двигателя
        data_stream << static_cast<float>(Tachometer);                               // Тахометр
        data_stream << static_cast<float>(Speedometer);                              // Спидометр
        data_stream << static_cast<qint32>(0);                                       // Одометр
        data_stream << static_cast<float>(FuelLevel);                                // Уровень топлива
        data_stream << static_cast<float>(EngineCoolantTemperature);                 // Температура ОЖ
        data_stream << static_cast<float>(Voltage);                                  // Напряжение батареи
        data_stream << static_cast<bool>(_driver->Lamps->PreheaterLamp);             // Предпусковой подогреватель
        data_stream << static_cast<bool>(_driver->Switches->Preheater);              // Выключатель предпускового подогревателя
        data_stream << static_cast<bool>(_driver->Switches->ArticulatedLorry);       // Выключатель знака автопоезда
        data_stream << static_cast<bool>(_driver->Lamps->WheelsAirPressure_Sand);    // Режим "песок"
        data_stream << static_cast<bool>(_driver->Lamps->WheelsAirPressure_Terrain); // Режим "грунт"
        data_stream << static_cast<bool>(_driver->Lamps->WheelsAirPressure_Highway); // Режим "шоссе"
        data_stream << static_cast<bool>(_driver->Lamps->WheelsAirPressure_Fault);   // Неисправность системы подкачки шин
        data_stream << static_cast<bool>(_driver->Switches->WheelsAirPressurePower); // Выключатель системы подкачки шин
        data_stream << static_cast<bool>(_driver->Switches->WheelsAirPressureMode);  // Режим системы подкачки шин
        data_stream << static_cast<bool>(_driver->OnBoard->BatteryActive);           // Масса
        data_stream << static_cast<bool>(_driver->Lamps->EngineBrakeLamp);           // Моторный тормоз
        data_stream << static_cast<bool>(_driver->Lamps->InteraxleDiffLockLamp);     // Межосевая блокировка
        data_stream << static_cast<bool>(_driver->Lamps->CrossaxleDiffLockLamp);     // Межколесная блокировка
        data_stream << static_cast<qint8>(_driver->OnBoard->FrontManualWheelPump);   // Ручная подкачка шин передней оси
        data_stream << static_cast<qint8>(_driver->OnBoard->RearManualWheelPump);    // Ручная подкачка шин задней оси
        data_stream << static_cast<bool>(_driver->Controls->EngineBrake);            // Выключатель моторного тормоза
        data_stream << static_cast<bool>(_driver->Controls->DiffLock);               // Выключатель межосевой блокировки
        data_stream << static_cast<bool>(_driver->Controls->WheelsLock);             // Выключатель межколесной блокировки
        data_stream << static_cast<float>(SystemAirPressure);                        // Давление воздуха в системе
        data_stream << static_cast<float>(BrakeSystemAirPressure);                   // Давление воздуха в тормозной системе
        data_stream << static_cast<float>(RearWheelsAirPressure);                    // Давление воздуха в шинах задней оси
        data_stream << static_cast<float>(FrontWheelsAirPressure);                   // Давление воздуха в шинах передней оси
        data_stream << static_cast<bool>(_driver->Switches->Diagnostics);            // Кнопка диагностики
        data_stream << static_cast<bool>(_driver->Switches->RearFogLight);           // Задние противотуманные фонари
        data_stream << static_cast<qint8>(_driver->OnBoard->CentralLightAdjustLevel);// Переключатель корректора фар
        data_stream << static_cast<bool>(_driver->Switches->Ignition_I);             // Замок зажигания положение I
        data_stream << static_cast<bool>(_driver->Switches->Ignition_II);            // Замок зажигания положение II
        data_stream << static_cast<float>(_driver->Controls->Steering);              // Руль
        data_stream << static_cast<float>(_driver->Controls->Coupling);              // Сцепление
        data_stream << static_cast<float>(_driver->Controls->Brake);                 // Тормоз
        data_stream << static_cast<float>(_driver->Controls->Gas);                   // Газ
        data_stream << static_cast<bool>(_driver->Controls->HandBrake);              // Ручник
        data_stream << static_cast<qint8>(_driver->OnBoard->Gear);                   // Передачи КПП
        data_stream << static_cast<qint8>(_driver->OnBoard->TransferGear);           // Передачи раздаточной коробки
        data_stream << static_cast<bool>(_driver->CompositeSwitch->LeftTurnSwitch);  // Левый поворотник
        data_stream << static_cast<bool>(_driver->CompositeSwitch->RightTurnSwitch); // Правый поворотник
        data_stream << static_cast<bool>(_driver->CompositeSwitch->Signal);          // Сигнал
        data_stream << static_cast<bool>(_driver->CompositeSwitch->DistanceLight);   // Дальний свет
        data_stream << static_cast<qint8>(_driver->OnBoard->DriverWiper_mode);       // Режим дворников
        data_stream << static_cast<bool>(_driver->CompositeSwitch->Washer);          // Омыватель
        data_stream << static_cast<bool>(_driver->Doping_2->LeftGreenLamp);          // Левый генератор системы пожаротушения исправен
        data_stream << static_cast<bool>(_driver->Doping_2->RightGreenLamp);         // Правый генератор системы пожаротушения исправен
        data_stream << static_cast<bool>(_driver->Doping_2->LeftRedLamp);            // Левый генератор системы пожаротушения неисправен
        data_stream << static_cast<bool>(_driver->Doping_2->RightRedLamp);           // Правый генератор системы пожаротушения неисправен
        data_stream << static_cast<bool>(_driver->Doping_2->Power);                  // Включение системы пожаротушения
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('O');
        header_stream << static_cast<quint32>(ControlsImitatorTCPPacketTypes::SR2CI_DriverControlsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ControlsImitatorPacket,quint32(ControlsImitatorTCPPacketTypes::SR2CI_DriverControlsDataPacket),packet);
    }
}
