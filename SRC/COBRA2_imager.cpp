#include "COBRA2_imager.h"

T_COBRA2_Imager::T_COBRA2_Imager()
{
    ClearQueueBuffer();
}

T_COBRA2_Imager::~T_COBRA2_Imager()
{

}

void T_COBRA2_Imager::Init()
{
    ClearQueueBuffer();
    T_Imager::Init();
}

int T_COBRA2_Imager::GetQueueBuffer()
{
    int buffer = 0;
    buffer = T_Imager::GetQueueBuffer();
    return buffer;
}

void T_COBRA2_Imager::ClearQueueBuffer()
{
    T_Imager::ClearQueueBuffer();
}
