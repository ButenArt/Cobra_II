#include "COBRA2_imager_tcp_client.h"


T_COBRA2_TCP_Client_Imager::T_COBRA2_TCP_Client_Imager(T_COBRA2_Server *Server, T_COBRA2_MotionSystem *MotionSystem, T_COBRA2_Imager *Imager, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, TSettingsFile *ini, TLogging *log, TReplay *replay)
{
    _Server = Server;
    _MotionSystem = MotionSystem;
    _Imager = Imager;
    _Instructor = Instructor;
    _driver = driver;
    _ini = ini;
    _log = log;
    _replay = replay;

    TxInterval = _ini->LanTxInterval();
    hostaddr = _ini->IP_Imager();
    tcp_port = _ini->PORT_Imager();

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

T_COBRA2_TCP_Client_Imager::~T_COBRA2_TCP_Client_Imager()
{
    TimerTCPSend->stop();
}

void T_COBRA2_TCP_Client_Imager::slotReadTcpData()
{
    ReadCommonTcpData();
}

void T_COBRA2_TCP_Client_Imager::slotSendTcpData()
{
    SendCommonTcpData();
    SendDriverStateDataPacket();
}

void T_COBRA2_TCP_Client_Imager::SendDriverStateDataPacket()
{
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<bool>(_driver->OnBoard->EngineStarted);             // запущен ли двигатель
        data_stream << static_cast<float>(_driver->OnBoard->Omega);                    // обороты двигателя
        data_stream << static_cast<quint8>(_driver->OnBoard->Starter);                 // стартер работает
        data_stream << static_cast<bool>(_driver->Controls->HandBrake);                // ручной тормоз
        data_stream << static_cast<bool>(_driver->Controls->EngineBrake);              // выключатель моторного тормоза
        data_stream << static_cast<float>(_driver->Controls->Brake);                   // педаль тормоза
        data_stream << static_cast<float>(_driver->Controls->Gas);                     // педаль газа
        data_stream << static_cast<float>(_driver->Controls->Coupling);                // педаль сцепления
        data_stream << static_cast<float>(_driver->Controls->Steering);                // руль
        data_stream << static_cast<qint32>(_driver->OnBoard->TransferGear);            // раздатка [-1 пониженная, 0 нейтральная, 1 повышенная передача]
        data_stream << static_cast<bool>(_driver->Controls->DiffLock);                 // блокировка межосевого дифференциала (сухая дорога/скользкая дорога)
        data_stream << static_cast<qint32>(_driver->OnBoard->Gear);                    // передача КПП [-1 .. 5]
        data_stream << static_cast<bool>(_driver->Controls->WheelsLock);               // блокировка межколесного дифференциала
        data_stream << static_cast<bool>(_driver->OnBoard->Signal);                    // звуковой сигнал
        data_stream << static_cast<bool>(_driver->Sidelights->Sidelights);             // фонари габаритов
        data_stream << static_cast<bool>(_driver->Sidelights->TurnLeftLights);         // фонари сигнала левого поворота
        data_stream << static_cast<bool>(_driver->Sidelights->TurnRightLights);        // фонари сигнала правого поворота
        data_stream << static_cast<bool>(_driver->OnBoard->LeftTurnSwitch);            // тумблер сигнала левого поворота
        data_stream << static_cast<bool>(_driver->OnBoard->RightTurnSwitch);           // тумблер сигнала правого поворота
        data_stream << static_cast<bool>(_driver->Sidelights->StopLights);             // фонари стоп сигнала
        data_stream << static_cast<bool>(_driver->Sidelights->CentralLights);          // фары (ближний свет)
        data_stream << static_cast<bool>(_driver->Sidelights->DistanceLight);          // фары (дальний свет)
        data_stream << static_cast<quint8>(_driver->OnBoard->CentralLightAdjustLevel); // корректор фар
        data_stream << static_cast<bool>(_driver->Sidelights->RearFogLight);           // задние противотуманные фонари
        data_stream << static_cast<bool>(_driver->Sidelights->LorryLight);             // фонарь "Автопоезд"
        data_stream << static_cast<quint8>(_driver->OnBoard->DriverWiper_mode);        // режим дворников механика-водителя
        data_stream << static_cast<bool>(_driver->CompositeSwitch->Washer);            // насос омывателя
        data_stream << static_cast<bool>(_driver->OnBoard->BatteryActive);             // масса (батарея)
        data_stream << static_cast<bool>(_driver->Sidelights->ReverseLight);           // фонарь заднего хода
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_DriverStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_DriverStateDataPacket),packet);
    }
}
