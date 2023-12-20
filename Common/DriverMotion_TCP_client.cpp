#include "DriverMotion_TCP_client.h"


T_TCP_Client_DriverMotionSystem::T_TCP_Client_DriverMotionSystem()
{

}

T_TCP_Client_DriverMotionSystem::~T_TCP_Client_DriverMotionSystem()
{

}

void T_TCP_Client_DriverMotionSystem::slotConnected()
{
    _MotionSystem->DriverMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_DriverMotionSystem_connected, "");
}

void T_TCP_Client_DriverMotionSystem::slotDisconnected()
{
    _MotionSystem->DriverMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_DriverMotionSystem_disconnected, "");
}

void T_TCP_Client_DriverMotionSystem::slotError()
{
    _MotionSystem->DriverMotionSystemActive = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_DriverMotionSystem_Error, socketErrorString);
}

void T_TCP_Client_DriverMotionSystem::SendCommonTcpData()
{
    SendDriverMotionStateDataPacket();
}

void T_TCP_Client_DriverMotionSystem::SendDriverMotionStateDataPacket()
{
    if (!_MotionSystem->DriverMotionState->PacketReady) _MotionSystem->DriverMotionStateDequeuePacket();
    double MatmodelZ = _MotionSystem->DriverMotionState->matmodelZ;
    float Roll = _MotionSystem->DriverMotionState->Roll / _ini->Divider_MotionSystemDriver();
    float Pitch = _MotionSystem->DriverMotionState->Pitch / _ini->Divider_MotionSystemDriver();
    float CourseNew = _MotionSystem->DriverMotionState->Course / _ini->Divider_MotionSystemDriver();


    if (_ini->RollInverse_MotionSystemDriver()) Roll = Roll * -1;
    if (_ini->PitchInverse_MotionSystemDriver()) Pitch = Pitch * -1;
    if (_ini->CourseInverse_MotionSystemDriver()) CourseNew = CourseNew * -1;

    float Course = (CourseOLD - CourseNew) * 5;
    if (Course > 5)
        Course -= 1800;
    if (Course < -5)
        Course += 1800;
    CourseOLD = CourseNew;
    if (_MotionSystem->Speed > 0) {
        accelerationOverload = speed - _MotionSystem->Speed;
        //qDebug() << (speed - _MotionSystem->Speed) << " = (speed - _MotionSystem->Speed)\n";
        if (((speed - _MotionSystem->Speed) < 0) && (speed > 1)) {
            Pitch += std::abs((speed - _MotionSystem->Speed)) * 30;
        }
        if (((speed - _MotionSystem->Speed) > 0) && (speed > 1)) {
            Pitch -= std::abs((speed - _MotionSystem->Speed)) * 30;
        }
    }

    speed = std::abs(_MotionSystem->Speed);

    if (speed < 1) {
        Course = 0;
        accelerationOverload = 0;
    }

    if (Course > MaxCURS)
        MaxCURS = Course;
    if (Course < MinCURS)
        MinCURS = Course;

    calculateAccelerations(Roll, Pitch, Course, speed); // Расчет ускорений, но пока не точный
    //qDebug() << Roll << " = ROLL\n"
    //    << Pitch << " = Pitch\n"
    //    << Course << " = Course\n"
    //    << speed << " = speed\n"
    //    << accelerationOverload << " = accelerationOverload\n"
    //    << MaxCURS << " = MaxCURS\n"
    //    << MinCURS << " = MinCURS\n";
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<double>(accelerationOverload);
        data_stream << static_cast<double>(0);
        data_stream << static_cast<double>(MatmodelZ);    // дистанция по оси Z от стартового положения из мат модели (метры)
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<float>(Roll);          // крен (градусы)
        data_stream << static_cast<float>(Pitch);         // тангаж (градусы)
        data_stream << static_cast<float>(Course);        // направление (градусы)
        data_stream << static_cast<float>(Accel.Ax);
        data_stream << static_cast<float>(Accel.Az);
        data_stream << static_cast<float>(Accel.Ay);
        data_stream << static_cast<float>(Angul.Wx);
        data_stream << static_cast<float>(Angul.Wz);
        data_stream << static_cast<float>(Angul.Wy);
        data_stream << static_cast<quint32>(0);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint32>(_MotionSystem->DriverPacketHeader.MagicNumber);
        header_stream << static_cast<quint16>(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket);
        header_stream << static_cast<quint16>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        _MotionSystem->DriverMotionState->PacketReady = false;
        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::MotionSystemPacket,quint32(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket),packet);
    }
}

void T_TCP_Client_DriverMotionSystem::ReadCommonTcpData()
{
    bool processedHeader = false;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(_MotionSystem->DriverPacketHeader);
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _MotionSystem->DriverPacketHeader.MagicNumber;
                header_stream >> _MotionSystem->DriverPacketHeader.PacketType;
                header_stream >> _MotionSystem->DriverPacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= _MotionSystem->DriverPacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_MotionSystem->DriverPacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(_MotionSystem->DriverPacketHeader.PacketType)
                {
                  case static_cast<quint32>(MotionSystemTCPPacketTypes::MS2SR_MotionSystemFeedbackDataPacket):
                     GetDriverMotionFeedbackDataPacket(data_stream);
                  break;
                  default:
                     _log->WriteLANLog(LogEvents::TCP_DriverMotionSystem_Unknown_PacketType, QString::number(_MotionSystem->DriverPacketHeader.PacketType));
                  break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void T_TCP_Client_DriverMotionSystem::GetDriverMotionFeedbackDataPacket(QDataStream &_stream)
{
    MotionSystemFeedbackDataPacket DriverMotionFeedback;

    DriverMotionFeedback.PacketReady = true;
    _stream >> DriverMotionFeedback.X;
    _stream >> DriverMotionFeedback.Y;
    _stream >> DriverMotionFeedback.Z;
    _stream >> DriverMotionFeedback.Roll;
    _stream >> DriverMotionFeedback.Pitch;
    _stream >> DriverMotionFeedback.Course;

    _MotionSystem->DriverMotionFeedbackEnqueuePacket(DriverMotionFeedback);
}

void T_TCP_Client_DriverMotionSystem::calculateAccelerations(double rollAngle, double pitchAngle, double yawAngle, double velocity) {
    const double gravity = 9.8;  // Ускорение свободного падения
    rollAngle = rollAngle * M_PI / 180; pitchAngle = pitchAngle * M_PI / 180; yawAngle = yawAngle * M_PI / 180; velocity = velocity / 3.6;
    // Вычисляем ускорения по трем осям
    Accel.Ax = (mass * velocity * velocity * std::sin(rollAngle)) / 1.2;
    Accel.Ay = (mass * velocity * velocity * std::sin(pitchAngle)) / 0.9;
    Accel.Az = gravity - (mass * velocity * velocity * std::sin(yawAngle)) / 0.2;
    calculateAngularAccelerations(velocity, Accel.Ax, Accel.Ay, Accel.Az);
}

void T_TCP_Client_DriverMotionSystem::calculateAngularAccelerations(double velocity, double accelerationX, double accelerationY, double accelerationZ) {
    // Вычисляем угловые ускорения по трем осям
    Angul.Wx = (accelerationX * 1.2) / (mass * velocity);
    Angul.Wy = (accelerationY * 0.9) / (mass * velocity);
    Angul.Wz = (accelerationZ * 0.2) / (mass * velocity);
}
