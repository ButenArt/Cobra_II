#include "FCC_TCP_client.h"

T_TCP_Client_FCC::T_TCP_Client_FCC()
{

}

T_TCP_Client_FCC::~T_TCP_Client_FCC()
{

}

void T_TCP_Client_FCC::slotConnected()
{
    _FCC->FCC_Active = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_FCC_connected, "");
}

void T_TCP_Client_FCC::slotDisconnected()
{
    _FCC->FCC_Active = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_FCC_disconnected, "");
}

void T_TCP_Client_FCC::slotError()
{
    _FCC->FCC_Active = ClientConnected();
    _log->WriteLANLog(LogEvents::TCP_FCC_Error, socketErrorString);
}

void T_TCP_Client_FCC::SendCommonTcpData()
{

}

void T_TCP_Client_FCC::ReadCommonTcpData()
{

}
