#ifndef TCP_CLIENT_IMAGER_H
#define TCP_CLIENT_IMAGER_H

#include <QDataStream>
#include "TCP_client.h"
#include "Imager.h"
#include "Instructor.h"
#include "MotionSystem.h"
#include "Logging.h"
#include "Replay.h"

class T_TCP_Client_Imager : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_Imager();
    ~T_TCP_Client_Imager();

protected:
    T_Server *_Server;
    T_Imager *_Imager;
    T_Instructor *_Instructor;
    T_MotionSystem *_MotionSystem;
    TLogging *_log;
    TReplay *_replay;
    void GetImagerStateDataPacket(QDataStream &_stream);
    void GetBallisticDataPacket(QDataStream &_stream);
    void GetImagerReadyDataPacket(QDataStream &_stream);
    void GetTriggerDataPacket(QDataStream &_stream);
    void GetImagerElementsStateDataPacket(QDataStream &_stream);
    void GetScoreDrivingDataPacket(QDataStream &_stream);
    void GetScoreShootingDataPacket(QDataStream &_stream);
    void GetScoreDrivingTruckDataPacket(QDataStream &_stream);
    void SendExerciseParametersPacket();
    void SendObjectsDataPacket();
    void SendTargetsDataPacket();
    void SendEnvironmentDataPacket();
    void SendCommandsDataPacket();
    void SendMessagesDataPacket();
    void SendTargetsControlDataPacket();
    void SendProjectileDataPacket();
    void SendWeaponStateDataPacket();
    void SendScoreRequestDataPacket();
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};

#endif // TCP_CLIENT_IMAGER_H
