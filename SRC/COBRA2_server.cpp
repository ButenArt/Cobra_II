#include "COBRA2_server.h"

T_COBRA2_Server::T_COBRA2_Server()
{

}

T_COBRA2_Server::~T_COBRA2_Server()
{

}

void T_COBRA2_Server::Init()
{
    T_Server::Init();
    ServerReady->simulatorType = SimulatorTypes::COBRA2; // тип техники
    ServerReady->ServerReady = true;                           // Готов
    ServerReady->PacketReady = true;
}
