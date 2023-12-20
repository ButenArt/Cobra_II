#include "COBRA2_manager.h"

T_COBRA2_manager::T_COBRA2_manager()
{
    ini = new TSettingsFile();
    log = new TLogging(ini);
    log->WriteLog(LogEvents::Application_start,"");
    replay = new TReplay(ini, log);
    replay->ClearReplay();
    replay->start();
    server = new T_COBRA2_Server();
    server->Init();
    can = new T_COBRE2_CAN(ini, log);
    if(can->BoardCount() > 0)
    {
      can->setTXEnabled(true);
      can->AutoTXEnable();
      can->setRXEnabled(true);
      can->AutoRXEnable();
      can->HeartBeatEnable();
    }
    imager = new T_COBRA2_Imager();
    instructor = new T_COBRA2_Instructor();
    scriptserver = new T_COBRA2_ScriptServer();
    motionsystem = new T_COBRA2_MotionSystem();
    controlsimitator = new T_COBRA2_ControlsImitator();
    driver = new T_COBRA2_driver();
    engine = new COBRA2_Engine(instructor->Environment->temperature);
    transmission = new COBRA2_Transmission();
    move = new COBRA2_Move(engine, transmission);
    errors = new T_COBRA2_ServiceErrors(instructor, driver, move);

    dpdll = new T_DPDLL(server, imager, instructor);
    dpdll->Init_driver();
    dpdll->SetPlatformDown_driver();
    if(ini->DPDLL_Driver_Enable()) dpdll->SetPlatformData_driver();

    controls = new T_COBRA2_controls(can, server, imager, instructor, driver, move, ini, replay, errors, motionsystem, dpdll);

    tcpclient_imager = new T_COBRA2_TCP_Client_Imager(server, motionsystem, imager, instructor, driver, ini, log, replay);
    tcpclient_instructor = new T_COBRA2_TCP_Client_Instructor(server, imager, instructor, driver, ini, log, replay);
    tcpclient_scriptserver = new T_COBRA2_TCP_Client_ScriptServer(scriptserver, instructor, ini, log, replay);
    tcpclient_controlsimitator = new T_COBRA2_TCP_Client_ControlsImitator(controlsimitator, instructor, driver, ini, log, replay);
    tcpclient_driver_motionsystem = new T_COBRA2_TCP_Client_DriverMotionSystem(motionsystem, instructor, ini, log, replay);

}

T_COBRA2_manager::~T_COBRA2_manager()
{
    delete dpdll;
    delete tcpclient_driver_motionsystem;
    delete tcpclient_controlsimitator;
    delete tcpclient_scriptserver;
    delete tcpclient_instructor;
    delete tcpclient_imager;
    delete controls;
    delete errors;
    delete move;
    delete engine;
    delete transmission;
    delete controlsimitator;
    delete driver;
    delete motionsystem;
    delete scriptserver;
    delete instructor;
    delete imager;
    delete can;
    delete server;
    delete replay;
    delete log;
    delete ini;
}
