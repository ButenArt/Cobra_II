#ifndef COBRA2_TCP_CLIENT_IMAGER_H
#define COBRA2_TCP_CLIENT_IMAGER_H

#include "Common/Imager_TCP_client.h"
#include "COBRA2.h"
#include "COBRA2_server.h"
#include "COBRA2_motionsystem.h"
#include "COBRA2_imager.h"
#include "COBRA2_instructor.h"
#include "Common/SettingsFile.h"

class T_COBRA2_TCP_Client_Imager : public T_TCP_Client_Imager
{
Q_OBJECT

public:
    T_COBRA2_TCP_Client_Imager(T_COBRA2_Server *Server, T_COBRA2_MotionSystem *MotionSystem, T_COBRA2_Imager *Imager, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, TSettingsFile *ini, TLogging *log, TReplay *replay);
    ~T_COBRA2_TCP_Client_Imager();
private:
    T_COBRA2_driver *_driver;
    TSettingsFile *_ini;
    void ReadTcpData();
    void SendDriverStateDataPacket();

private slots:
    void slotReadTcpData();
    void slotSendTcpData();
};


#endif // COBRA2_TCP_CLIENT_IMAGER_H
