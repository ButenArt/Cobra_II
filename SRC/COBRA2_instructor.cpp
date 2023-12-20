#include "COBRA2_instructor.h"

T_COBRA2_Instructor::T_COBRA2_Instructor()
{
    ClearQueueBuffer();
}

T_COBRA2_Instructor::~T_COBRA2_Instructor()
{

}

void T_COBRA2_Instructor::Init()
{
    ClearQueueBuffer();
    T_Instructor::Init();
}

int T_COBRA2_Instructor::GetQueueBuffer()
{
    int buffer = T_Instructor::GetQueueBuffer();
    return buffer;
}

void T_COBRA2_Instructor::ClearQueueBuffer()
{
    T_Instructor::ClearQueueBuffer();
}
