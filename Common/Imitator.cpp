#include "Imitator.h"

T_ControlsImitator::T_ControlsImitator(QObject *parent) : QObject(parent)
{
    ControlsImitatorActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;

}

T_ControlsImitator::~T_ControlsImitator()
{

}

void T_ControlsImitator::Init()
{
    ControlsImitatorActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;
}
