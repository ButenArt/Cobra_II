#include "COBRA2_udp_ou.h"

TIGER_UDP_OU::TIGER_UDP_OU(T_TIGER_Instructor *Instructor, T_TIGER_driver *driver, TSettingsFile *ini, TReplay *replay)
{
    _Instructor = Instructor;
    _driver = driver;
    _ini = ini;
    _replay = replay;
    TxInterval = _ini->LanTxInterval();
    IP_Broadcast = _ini->IP_OU_Broadcast();
    REMOTE_PORT_OU_MV_UPR = _ini->PORT_OU_MV_UPR();
    REMOTE_PORT_OU_MV_IND = _ini->PORT_OU_MV_IND();

    UDP2OU_SendTimer->setInterval(TxInterval);
    connect(UDP2OU_SendTimer,SIGNAL(timeout()),this, SLOT(slotUDPSend()));
    UDP2OU_SendTimer->start();
}

TIGER_UDP_OU::~TIGER_UDP_OU()
{
    UDP2OU_SendTimer->stop();
}

void TIGER_UDP_OU::slotUDPSend()
{
    OU_MV();
}

void TIGER_UDP_OU::OU_MV()
{
    // корректировка показаний приборов
    quint16 Speedometer = 0;
    quint16 Tachometer = 0;
    quint8 SystemAirPressure = _driver->Gauges->SystemAirPressure/10.0f;
    quint8 FrontWheelsAirPressure = _driver->Gauges->FrontWheelsAirPressure;
    quint8 EngineOilPressure = 0;
    quint8 EngineCoolantTemperature = 0;
    quint8 FuelLevel = 0;
    quint8 Voltage = _driver->Gauges->Voltage*3;
    if(_driver->OnBoard->BatteryActive)
    {
        Speedometer = _driver->Gauges->Speedometer*100;
        Tachometer = _driver->Gauges->Tachometer;
        EngineOilPressure = _driver->Gauges->EngineOilPressure;
        EngineCoolantTemperature = _driver->Gauges->EngineCoolantTemperature-20;
        FuelLevel = _driver->Gauges->FuelLevel;
    }

    BYTE IND_MatrixSize = 14;
    BYTE UPR_MatrixSize = 9+8;
    BYTE tmp = 0x00;
    QByteArray IND_data(IND_MatrixSize,0x00);
    QDataStream IND_data_stream(&IND_data, QIODevice::WriteOnly);
    IND_data_stream.setByteOrder(QDataStream::LittleEndian);
    IND_data_stream << quint16(Tachometer);                           // тахометр
    IND_data_stream << quint16(Speedometer);                          // спидометр
    IND_data_stream << quint8(EngineOilPressure);                     // манометр масла двигателя
    IND_data_stream << quint8(EngineCoolantTemperature);              // термометр ОЖ
    IND_data_stream << quint8(FrontWheelsAirPressure);                // манометра шин передняя ось (белая стрелка)
    IND_data_stream << quint8(SystemAirPressure);                     // манометр воздушной системы (белая стрелка)
    IND_data_stream << quint8(Voltage);                               // напряжение АКБ
    IND_data_stream << quint8(FuelLevel);                             // указатель уровня топлива
    tmp = 0x00;
    IND_data_stream << quint8(tmp); // 10
    tmp = SetByte(false,false,false,false,false,false,false,false);
    IND_data_stream << quint8(tmp); // 11
    tmp = SetByte(false,false,false,false,false,false,false,false);
    IND_data_stream << quint8(tmp); // 12
    tmp = SetByte(false,false,false,false,false,false,false,false);
    IND_data_stream << quint8(tmp); // 13
    QByteArray IND_header;
    QDataStream IND_header_stream(&IND_header, QIODevice::WriteOnly);
    IND_header_stream.setByteOrder(QDataStream::LittleEndian);
    IND_header_stream << quint8(34);
    IND_header_stream << quint8(17);
    IND_header_stream << quint8(80);
    IND_header_stream << quint8(0);
    IND_header_stream << quint8(IND_MatrixSize);
    IND_header_stream << quint8(0);
    QByteArray IND_packet;
    IND_packet.append(IND_header);
    IND_packet.append(IND_data);
    sendUDP(IND_packet, QHostAddress(IP_Broadcast), REMOTE_PORT_OU_MV_IND);
    if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::OU_IND_MV_Packet,0,IND_packet);

    QByteArray UPR_data(UPR_MatrixSize,0x00);
    QDataStream UPR_data_stream(&UPR_data, QIODevice::WriteOnly);
    UPR_data_stream.setByteOrder(QDataStream::LittleEndian);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(0x00);
    UPR_data_stream << quint8(_driver->Controls->Brake*2.5);            // 0
    UPR_data_stream << quint8(_driver->Controls->Gas*2.5);              // 1
    UPR_data_stream << quint8(_driver->Controls->Coupling*2.5);         // 2
    UPR_data_stream << quint16(_driver->Controls->Steering*2.5);        // 3-4
    UPR_data_stream << quint8(0x00); // 5
    tmp = SetByte(false,false,false,false,false,false,false,false);
    UPR_data_stream << quint8(tmp);  // 6
    tmp = SetByte(false,false,false,false,false,false,false,false);
    UPR_data_stream << quint8(tmp);  // 7
    tmp = SetByte(false,false,false,false,false,false,false,false);
    UPR_data_stream << quint8(tmp);  // 8

    QByteArray UPR_header;
    QDataStream UPR_header_stream(&UPR_header, QIODevice::WriteOnly);
    UPR_header_stream.setByteOrder(QDataStream::LittleEndian);
    UPR_header_stream << quint8(34);
    UPR_header_stream << quint8(17);
    UPR_header_stream << quint8(80);
    UPR_header_stream << quint8(UPR_MatrixSize);
    UPR_header_stream << quint8(0);
    UPR_header_stream << quint8(0);
    QByteArray UPR_packet;
    UPR_packet.append(UPR_header);
    UPR_packet.append(UPR_data);
    sendUDP(UPR_packet, QHostAddress(IP_Broadcast), REMOTE_PORT_OU_MV_UPR);
    if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::OU_UPR_MV_Packet,0,UPR_packet);
}
