#ifndef SCRIPT_TCP_CLIENT_H
#define SCRIPT_TCP_CLIENT_H

#include <QDataStream>
#include "TCP_client.h"
#include "Script.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"


class T_TCP_Client_ScriptServer : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_ScriptServer();
    ~T_TCP_Client_ScriptServer();
protected:
    T_ScriptServer *_ScriptServer;
    T_Instructor *_Instructor;
    TLogging *_log;
    TReplay *_replay;
    void SendControlDataPacket();
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void slotError();
    void slotConnected();
    void slotDisconnected();
};


#endif // SCRIPT_TCP_CLIENT_H
