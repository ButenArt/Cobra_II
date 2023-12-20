#include "COBRA2_script_tcp_client.h"


T_COBRA2_TCP_Client_ScriptServer::T_COBRA2_TCP_Client_ScriptServer(T_COBRA2_ScriptServer *ScriptServer, T_COBRA2_Instructor *Instructor, TSettingsFile *ini, TLogging *log, TReplay *replay)
{
    _ScriptServer = ScriptServer;
    _Instructor = Instructor;
    _ini = ini;
    _log = log;
    _replay = replay;

    TxInterval = _ini->LanTxInterval();
    hostaddr = _ini->IP_ScriptServer();
    tcp_port = _ini->PORT_ScriptServer();

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

T_COBRA2_TCP_Client_ScriptServer::~T_COBRA2_TCP_Client_ScriptServer()
{
    TimerTCPSend->stop();
}

void T_COBRA2_TCP_Client_ScriptServer::slotReadTcpData()
{
    ReadCommonTcpData();
}

void T_COBRA2_TCP_Client_ScriptServer::slotSendTcpData()
{
    SendCommonTcpData();
}
