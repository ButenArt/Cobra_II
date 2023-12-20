#include "Script_TCP_client.h"


T_TCP_Client_ScriptServer::T_TCP_Client_ScriptServer()
{

}

T_TCP_Client_ScriptServer::~T_TCP_Client_ScriptServer()
{

}

void T_TCP_Client_ScriptServer::slotConnected()
{
    _ScriptServer->ScriptServerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ScriptServer_connected,"");
}

void T_TCP_Client_ScriptServer::slotDisconnected()
{
    _ScriptServer->ScriptServerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ScriptServer_disconnected,"");
}

void T_TCP_Client_ScriptServer::slotError()
{
    _ScriptServer->ScriptServerActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ScriptServer_Error,socketErrorString);
}

void T_TCP_Client_ScriptServer::ReadCommonTcpData()
{
    bool processedHeader = false;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_ScriptServer->PacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _ScriptServer->PacketHeader.Mark;
                header_stream >> _ScriptServer->PacketHeader.PacketType;
                header_stream >> _ScriptServer->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _ScriptServer->PacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_ScriptServer->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_ScriptServer->PacketHeader.PacketType)
                {
                    default:
                        _log->WriteLANLog(LogEvents::TCP_ScriptServer_Unknown_PacketType,QString::number(_ScriptServer->PacketHeader.PacketType));
                    break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void T_TCP_Client_ScriptServer::SendCommonTcpData()
{
    SendControlDataPacket();
}

void T_TCP_Client_ScriptServer::SendControlDataPacket()
{
    if(!_Instructor->Control->PacketReady) _Instructor->ControlDequeuePacket();
    if(!_Instructor->Control->PacketReady) return;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<QString>(_Instructor->Control->CommandText);        // текст команды
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('C');
        header_stream << static_cast<quint32>(ScriptServerTCPPacketTypes::SR2ST_ControlDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        _Instructor->Control->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ScriptServerPacket,quint32(ScriptServerTCPPacketTypes::SR2ST_ControlDataPacket),packet);
    }
}
