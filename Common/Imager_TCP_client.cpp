#include "Imager_TCP_client.h"


T_TCP_Client_Imager::T_TCP_Client_Imager()
{

}

T_TCP_Client_Imager::~T_TCP_Client_Imager()
{

}

void T_TCP_Client_Imager::slotConnected()
{
    _Imager->ImagerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Imager_connected, "");
}

void T_TCP_Client_Imager::slotDisconnected()
{
    _Imager->ImagerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Imager_disconnected, "");
}

void T_TCP_Client_Imager::slotError()
{
    _Imager->ImagerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_Imager_Error, socketErrorString);
}

void T_TCP_Client_Imager::SendCommonTcpData()
{
    SendExerciseParametersPacket();
    SendObjectsDataPacket();
    SendTargetsDataPacket();
    SendEnvironmentDataPacket();
    SendCommandsDataPacket();
    SendMessagesDataPacket();
    SendTargetsControlDataPacket();
    SendProjectileDataPacket();
    SendWeaponStateDataPacket();
    SendScoreRequestDataPacket( );
}

void T_TCP_Client_Imager::SendExerciseParametersPacket()
{

    if(!_Instructor->ExerciseParameters->PacketReady) _Instructor->ExerciseParametersDequeuePacket();
    if(!_Instructor->ExerciseParameters->PacketReady) return;

    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->ReStart);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->Pause);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->Stop);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters->ExerciseNum);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters->MapType);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->night);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters->season);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->Joystick);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->AutoloadingSystem );
        data_stream << static_cast<qint32>(_Instructor->ExerciseParameters->directionsNumber);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters->backfire);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters->selectedKit);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters->selectedMode);
        data_stream << static_cast<quint32>(_Instructor->ExerciseParameters->distance2screen);
        //data_stream << static_cast<QString>(_Instructor->ExerciseParameters->guid_run);
        data_stream << static_cast<QString>(_Instructor->ExerciseParameters->lang);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_ExerciseParametersPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        _Instructor->ExerciseParameters->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_ExerciseParametersPacket),packet);
        qDebug( ) << "<<--" __FUNCTION__;
        qDebug( ) << "ds2scr:" << _Instructor->ExerciseParameters->distance2screen << "lang:" << _Instructor->ExerciseParameters->lang;

    }
}

void T_TCP_Client_Imager::SendObjectsDataPacket()
{
    if(!_Instructor->Objects->PacketReady) _Instructor->ObjectsDequeuePacket();
    if(!_Instructor->Objects->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Instructor->Objects->mapObjCount);
        for(int i=0; i<_Instructor->Objects->mapObjCount; i++)
        {
            MapObjects mapObj = static_cast<MapObjects>(_Instructor->Objects->mapObjList[i]);
            data_stream << static_cast<qint32>(mapObj.objUID);
            data_stream << static_cast<quint8>(mapObj.type);
            data_stream << static_cast<quint8>(mapObj.subType);
            data_stream << static_cast<qint32>(mapObj.azimuth);
            data_stream << static_cast<float>(mapObj.latitude);
            data_stream << static_cast<float>(mapObj.longitude);
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_ObjectsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Objects->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_ObjectsDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendTargetsDataPacket()
{
    if(!_Instructor->Targets->PacketReady) _Instructor->TargetsDequeuePacket();
    if(!_Instructor->Targets->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Instructor->Targets->mapTargetsCount);
        for(int i=0; i<_Instructor->Targets->mapTargetsCount; i++)
        {
            MapTargets mapTargets = static_cast<MapTargets>(_Instructor->Targets->mapTargetsList[i]);
            data_stream << static_cast<qint32>(mapTargets.ObjectiveUID);
            data_stream << static_cast<qint32>(mapTargets.TargetUID);
            data_stream << static_cast<quint8>(mapTargets.TargetType);
            data_stream << static_cast<quint8>(mapTargets.TargetColor);
            data_stream << static_cast<qint32>(mapTargets.azimuth);
            data_stream << static_cast<float>(mapTargets.latitude);
            data_stream << static_cast<float>(mapTargets.longitude);
            data_stream << static_cast<bool>(mapTargets.running);
            data_stream << static_cast<float>(mapTargets.speed);
            data_stream << static_cast<qint32>(mapTargets.moveAzimuth);
            data_stream << static_cast<qint32>(mapTargets.moveDistance);
            //data_stream << static_cast<qint32>(mapTargets.delay);
            //data_stream << static_cast<qint32>(mapTargets.activeTime);
            data_stream << static_cast<bool>(mapTargets.onlyNight);
            data_stream << static_cast<bool>(mapTargets.onlyDay);
            data_stream << static_cast<bool>(mapTargets.dropTarget);
            data_stream << static_cast<bool>(mapTargets.blockTarget);
            data_stream << static_cast<quint8>(mapTargets.targetShowNumber);
            for(int j=0; j<mapTargets.targetShowNumber; j++)
            {
                TargetsShow targetsShow = static_cast<TargetsShow>(_Instructor->Targets->mapTargetsList[i].targetsShowList[j]);
                data_stream << static_cast<qint32>(targetsShow.startTime);
                data_stream << static_cast<qint32>(targetsShow.dayTime);
                data_stream << static_cast<qint32>(targetsShow.nightTime);
            }
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_TargetsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Targets->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_TargetsDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendEnvironmentDataPacket()
{
    if(!_Instructor->Environment->PacketReady) _Instructor->EnvironmentDequeuePacket();
    if(!_Instructor->Environment->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint32>(_Instructor->Environment->unixtime);
        data_stream << static_cast<qint32>(_Instructor->Environment->temperature);
        data_stream << static_cast<qint32>(_Instructor->Environment->cloudsGrade);
        data_stream << static_cast<float>(_Instructor->Environment->bottomCloudsLevel);
        data_stream << static_cast<float>(_Instructor->Environment->topCloudsLevel);
        data_stream << static_cast<bool>(_Instructor->Environment->moistureLevel);
        data_stream << static_cast<float>(_Instructor->Environment->windSpeed);
        data_stream << static_cast<float>(_Instructor->Environment->windDirection);
        data_stream << static_cast<float>(_Instructor->Environment->visibility);
        data_stream << static_cast<float>(_Instructor->Environment->verticalWindSpeed);
        data_stream << static_cast<float>(_Instructor->Environment->windBlows);
        data_stream << static_cast<float>(_Instructor->Environment->pressure);
        data_stream << static_cast<qint32>(_Instructor->Environment->snowLevel);
        data_stream << static_cast<qint32>(_Instructor->Environment->camNumber);
        data_stream << static_cast<bool>(_Instructor->Environment->fog);
        data_stream << static_cast<qint32>( _Instructor->Environment->amount_precipitation );
        data_stream << static_cast<qint32>( _Instructor->Environment->intensity_precipitation );
        data_stream << static_cast<qint32>( _Instructor->Environment->intensity_vvp );
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_EnvironmentDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Environment->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_EnvironmentDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendCommandsDataPacket()
{
    if(!_Instructor->Commands->PacketReady) _Instructor->CommandsDequeuePacket();
    if(!_Instructor->Commands->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Instructor->Commands->MoveForward);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_CommandsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Commands->PacketReady = false;
        _Instructor->commandMoveForward = true;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_CommandsDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendMessagesDataPacket()
{
    if(!_Instructor->Messages->PacketReady) _Instructor->MessagesDequeuePacket();
    if(!_Instructor->Messages->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Instructor->Messages->MessageType);
        data_stream << static_cast<QString>(_Instructor->Messages->MessageText);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_MessagesDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Messages->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_MessagesDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendTargetsControlDataPacket()
{
    if(!_Imager->TargetsControl->PacketReady) _Imager->TargetsControlDequeuePacket();
    if(!_Imager->TargetsControl->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Imager->TargetsControl->mapTargetsCount);
        for(int i = 0; i < _Imager->TargetsControl->mapTargetsCount; i++)
        {
            data_stream << static_cast<qint32>(_Imager->TargetsControl->targetControlList[i].ObjectiveUID);
            data_stream << static_cast<qint32>(_Imager->TargetsControl->targetControlList[i].TargetUID);
            data_stream << static_cast<quint8>(_Imager->TargetsControl->targetControlList[i].TargetState);
//            _log->WriteBallisticsLog("Send to Imager: ouid: " + QString::number(_Imager->TargetsControl->targetControlList[i].ObjectiveUID) +
//                                     " tuid: " + QString::number(_Imager->TargetsControl->targetControlList[i].TargetUID) +
//                                     " state: " + QString::number(_Imager->TargetsControl->targetControlList[i].TargetState));
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_TargetsControlDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Imager->TargetsControl->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_TargetsControlDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendProjectileDataPacket()
{
    if(!_Imager->Projectile->PacketReady) _Imager->ProjectileDequeuePacket();
    if(!_Imager->Projectile->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Imager->Projectile->TrajectoriesCount);
        for(int i=0; i<_Imager->Projectile->TrajectoriesCount; i++)
        {
            Trajectories trajectories = static_cast<Trajectories>(_Imager->Projectile->TrajectoriesList[i]);
            data_stream << static_cast<qint32>(trajectories.WeaponID);
            data_stream << static_cast<quint8>(trajectories.WeaponType);
            data_stream << static_cast<quint8>(trajectories.ProjectileType);
            data_stream << static_cast<quint8>(trajectories.ProjectileSubType);
            data_stream << static_cast<float>(trajectories.InitialVelocity);
            data_stream << static_cast<qint32>(trajectories.ShotID);
            data_stream << static_cast<float>(trajectories.time);
            data_stream << static_cast<float>(trajectories.pos_offset);
            data_stream << static_cast<float>(trajectories.pos_height);
            data_stream << static_cast<float>(trajectories.pos_distance);
            data_stream << static_cast<float>(trajectories.rise_angle);
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_ProjectileDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        //qDebug() << "Projectile send:  " << QDateTime::currentDateTime().currentMSecsSinceEpoch();
        _Imager->Projectile->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,quint32(ImagerTCPPacketTypes::SR2IG_ProjectileDataPacket),packet);
    }
}

void T_TCP_Client_Imager::SendWeaponStateDataPacket()
{
    //if(!_Instructor->exerciseStarted) return;
    if(!_Instructor->AmmunitionInPack->isNewPacket) return;

    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint32>(_Instructor->AmmunitionInPack->packageCount);  // число наборов боеприпасов
        for(int i=0; i<_Instructor->AmmunitionInPack->packageCount; i++)
        {
            Ammunition ammunition = static_cast<Ammunition>(_Instructor->AmmunitionInPack->AmmunitionList[i]);
            data_stream << static_cast<quint8>(ammunition.gunType);
            data_stream << static_cast<quint8>(ammunition.ammoType);
            data_stream << static_cast<quint32>(ammunition.amount);
        }
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_WeaponStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ImagerPacket,static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_WeaponStateDataPacket),packet);
        if(_Imager->ImagerState->AmmoLoaded)
            _Instructor->AmmunitionInPack->isNewPacket = false;

//        if(!_Imager->ImagerState->VehicleInLevel) _Instructor->AmmunitionInPack->PacketReadyToSend = true;
//        else _Instructor->AmmunitionInPack->PacketReadyToSend = false;
    }
}

void T_TCP_Client_Imager::SendScoreRequestDataPacket()
{
    if ( !_Instructor->ScoreRequest->PacketReady ) _Instructor->ScoreRequestDequeuePacket( );
    if ( !_Instructor->ScoreRequest->PacketReady ) return;
    qDebug( ) << "<<--" << __FUNCTION__;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<bool>( _Instructor->ScoreRequest->flag );

        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'V' );
        header_stream << static_cast<quint32>( ImagerTCPPacketTypes::SR2IG_ScoreRequestDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        _Instructor->ScoreRequest->PacketReady = false;
        if ( _Instructor->exerciseStarted ) _replay->WritePacketData( _Instructor->ExerciseParameters->Uuid, ClientPacketTypes::ImagerPacket, quint32( ImagerTCPPacketTypes::SR2IG_ScoreRequestDataPacket ), packet );
    }
}

void T_TCP_Client_Imager::ReadCommonTcpData()
{
    bool processedHeader = false;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_Imager->PacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _Imager->PacketHeader.Mark;
                header_stream >> _Imager->PacketHeader.PacketType;
                header_stream >> _Imager->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _Imager->PacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_Imager->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_Imager->PacketHeader.PacketType)
                {
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerStateDataPacket):
                     GetImagerStateDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_BallisticEventsPacket):
                     GetBallisticDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerReadyDataPacket):
                     GetImagerReadyDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_TriggerEventsPacket):
                     GetTriggerDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerElementsStateDataPacket):
                     GetImagerElementsStateDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ScoreDrivingDataPacket ):
                     GetScoreDrivingDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ScoreShootingDataPacket ):
                     GetScoreShootingDataPacket(data_stream);
                  break;
                  case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ScoreDrivingTruckDataPacket ):
                     GetScoreDrivingTruckDataPacket(data_stream);
                  break;
                  default:
                      qDebug( ) << "Неизвестный пакет от Imager" << _Imager->PacketHeader.PacketType;
                     _log->WriteLANLog(LogEvents::TCP_Imager_Unknown_PacketType, QString::number(_Imager->PacketHeader.PacketType));
                  break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

// Обработчик пакета №4 - общие пакеты https://docs.google.com/spreadsheets/d/1aXbV-L_XWAVQD7CVckTeHpQEdTlID4dP8ddYpb90400/edit?pli=1#gid=1353131782
void T_TCP_Client_Imager::GetImagerStateDataPacket(QDataStream &_stream)
{
    ImagerStateDataPacket ImagerState;
    qint32 WheelsCount = 0;

    ImagerState.PacketReady = true;
    _stream >> ImagerState.X;
    _stream >> ImagerState.Y;
    _stream >> ImagerState.Z;
    _stream >> ImagerState.Roll;
    _stream >> ImagerState.Pitch;
    _stream >> ImagerState.Course;
    _stream >> ImagerState.Sail;
    _stream >> ImagerState.Surface;
    _stream >> ImagerState.Speed;
    _stream >> ImagerState.WheelsCount;
    ImagerState.WheelsList.clear();
    WheelsCount = ImagerState.WheelsCount;
    while(WheelsCount > 0)
    {
        Wheels wheel;
        _stream >> wheel.wheelSpeed;
        ImagerState.WheelsList << wheel;
        WheelsCount--;
    }
    _stream >> ImagerState.EngineTorque;
    _stream >> ImagerState.RPM;
    _stream >> ImagerState.AmmoLoaded;

    _Imager->ImagerStateEnqueuePacket(ImagerState);

    MotionSystemStateDataPacket MotionState;
    MotionState.PacketReady = true;
    MotionState.Roll = ImagerState.Roll;
    MotionState.Pitch = ImagerState.Pitch;
    MotionState.Course = ImagerState.Course;
    MotionState.matmodelZ = 0;
    _MotionSystem->Speed = ImagerState.Speed;
    _MotionSystem->RPM = ImagerState.RPM;
    if(_Server->ResetPowerTickCounter == 0)
    {
        _MotionSystem->DriverMotionStateEnqueuePacket(MotionState);
        _MotionSystem->GunnerMotionStateEnqueuePacket(MotionState);
    }
}

void T_TCP_Client_Imager::GetBallisticDataPacket(QDataStream &_stream)
{
    BallisticDataPacket Ballistic;

    Ballistic.PacketReady = true;
    _stream >> Ballistic.ObjectiveUID;
    _stream >> Ballistic.TargetUID;
    _stream >> Ballistic.ObjectiveType;
    _stream >> Ballistic.Hit;
    _stream >> Ballistic.Radius;
    _stream >> Ballistic.GunType;
    _stream >> Ballistic.TargetState;
    _stream >> Ballistic.TriggerID;
    _stream >> Ballistic.Direction;
    _stream >> Ballistic.Hit_X;
    _stream >> Ballistic.Hit_Y;
    _stream >> Ballistic.ammoAmountMG;

    _Imager->BallisticEnqueuePacket(Ballistic);
}

void T_TCP_Client_Imager::GetImagerReadyDataPacket(QDataStream &_stream)
{
    ImagerReadyDataPacket ImagerReady;
    quint8 simulatorType = 0;

    ImagerReady.PacketReady = true;
    _stream >> simulatorType;  //*** todo ***// надо сделать проверку на соответствие вида техники визуалки и сервера
    _stream >> ImagerReady.ImagerReady;
    ImagerReady.simulatorType = static_cast<SimulatorTypes>(simulatorType);

    _Imager->ImagerReadyEnqueuePacket(ImagerReady);
}

void T_TCP_Client_Imager::GetTriggerDataPacket(QDataStream &_stream)
{
    TriggerDataPacket Triggers;

    Triggers.PacketReady = true;
    _stream >> Triggers.ObstacleUID;
    _stream >> Triggers.ObstacleType;
    _stream >> Triggers.ObstacleIn;
    _stream >> Triggers.ObstacleOut;
    _stream >> Triggers.Trigger;

    _Imager->TriggersEnqueuePacket(Triggers);
}

void T_TCP_Client_Imager::GetImagerElementsStateDataPacket(QDataStream &_stream)
{
    ImagerElementsStateDataPacket ImagerElementsState;
    qint32 ElementsCount = 0;

    qint32 ElementNameSize = 0;
    QByteArray ElementName = 0;
    qint32 CommentSize = 0;
    QByteArray Comment = 0;
    quint8 ElementNameDetail = 0;
    quint8 CommentDetail = 0;

    ImagerElementsState.PacketReady = true;
    _stream >> ImagerElementsState.ElementsCount;
    ImagerElementsState.ElementsStateList.clear();

    ElementsCount = ImagerElementsState.ElementsCount;
    while(ElementsCount > 0)
    {
        Elements elements;
        _stream >> elements.ElementType;
        _stream >> elements.ElementState;
        _stream >> elements.ElementNameSize;
        ElementNameSize = elements.ElementNameSize;
        while(ElementNameSize > 0)
        {
            _stream >> ElementNameDetail;
            ElementName.append(ElementNameDetail);
            ElementNameSize--;
        }
        elements.ElementName = QTextCodec::codecForMib(1015)->toUnicode(ElementName);
        ElementName = 0;
        _stream >> elements.CommentSize;
        CommentSize = elements.CommentSize;
        while(CommentSize > 0)
        {
            _stream >> CommentDetail;
            Comment.append(CommentDetail);
            CommentSize--;
        }
        elements.Comment = QTextCodec::codecForMib(1015)->toUnicode(Comment);
        Comment = 0;
        ImagerElementsState.ElementsStateList << elements;
        ElementsCount--;
    }
    _Imager->ImagerElementsStateEnqueuePacket(ImagerElementsState);
}

void T_TCP_Client_Imager::GetScoreDrivingTruckDataPacket(QDataStream &_stream)
{
    qDebug( ) << ">>--" << __FUNCTION__;

    ScoreDrivingTruckDataPacket ScoreDrivingTruck;

    ScoreDrivingTruck.PacketReady = true;
    _stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
    _stream >> ScoreDrivingTruck.Total;
    _stream >> ScoreDrivingTruck.TimeDriving;
    _stream >> ScoreDrivingTruck.Distance;

    _Imager->ScoreDrivingTruckEnqueuePacket( ScoreDrivingTruck );
}

void T_TCP_Client_Imager::GetScoreShootingDataPacket(QDataStream &_stream)
{
    qDebug( ) << ">>--" << __FUNCTION__;
    ScoreShootingDataPacket ScoreShooting;

    ScoreShooting.PacketReady = true;
    _stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
    _stream >> ScoreShooting.Total;
    _stream >> ScoreShooting.Target1;
    _stream >> ScoreShooting.Target2;
    _stream >> ScoreShooting.Target3;
    _stream >> ScoreShooting.Target4;
    qDebug( ) << ScoreShooting.Total << ScoreShooting.Target1 << ScoreShooting.Target2 << ScoreShooting.Target3 << ScoreShooting.Target4;
    _Imager->ScoreShootingEnqueuePacket( ScoreShooting );
}

void T_TCP_Client_Imager::GetScoreDrivingDataPacket(QDataStream &_stream)
{
    qDebug( ) << ">>--" << __FUNCTION__;
    ScoreDrivingDataPacket ScoreDriving;

    ScoreDriving.PacketReady = true;
    _stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
    _stream >> ScoreDriving.Total;
    _stream >> ScoreDriving.Speed;
    _stream >> ScoreDriving.Indicator;
    _stream >> ScoreDriving.Normativ;
    _stream >> ScoreDriving.Indicator1;
    _stream >> ScoreDriving.Indicator2;
    _stream >> ScoreDriving.Indicator3;
    _stream >> ScoreDriving.Indicator4;
    _stream >> ScoreDriving.Indicator5;
    _stream >> ScoreDriving.AverageSpeed;

    _Imager->ScoreDrivingEnqueuePacket( ScoreDriving );
}
