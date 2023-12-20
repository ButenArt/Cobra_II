#include "COBRA2_motionsystem.h"

T_COBRA2_MotionSystem::T_COBRA2_MotionSystem()
{
    ClearDriverQueueBuffer();
}

T_COBRA2_MotionSystem::~T_COBRA2_MotionSystem()
{

}

void T_COBRA2_MotionSystem::Init()
{
    ClearDriverQueueBuffer();
    T_MotionSystem::Init();
}

int T_COBRA2_MotionSystem::GetDriverQueueBuffer()
{
    int buffer = 0;
    buffer = T_MotionSystem::GetDriverQueueBuffer();
    return buffer;
}

void T_COBRA2_MotionSystem::ClearDriverQueueBuffer()
{
    T_MotionSystem::ClearDriverQueueBuffer();
}
