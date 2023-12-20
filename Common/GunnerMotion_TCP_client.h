#ifndef GUNNERMOTION_TCP_CLIENT_H
#define GUNNERMOTION_TCP_CLIENT_H

#include <QDataStream>
#include "TCP_client.h"
#include "MotionSystem.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"
#include "SettingsFile.h"

class T_TCP_Client_GunnerMotionSystem : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_GunnerMotionSystem();
    ~T_TCP_Client_GunnerMotionSystem();

protected:
    T_MotionSystem *_MotionSystem;
    T_Instructor *_Instructor;
    TSettingsFile *_ini;
    TLogging *_log;
    TReplay *_replay;
    void GetGunnerMotionFeedbackDataPacket(QDataStream &_stream);
    void SendGunnerMotionStateDataPacket();
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};

#endif // GUNNERMOTION_TCP_CLIENT_H
