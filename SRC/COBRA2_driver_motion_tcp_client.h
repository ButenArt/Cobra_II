#ifndef COBRA2_DRIVER_MOTION_TCP_CLIENT_H
#define COBRA2_DRIVER_MOTION_TCP_CLIENT_H

#include "Common/DriverMotion_TCP_client.h"
#include "COBRA2_motionsystem.h"
#include "COBRA2_instructor.h"

class T_COBRA2_TCP_Client_DriverMotionSystem: public T_TCP_Client_DriverMotionSystem
{
Q_OBJECT

public:
    T_COBRA2_TCP_Client_DriverMotionSystem(T_COBRA2_MotionSystem *MotionSystem, T_COBRA2_Instructor *Instructor, TSettingsFile *ini, TLogging *log, TReplay *replay);
    ~T_COBRA2_TCP_Client_DriverMotionSystem();

private slots:
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // COBRA2_DRIVER_MOTION_TCP_CLIENT_H
