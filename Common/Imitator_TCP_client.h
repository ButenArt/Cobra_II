#ifndef IMITATOR_TCP_CLIENT_H
#define IMITATOR_TCP_CLIENT_H

#include <QDataStream>
#include "TCP_client.h"
#include "Imitator.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"


class T_TCP_Client_ControlsImitator : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_ControlsImitator();
    ~T_TCP_Client_ControlsImitator();
protected:
    T_ControlsImitator *_ControlsImitator;
    T_Instructor *_Instructor;
    TLogging *_log;
    TReplay *_replay;
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};


#endif // IMITATOR_TCP_CLIENT_H
