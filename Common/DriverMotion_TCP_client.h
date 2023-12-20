#ifndef DRIVERMOTION_TCP_CLIENT_H
#define DRIVERMOTION_TCP_CLIENT_H

#include <QDataStream>
#include "TCP_client.h"
#include "MotionSystem.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"
#include "SettingsFile.h"

struct Acceleration {
    double Ax;
    double Ay;
    double Az;
};

struct AngularAcceleration {
    double Wx;
    double Wy;
    double Wz;
};

class T_TCP_Client_DriverMotionSystem : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_DriverMotionSystem();
    ~T_TCP_Client_DriverMotionSystem();

    double mass = 14300.0;   // Масса машины в килограммах
    double speed = 0;
    double RPM = 0;
    double MaxCURS = 0;
    double MinCURS = 0;
    double accelerationOverload = 0;
    double brakingOverload = 0;
    float CourseOLD = 0;
    Acceleration Accel;
    AngularAcceleration Angul;

protected:
    T_MotionSystem *_MotionSystem;
    T_Instructor *_Instructor;
    TSettingsFile *_ini;
    TLogging *_log;
    TReplay *_replay;
    double distanceZ = 0.2;  // Дистанция от вертикальной оси до центра масс
    double distanceX = 1.2;  // Дистанция от продольной оси до центра масс
    double distanceY = 0.9;  // Дистанция от поперечной оси до центра масс
    void calculateAccelerations(double rollAngle, double pitchAngle, double yawAngle, double velocity);
    void calculateAngularAccelerations(double velocity, double accelerationX, double accelerationY, double accelerationZ);
    void GetDriverMotionFeedbackDataPacket(QDataStream &_stream);
    void SendDriverMotionStateDataPacket();
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};

#endif // DRIVERMOTION_TCP_CLIENT_H
