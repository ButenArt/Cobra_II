#include "GunnerMotion_TCP_client.h"


T_TCP_Client_GunnerMotionSystem::T_TCP_Client_GunnerMotionSystem()
{

}

T_TCP_Client_GunnerMotionSystem::~T_TCP_Client_GunnerMotionSystem()
{

}

void T_TCP_Client_GunnerMotionSystem::slotConnected()
{
    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_connected, "");
}

void T_TCP_Client_GunnerMotionSystem::slotDisconnected()
{
    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_disconnected, "");
}

void T_TCP_Client_GunnerMotionSystem::slotError()
{
    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_Error, socketErrorString);
}

void T_TCP_Client_GunnerMotionSystem::SendCommonTcpData()
{
    SendGunnerMotionStateDataPacket();
}

void T_TCP_Client_GunnerMotionSystem::SendGunnerMotionStateDataPacket()
{
    if(!_MotionSystem->GunnerMotionState->PacketReady) _MotionSystem->GunnerMotionStateDequeuePacket();
    double MatmodelZ = _MotionSystem->GunnerMotionState->matmodelZ;
    float Roll = _MotionSystem->GunnerMotionState->Roll / _ini->Divider_MotionSystemGunner();
    float Pitch = _MotionSystem->GunnerMotionState->Pitch / _ini->Divider_MotionSystemGunner();
    float Course = _MotionSystem->GunnerMotionState->Course / _ini->Divider_MotionSystemGunner();
    if(_ini->RollInverse_MotionSystemGunner()) Roll = Roll * -1;
    if(_ini->PitchInverse_MotionSystemGunner()) Pitch = Pitch * -1;
    if(_ini->CourseInverse_MotionSystemGunner()) Course = Course * -1;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<double>(0);
        data_stream << static_cast<double>(0);
        data_stream << static_cast<double>(MatmodelZ);    // дистанция по оси Z от стартового положения из мат модели (метры)
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<float>(Roll);          // крен (градусы)
        data_stream << static_cast<float>(Pitch);         // тангаж (градусы)
        data_stream << static_cast<float>(Course);        // направление (градусы)
        data_stream << static_cast<float>(0);
        data_stream << static_cast<float>(0);
        data_stream << static_cast<float>(0);
        data_stream << static_cast<float>(0);
        data_stream << static_cast<float>(0);
        data_stream << static_cast<float>(0);
        data_stream << static_cast<quint32>(0);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint32>(_MotionSystem->GunnerPacketHeader.MagicNumber);
        header_stream << static_cast<quint16>(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket);
        header_stream << static_cast<quint16>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        _MotionSystem->GunnerMotionState->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::MotionSystemPacket,quint32(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket),packet);
    }
}

void T_TCP_Client_GunnerMotionSystem::ReadCommonTcpData()
{
    bool processedHeader = false;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_MotionSystem->GunnerPacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _MotionSystem->GunnerPacketHeader.MagicNumber;
                header_stream >> _MotionSystem->GunnerPacketHeader.PacketType;
                header_stream >> _MotionSystem->GunnerPacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _MotionSystem->GunnerPacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_MotionSystem->GunnerPacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_MotionSystem->GunnerPacketHeader.PacketType)
                {
                  case static_cast<quint32>(MotionSystemTCPPacketTypes::MS2SR_MotionSystemFeedbackDataPacket):
                     GetGunnerMotionFeedbackDataPacket(data_stream);
                  break;
                  default:
                     _log->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_Unknown_PacketType, QString::number(_MotionSystem->GunnerPacketHeader.PacketType));
                  break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void T_TCP_Client_GunnerMotionSystem::GetGunnerMotionFeedbackDataPacket(QDataStream &_stream)
{
    MotionSystemFeedbackDataPacket GunnerMotionFeedback;

    GunnerMotionFeedback.PacketReady = true;
    _stream >> GunnerMotionFeedback.X;
    _stream >> GunnerMotionFeedback.Y;
    _stream >> GunnerMotionFeedback.Z;
    _stream >> GunnerMotionFeedback.Roll;
    _stream >> GunnerMotionFeedback.Pitch;
    _stream >> GunnerMotionFeedback.Course;

    _MotionSystem->GunnerMotionFeedbackEnqueuePacket(GunnerMotionFeedback);
}
