#ifndef TCP_CLIENT_INSTRUCTOR_H
#define TCP_CLIENT_INSTRUCTOR_H

#include <QDataStream>
#include "TCP_client.h"
#include "server.h"
#include "Imager.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"


class T_TCP_Client_Instructor : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_Instructor();
    ~T_TCP_Client_Instructor();
protected:
    T_Server *_Server;
    T_Imager *_Imager;
    T_Instructor *_Instructor;
    TLogging *_log;
    TReplay *_replay;
    bool processedHeader = false;
    void GetExerciseParametersPacket(QDataStream &_stream);
    void GetObjectsDataPacket(QDataStream &_stream);
    void GetTargetsDataPacket(QDataStream &_stream);
    void GetEnvironmentDataPacket(QDataStream &_stream);
    void GetAmmunitionDataPacket(QDataStream &_stream);
    void GetControlDataPacket(QDataStream &_stream);
    void GetFailuresDataPacket(QDataStream &_stream);
    void GetCommandsDataPacket(QDataStream &_stream);
    void GetMessagesDataPacket(QDataStream &_stream);
    void GetScoreRequestDataPacket(QDataStream &_stream);
    void GetStatisticsDataPacket(QDataStream &_stream);
    void SendBallisticDataPacket();
    void SendTriggerDataPacket();
    void SendImagerReadyDataPacket();
    void SendServerReadyDataPacket();
    void SendScoreDrivingDataPacket( );
    void SendScoreShootingDataPacket( );
    void SendScoreDrivingTruckDataPacket( );
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};


#endif // TCP_CLIENT_INSTRUCTOR_H
