#include "Instructor_TCP_client.h"


T_TCP_Client_Instructor::T_TCP_Client_Instructor()
{

}

T_TCP_Client_Instructor::~T_TCP_Client_Instructor()
{

}

void T_TCP_Client_Instructor::slotConnected()
{
    _Instructor->InstructorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Instructor_connected,"");
}

void T_TCP_Client_Instructor::slotDisconnected()
{
    _Instructor->InstructorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Instructor_disconnected,"");
}

void T_TCP_Client_Instructor::slotError()
{
    _Instructor->InstructorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Instructor_Error,socketErrorString);
}

void T_TCP_Client_Instructor::ReadCommonTcpData()
{
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_Instructor->PacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _Instructor->PacketHeader.Mark;
                header_stream >> _Instructor->PacketHeader.PacketType;
                header_stream >> _Instructor->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _Instructor->PacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_Instructor->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_Instructor->PacketHeader.PacketType)
                {
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ExerciseParametersPacket):
                        GetExerciseParametersPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ObjectsDataPacket):
                        GetObjectsDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_TargetsDataPacket):
                        GetTargetsDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_EnvironmentDataPacket):
                        GetEnvironmentDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_AmmunitionDataPacket):
                        GetAmmunitionDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ControlDataPacket):
                        GetControlDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_FailuresDataPacket):
                        GetFailuresDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_CommandsDataPacket):
                        GetCommandsDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_MessagesDataPacket):
                        GetMessagesDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ScoreRequestDataPacket ):
                        GetScoreRequestDataPacket(data_stream);
                    break;
                    case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_StatisticsDataPacket):
                        GetStatisticsDataPacket(data_stream);
                    break;
                    default:
                        qDebug( ) << "Неизвестный пакет от Instructor" << _Instructor->PacketHeader.PacketType;
                        _log->WriteLANLog(LogEvents::TCP_Instructor_Unknown_PacketType,QString::number(_Instructor->PacketHeader.PacketType));
                    break;
                }
                processedHeader = false;
            }
            if((processedHeader) && (availabeBytes >= _Instructor->PacketHeader.DataSize))
                break;
            availabeBytes = TcpSocket->bytesAvailable();
        }
    }
}

void T_TCP_Client_Instructor::GetExerciseParametersPacket(QDataStream &_stream)
{

    ExerciseParametersPacket ExerciseParameters;
    qint32 uuidCount = 0;
    quint8 uuidDetail = 0;

    ExerciseParameters.PacketReady = true;
    _stream >> ExerciseParameters.ReStart;
    _stream >> ExerciseParameters.Pause;
    _stream >> ExerciseParameters.Stop;
    _stream >> ExerciseParameters.ExerciseNum;
    _stream >> ExerciseParameters.MapType;
    _stream >> ExerciseParameters.night;
    _stream >> ExerciseParameters.season;
    _stream >> ExerciseParameters.Joystick;
    _stream >> ExerciseParameters.AutoloadingSystem;
    _stream >> ExerciseParameters.uuidSize;
    uuidCount = ExerciseParameters.uuidSize;
    while(uuidCount > 0)
    {
        _stream >> uuidDetail;
        ExerciseParameters.Uuid.append(uuidDetail);
        uuidCount--;
    }
    _stream >> ExerciseParameters.directionsNumber;
    _stream >> ExerciseParameters.backfire;
    _stream >> ExerciseParameters.selectedKit;
    _stream >> ExerciseParameters.selectedMode;
    _stream >> ExerciseParameters.distance2screen;
    _stream >> ExerciseParameters.guid_run;
    _stream >> ExerciseParameters.lang;
    qDebug( ) << "--<<" __FUNCTION__;
    //qDebug() << ExerciseParameters.distance2screen << ExerciseParameters.guid_run << ExerciseParameters.lang;
    _Instructor->ExerciseParametersEnqueuePacket(ExerciseParameters);

    if(ExerciseParameters.ReStart) _Instructor->ExerciseReStart();
    if(ExerciseParameters.Stop)
    {
        _Instructor->ExerciseStop();
        _Instructor->UnloadAmmo();
    }
}

void T_TCP_Client_Instructor::GetObjectsDataPacket(QDataStream &_stream)
{
    ObjectsDataPacket Objects;
    qint32 mapObjCount = 0;

    Objects.PacketReady = true;
    _stream >> Objects.mapObjCount;
    Objects.mapObjList.clear();

    mapObjCount = Objects.mapObjCount;
    while(mapObjCount > 0)
    {
        MapObjects mapObj;
        _stream >> mapObj.objUID;
        _stream >> mapObj.type;
        _stream >> mapObj.subType;
        _stream >> mapObj.azimuth;
        _stream >> mapObj.latitude;
        _stream >> mapObj.longitude;
        Objects.mapObjList << mapObj;
        mapObjCount--;
    }

    _Instructor->ObjectsEnqueuePacket(Objects);
}

void T_TCP_Client_Instructor::GetTargetsDataPacket(QDataStream &_stream)
{
    TargetsDataPacket Targets;
    qint32 mapTargetsCount = 0;

    Targets.PacketReady = true;
    _stream >> Targets.mapTargetsCount;
    Targets.mapTargetsList.clear();

    mapTargetsCount = Targets.mapTargetsCount;
    while(mapTargetsCount > 0)
    {
        MapTargets mapTargets;
        mapTargets.targetsShowList.clear();
        _stream >> mapTargets.ObjectiveUID;
        _stream >> mapTargets.TargetUID;
        _stream >> mapTargets.TargetType;
        _stream >> mapTargets.TargetColor;
        _stream >> mapTargets.azimuth;
        _stream >> mapTargets.latitude;
        _stream >> mapTargets.longitude;
        _stream >> mapTargets.running;
        _stream >> mapTargets.speed;
        _stream >> mapTargets.moveAzimuth;
        _stream >> mapTargets.moveDistance;
        //_stream >> mapTargets.delay;
        //_stream >> mapTargets.activeTime;
        _stream >> mapTargets.onlyNight;
        _stream >> mapTargets.onlyDay;
        _stream >> mapTargets.dropTarget;
        _stream >> mapTargets.blockTarget;
        _stream >> mapTargets.targetShowNumber;
        for(int j=0; j<mapTargets.targetShowNumber; j++)
        {
            TargetsShow targetsShow;
            _stream >> targetsShow.startTime;
            _stream >> targetsShow.dayTime;
            _stream >> targetsShow.nightTime;
            mapTargets.targetsShowList << targetsShow;
        }
        Targets.mapTargetsList << mapTargets;
        mapTargetsCount--;
//        _log->WriteBallisticsLog("GetTargetsDataPacket: ouid: " + QString::number(mapTargets.ObjectiveUID) +
//                                 " tuid: " + QString::number(mapTargets.TargetUID) +
//                                 " type: " + QString::number(mapTargets.TargetType) +
//                                 " delay: " + QString::number(mapTargets.delay) +
//                                 " atime: " + QString::number(mapTargets.activeTime));
    }

    _Instructor->TargetsEnqueuePacket(Targets);
}

void T_TCP_Client_Instructor::GetEnvironmentDataPacket(QDataStream &_stream)
{
    EnvironmentDataPacket Environment;

    Environment.PacketReady = true;
    _stream >> Environment.unixtime;
    _stream >> Environment.temperature;
    _stream >> Environment.cloudsGrade;
    _stream >> Environment.bottomCloudsLevel;
    _stream >> Environment.topCloudsLevel;
    _stream >> Environment.moistureLevel;
    _stream >> Environment.windSpeed;
    _stream >> Environment.windDirection;
    _stream >> Environment.visibility;
    _stream >> Environment.verticalWindSpeed;
    _stream >> Environment.windBlows;
    _stream >> Environment.pressure;
    _stream >> Environment.snowLevel;
    _stream >> Environment.camNumber;
    _stream >> Environment.fog;

    _Instructor->EnvironmentEnqueuePacket(Environment);
}

void T_TCP_Client_Instructor::GetAmmunitionDataPacket(QDataStream &_stream)
{
    AmmunitionDataPacket AmmunitionInPack;
    quint8 packageCount = 0;

    AmmunitionInPack.PacketReady = true;
    AmmunitionInPack.isNewPacket = true;
    _stream >> AmmunitionInPack.packageCount;
    AmmunitionInPack.AmmunitionList.clear();

    packageCount = AmmunitionInPack.packageCount;
    while(packageCount > 0)
    {
        Ammunition ammunition;
        _stream >> ammunition.gunType;
        _stream >> ammunition.ammoType;
        _stream >> ammunition.amount;
        AmmunitionInPack.AmmunitionList << ammunition;
        packageCount--;
    }

    _Instructor->AmmunitionInEnqueuePacket(AmmunitionInPack);
}

void T_TCP_Client_Instructor::GetControlDataPacket(QDataStream &_stream)
{
    ControlDataPacket Control;

    Control.PacketReady = true;
    _stream >> Control.CommandText;
    Control.CommandSize = Control.CommandText.size();

    _Instructor->ControlEnqueuePacket(Control);
}

void T_TCP_Client_Instructor::GetFailuresDataPacket(QDataStream &_stream)
{
    FailuresDataPacket Failures;

    Failures.PacketReady = true;
    _stream >> Failures.failureType;
    _stream >> Failures.failureActive;

    _Instructor->FailuresEnqueuePacket(Failures);
}

void T_TCP_Client_Instructor::GetCommandsDataPacket(QDataStream &_stream)
{
    CommandsDataPacket Commands;

    Commands.PacketReady = true;
    _stream >> Commands.MoveForward;

    _Instructor->CommandsEnqueuePacket(Commands);
}

void T_TCP_Client_Instructor::GetMessagesDataPacket(QDataStream &_stream)
{
    MessagesDataPacket Messages;

    Messages.PacketReady = true;
    _stream >> Messages.MessageType;
    _stream >> Messages.MessageText;

    _Instructor->MessagesEnqueuePacket(Messages);
}

void T_TCP_Client_Instructor::GetStatisticsDataPacket(QDataStream &_stream)
{
    StatisticsDataPacket Statistics;

    Statistics.PacketReady = true;
    _stream >> Statistics.Odometer;
    _stream >> Statistics.EngineClock;
    //QDateTime time = QDateTime::currentDateTime();
    //qDebug() << time.toTime_t() << _Instructor->Statistics->Odometer << _Instructor->Statistics->EngineClock;
    _Instructor->StatisticsEnqueuePacket(Statistics);
}

void T_TCP_Client_Instructor::GetScoreRequestDataPacket(QDataStream &_stream)
{
    qDebug( ) << "--<<" << __FUNCTION__;
    ScoreRequestDataPacket ScoreRequest;

    ScoreRequest.PacketReady = true;
    ScoreRequest.flag = true;
    _Instructor->ScoreRequestEnqueuePacket( ScoreRequest );
}

void T_TCP_Client_Instructor::SendCommonTcpData()
{
    SendBallisticDataPacket();
    SendTriggerDataPacket();
    SendImagerReadyDataPacket();
    SendServerReadyDataPacket();
    SendScoreDrivingDataPacket();
    SendScoreShootingDataPacket();
    SendScoreDrivingTruckDataPacket();
}

void T_TCP_Client_Instructor::SendBallisticDataPacket()
{
    if(!_Imager->Ballistic->PacketReady) _Imager->BallisticDequeuePacket();
    if(!_Imager->Ballistic->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Imager->Ballistic->ObjectiveUID);            // UID цели
        data_stream << static_cast<qint32>(_Imager->Ballistic->TargetUID);               // UID цели
        data_stream << static_cast<quint8>(_Imager->Ballistic->ObjectiveType);           // Тип цели
        data_stream << static_cast<bool>(_Imager->Ballistic->Hit);                       // Попадание
        data_stream << static_cast<float>(_Imager->Ballistic->Radius);                   // Радиус попадания
        data_stream << static_cast<quint8>(_Imager->Ballistic->GunType);                 // Тип оружия
        data_stream << static_cast<bool>(_Imager->Ballistic->TargetState);               // Мишень опущена/поднята
        data_stream << static_cast<quint8>(_Imager->Ballistic->TriggerID);               // ID Триггера
        data_stream << static_cast<quint8>(_Imager->Ballistic->Direction);               // Направление
        data_stream << static_cast<quint8>(_Imager->Ballistic->Hit_X);                   // Координаты пробоины по X (в % от левого края)
        data_stream << static_cast<quint8>(_Imager->Ballistic->Hit_Y);                   // Координаты пробоины по Y (в % от верхнего края)
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_BallisticEventsPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Imager->Ballistic->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::InstructorPacket,quint32(InstructorTCPPacketTypes::SR2IR_BallisticEventsPacket),packet);
    }
}

void T_TCP_Client_Instructor::SendTriggerDataPacket()
{
    if(!_Imager->Triggers->PacketReady) _Imager->TriggersDequeuePacket();
    if(!_Imager->Triggers->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Imager->Triggers->ObstacleUID);             // UID препятствия
        data_stream << static_cast<quint8>(_Imager->Triggers->ObstacleType);            // Тип препятствия
        data_stream << static_cast<quint8>(_Imager->Triggers->ObstacleIn);              // Въезд на препятствие
        data_stream << static_cast<quint8>(_Imager->Triggers->ObstacleOut);             // Выезд из препятствия
        data_stream << static_cast<quint8>(_Imager->Triggers->Trigger);                 // Триггер
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << quint8('S');
        header_stream << quint8('t');
        header_stream << quint8('o');
        header_stream << quint8('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_TriggerEventsPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Imager->Triggers->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::InstructorPacket,quint32(InstructorTCPPacketTypes::SR2IR_TriggerEventsPacket),packet);
    }
}

void T_TCP_Client_Instructor::SendImagerReadyDataPacket()
{
    if(!_Imager->ImagerReady->PacketReady) _Imager->ImagerReadyDequeuePacket();
    if(!_Imager->ImagerReady->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Imager->ImagerReady->simulatorType);          // Тип симулятора (техники)
        data_stream << static_cast<quint8>(_Imager->ImagerReady->ImagerReady);            // Готов к упражнению
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_ImagerReadyStatePacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Imager->ImagerReady->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::InstructorPacket,quint32(InstructorTCPPacketTypes::SR2IR_ImagerReadyStatePacket),packet);
    }
}

void T_TCP_Client_Instructor::SendServerReadyDataPacket()
{
    if(!_Server->ServerReady->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Server->ServerReady->simulatorType);          // Тип симулятора (техники)
        data_stream << static_cast<quint8>(_Server->ServerReady->ServerReady);            // Готов к упражнению
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_ServerReadyStatePacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        //_Server->ServerReady->PacketReady = false;
    }
}

void T_TCP_Client_Instructor::SendScoreDrivingDataPacket( )
{
    if ( !_Imager->ScoreDriving->PacketReady ) _Imager->ScoreDrivingDequeuePacket( );
    if ( !_Imager->ScoreDriving->PacketReady ) return;
    qDebug( ) << "-->>" << __FUNCTION__;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );

        data_stream << static_cast<float>( _Imager->ScoreDriving->Total );
        data_stream << static_cast<int>( _Imager->ScoreDriving->Speed );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Normativ );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator1 );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator2 );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator3 );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator4 );
        data_stream << static_cast<float>( _Imager->ScoreDriving->Indicator5 );
        data_stream << static_cast<float>( _Imager->ScoreDriving->AverageSpeed );

        QByteArray header;         
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_ScoreDrivingDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        _Imager->ScoreDriving->PacketReady = false;
        if ( _Instructor->exerciseStarted ) _replay->WritePacketData( _Instructor->ExerciseParameters->Uuid, ClientPacketTypes::InstructorPacket, quint32( InstructorTCPPacketTypes::SR2IR_ScoreDrivingDataPacket ), packet );
    }
}

void T_TCP_Client_Instructor::SendScoreShootingDataPacket( )
{
    if ( !_Imager->ScoreShooting->PacketReady ) _Imager->ScoreShootingDequeuePacket( );
    if ( !_Imager->ScoreShooting->PacketReady ) return;
    qDebug( ) << "-->>" << __FUNCTION__;

    qDebug( ) << _Imager->ScoreShooting->Total
        << _Imager->ScoreShooting->Target1 
        << _Imager->ScoreShooting->Target2
        << _Imager->ScoreShooting->Target3
        << _Imager->ScoreShooting->Target4;

    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<float>( _Imager->ScoreShooting->Total );
        data_stream << static_cast<float>( _Imager->ScoreShooting->Target1 );
        data_stream << static_cast<float>( _Imager->ScoreShooting->Target2 );
        data_stream << static_cast<float>( _Imager->ScoreShooting->Target3 );
        data_stream << static_cast<float>( _Imager->ScoreShooting->Target4 );
        QByteArray header;         
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_ScoreShootingDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        _Imager->ScoreShooting->PacketReady = false;
        if ( _Instructor->exerciseStarted ) _replay->WritePacketData( _Instructor->ExerciseParameters->Uuid, ClientPacketTypes::InstructorPacket, quint32( InstructorTCPPacketTypes::SR2IR_ScoreShootingDataPacket ), packet );
    }
}

void T_TCP_Client_Instructor::SendScoreDrivingTruckDataPacket( )
{
    if ( !_Imager->ScoreDrivingTruck->PacketReady ) _Imager->ScoreDrivingTruckDequeuePacket( );
    if ( !_Imager->ScoreDrivingTruck->PacketReady ) return;
    qDebug( ) << "-->>" << __FUNCTION__;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<float>( _Imager->ScoreDrivingTruck->Total );
        data_stream << static_cast<int>( _Imager->ScoreDrivingTruck->TimeDriving );
        data_stream << static_cast<float>( _Imager->ScoreDrivingTruck->Distance );

        QByteArray header;         
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_ScoreDrivingTruckDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        _Imager->ScoreDrivingTruck->PacketReady = false;
        if ( _Instructor->exerciseStarted ) _replay->WritePacketData( _Instructor->ExerciseParameters->Uuid, ClientPacketTypes::InstructorPacket, quint32( InstructorTCPPacketTypes::SR2IR_ScoreDrivingTruckDataPacket ), packet );
    }
}

