#ifndef COBRA2_MOTIONSYSTEM_H
#define COBRA2_MOTIONSYSTEM_H

#include "Common/MotionSystem.h"

class T_COBRA2_MotionSystem : public T_MotionSystem
{
    Q_OBJECT
public:
    T_COBRA2_MotionSystem();
    ~T_COBRA2_MotionSystem();
    void Init();                       // инициализация параметров
    int GetDriverQueueBuffer();
    void ClearDriverQueueBuffer();
};

#endif // COBRA2_MOTIONSYSTEM_H
