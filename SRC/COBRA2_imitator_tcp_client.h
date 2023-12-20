#ifndef COBRA2_IMITATOR_TCP_CLIENT_H
#define COBRA2_IMITATOR_TCP_CLIENT_H

#include "Common/Imitator_TCP_client.h"
#include "COBRA2.h"
#include "COBRA2_imitator.h"
#include "COBRA2_instructor.h"
#include "Common/SettingsFile.h"

class T_COBRA2_TCP_Client_ControlsImitator : public T_TCP_Client_ControlsImitator
{
Q_OBJECT

public:
    T_COBRA2_TCP_Client_ControlsImitator(T_COBRA2_ControlsImitator *ControlsImitator, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, TSettingsFile *ini, TLogging *log, TReplay *replay);
    ~T_COBRA2_TCP_Client_ControlsImitator();
private:
    T_COBRA2_driver *_driver;
    TSettingsFile *_ini;
    void SendControlsDataPacket();

private slots:
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // COBRA2_IMITATOR_TCP_CLIENT_H
