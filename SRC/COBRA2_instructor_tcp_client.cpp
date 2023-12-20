#include "COBRA2_instructor_tcp_client.h"


T_COBRA2_TCP_Client_Instructor::T_COBRA2_TCP_Client_Instructor(T_COBRA2_Server *Server, T_COBRA2_Imager *Imager, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, TSettingsFile *ini, TLogging *log, TReplay *replay)
{
    _Server = Server;
    _Imager = Imager;
    _Instructor = Instructor;
    _driver = driver;
    _ini = ini;
    _log = log;
    _replay = replay;

    TxInterval = _ini->LanTxInterval();
    hostaddr = _ini->IP_Instructor();
    tcp_port = _ini->PORT_Instructor();

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

T_COBRA2_TCP_Client_Instructor::~T_COBRA2_TCP_Client_Instructor()
{
    TimerTCPSend->stop();
}

void T_COBRA2_TCP_Client_Instructor::slotReadTcpData()
{
    ReadCommonTcpData();
}

void T_COBRA2_TCP_Client_Instructor::slotSendTcpData()
{
    SendCommonTcpData();
    SendComponentsStatePacket();
    SendDriverStateDataPacket();
}

void T_COBRA2_TCP_Client_Instructor::SendDriverStateDataPacket()
{
    if(!_Imager->ImagerState->PacketReady) _Imager->ImagerStateDequeuePacket();
    if(!_Imager->ImagerState->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<float>(_Imager->ImagerState->Course);                 // курс
        data_stream << static_cast<float>(_Imager->ImagerState->Speed);                  // скорость
        data_stream << static_cast<float>(_Imager->ImagerState->X);                      // Координата техники X (от угла карты в метрах)
        data_stream << static_cast<float>(_Imager->ImagerState->Y);                      // Координата техники Y (от угла карты в метрах)
        data_stream << static_cast<bool>(_driver->OnBoard->EngineStarted);               // запущен ли двигатель
        data_stream << static_cast<qint8>(_driver->OnBoard->Gear);                       // передача КПП
        data_stream << static_cast<float>(_driver->OnBoard->Omega);                      // обороты двигателя
        data_stream << static_cast<bool>(_driver->OnBoard->Signal);                      // звуковой сигнал
        data_stream << static_cast<bool>(_driver->Controls->Gas);                        // активность педали газа
        data_stream << static_cast<bool>(_driver->Controls->HandBrake);                  // ручной тормоз
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_DriverStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Imager->ImagerState->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::InstructorPacket,quint32(InstructorTCPPacketTypes::SR2IR_DriverStateDataPacket),packet);
    }
}

void T_COBRA2_TCP_Client_Instructor::SendComponentsStatePacket()
{
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 StaticStructCount = 5;
        quint8 ImagerOfflinePacketsCount = 50;

        QList<Elements> VisualElementsList;
        VisualElementsList.clear();
        if(!_Imager->ImagerElementsState->PacketReady) _Imager->ImagerElementsStateDequeuePacket();
        if(_Imager->ImagerElementsState->PacketReady)
        {
            VisualElementsList = _Imager->ImagerElementsState->ElementsStateList;
            _Imager->ImagerElementsState->OfflineCounter = ImagerOfflinePacketsCount;
            _Imager->ImagerElementsState->PacketReady = false;
        }
        else
        {
            if(_Imager->ImagerElementsState->OfflineCounter > 0)
            {
                VisualElementsList = _Imager->ImagerElementsState->ElementsStateList;
                _Imager->ImagerElementsState->OfflineCounter--;
            }
        }
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Server->clockGenerator);                         // Время прошедшее с запуска сервера (в милисекундах)
        data_stream << static_cast<qint32>(StaticStructCount + VisualElementsList.count());  // Число структур теста функционирования
        data_stream << static_cast<quint8>(ElementsTypes::Server);                           // 1
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Server");
        data_stream << QString("");
        data_stream << static_cast<quint8>(ElementsTypes::CAN);                              // 2
        //data_stream << static_cast<quint8>(_Server->CANBusState);
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Can");
        data_stream << QString("");
        data_stream << static_cast<quint8>(ElementsTypes::Resident);                         // 3
        //data_stream << static_cast<quint8>(_Server->ResidentDriverState);
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Resident_Driver");
        data_stream << QString("");
        data_stream << static_cast<quint8>(ElementsTypes::MotionSystem);                     // 4
        //data_stream << static_cast<quint8>(_Server->MotionSystemDriverState);
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Platform_Driver");
        data_stream << QString("");
        data_stream << static_cast<quint8>(ElementsTypes::OU);                               // 5
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Visual_Controls");
        data_stream << QString("");
        //Fake visual server Good state
//        data_stream << static_cast<quint8>(ElementsTypes::VisualServer);
//        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
//        data_stream << QString("BTR-80_Instructor_Rep_Server");
//        data_stream << QString("");
        for(int i=0; i<VisualElementsList.count(); i++)
        {
            data_stream << static_cast<quint8>(VisualElementsList[i].ElementType);
            data_stream << static_cast<quint8>(VisualElementsList[i].ElementState);
            data_stream << QString(VisualElementsList[i].ElementName);
            data_stream << QString(VisualElementsList[i].Comment);
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_ComponentsStatePacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::InstructorPacket,quint32(InstructorTCPPacketTypes::SR2IR_ComponentsStatePacket),packet);
    }
}
