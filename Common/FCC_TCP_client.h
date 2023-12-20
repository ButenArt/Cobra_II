#ifndef FCC_TCP_CLIENT_H
#define FCC_TCP_CLIENT_H

#include <QDataStream>
#include "TCP_client.h"
#include "FCC.h"
#include "Imager.h"
#include "Instructor.h"
#include "Logging.h"
#include "Replay.h"

class T_TCP_Client_FCC : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_FCC();
    ~T_TCP_Client_FCC();

protected:
    T_FCC *_FCC;
    T_Imager *_Imager;
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

#endif // FCC_TCP_CLIENT_H
