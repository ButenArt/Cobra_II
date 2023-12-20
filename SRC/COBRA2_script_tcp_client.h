#ifndef COBRA2_SCRIPT_TCP_CLIENT_H
#define COBRA2_SCRIPT_TCP_CLIENT_H

#include "Common/Script_TCP_client.h"
#include "COBRA2_script.h"
#include "COBRA2_instructor.h"
#include "Common/SettingsFile.h"

class T_COBRA2_TCP_Client_ScriptServer : public T_TCP_Client_ScriptServer
{
Q_OBJECT

public:
    T_COBRA2_TCP_Client_ScriptServer(T_COBRA2_ScriptServer *ScriptServer, T_COBRA2_Instructor *Instructor, TSettingsFile *ini, TLogging *log, TReplay *replay);
    ~T_COBRA2_TCP_Client_ScriptServer();
private:
    TSettingsFile *_ini;

private slots:
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // COBRA2_SCRIPT_TCP_CLIENT_H
