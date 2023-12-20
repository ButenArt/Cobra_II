#ifndef COBRA2_MANAGER_H
#define COBRA2_MANAGER_H

#include "COBRA2_can_data.h"
#include "COBRA2.h"
#include "COBRA2_transmission.h"
#include "COBRA2_engine.h"
#include "COBRA2_move.h"
#include "COBRA2_controls.h"
#include "COBRA2_server.h"
#include "COBRA2_imager.h"
#include "COBRA2_instructor.h"
#include "COBRA2_script.h"
#include "COBRA2_motionsystem.h"
#include "COBRA2_imager_tcp_client.h"
#include "COBRA2_instructor_tcp_client.h"
#include "COBRA2_script_tcp_client.h"
#include "COBRA2_driver_motion_tcp_client.h"
#include "COBRA2_imitator.h"
#include "COBRA2_imitator_tcp_client.h"
#include "COBRA2_serviceerrors.h"
#include "Common/SettingsFile.h"
#include "Common/Logging.h"
#include "Common/Replay.h"
#include "DPDLL/MotionSystem_DPDLL.h"


class T_COBRA2_manager : public QObject
{
Q_OBJECT

public:
    T_COBRA2_manager();
    ~T_COBRA2_manager();
    T_COBRA2_Server *server;
    T_COBRE2_CAN *can;
    T_COBRA2_ServiceErrors *errors;
    T_COBRA2_Imager *imager;
    T_COBRA2_Instructor *instructor;
    T_COBRA2_ScriptServer *scriptserver;
    T_COBRA2_MotionSystem *motionsystem;
    T_COBRA2_controls *controls;
    T_COBRA2_ControlsImitator *controlsimitator;
    T_COBRA2_driver *driver;
    COBRA2_Move *move;
    TLogging *log;
    TSettingsFile *ini;
    TReplay *replay;
    T_COBRA2_TCP_Client_Imager *tcpclient_imager;
    T_COBRA2_TCP_Client_Instructor *tcpclient_instructor;
    T_COBRA2_TCP_Client_ScriptServer *tcpclient_scriptserver;
    T_COBRA2_TCP_Client_ControlsImitator *tcpclient_controlsimitator;
    T_COBRA2_TCP_Client_DriverMotionSystem *tcpclient_driver_motionsystem;
    T_DPDLL *dpdll;
private:
    COBRA2_Engine *engine;
    COBRA2_Transmission *transmission;
};

#endif // COBRA2_MANAGER_H
