#include "COBRA2_driver_motion_tcp_client.h"

T_COBRA2_TCP_Client_DriverMotionSystem::T_COBRA2_TCP_Client_DriverMotionSystem(T_COBRA2_MotionSystem *MotionSystem, T_COBRA2_Instructor *Instructor, TSettingsFile *ini, TLogging *log, TReplay *replay)
{
    _MotionSystem = MotionSystem;
    _Instructor = Instructor;
    _ini = ini;
    _log = log;
    _replay = replay;

    TxInterval = _ini->LanTxInterval();
    hostaddr = _ini->IP_MotionSystemDriver();
    tcp_port = _ini->PORT_MotionSystemDriver();

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

T_COBRA2_TCP_Client_DriverMotionSystem::~T_COBRA2_TCP_Client_DriverMotionSystem()
{
    TimerTCPSend->stop();
}

void T_COBRA2_TCP_Client_DriverMotionSystem::slotReadTcpData()
{
    ReadCommonTcpData();
}

void T_COBRA2_TCP_Client_DriverMotionSystem::slotSendTcpData()
{
    SendCommonTcpData();
}
