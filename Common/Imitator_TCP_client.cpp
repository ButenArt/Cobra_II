#include "Imitator_TCP_client.h"


T_TCP_Client_ControlsImitator::T_TCP_Client_ControlsImitator()
{

}

T_TCP_Client_ControlsImitator::~T_TCP_Client_ControlsImitator()
{

}

void T_TCP_Client_ControlsImitator::slotConnected()
{
    _ControlsImitator->ControlsImitatorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ControlsImitator_connected,"");
}

void T_TCP_Client_ControlsImitator::slotDisconnected()
{
    _ControlsImitator->ControlsImitatorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ControlsImitator_disconnected,"");
}

void T_TCP_Client_ControlsImitator::slotError()
{
    _ControlsImitator->ControlsImitatorActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_ControlsImitator_Error,socketErrorString);
}

void T_TCP_Client_ControlsImitator::ReadCommonTcpData()
{
    bool processedHeader = false;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_ControlsImitator->PacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _ControlsImitator->PacketHeader.Mark;
                header_stream >> _ControlsImitator->PacketHeader.PacketType;
                header_stream >> _ControlsImitator->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _ControlsImitator->PacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_ControlsImitator->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_ControlsImitator->PacketHeader.PacketType)
                {
                    default:
                        _log->WriteLANLog(LogEvents::TCP_ControlsImitator_Unknown_PacketType,QString::number(_ControlsImitator->PacketHeader.PacketType));
                    break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void T_TCP_Client_ControlsImitator::SendCommonTcpData()
{

}

