#ifndef COBRA2_IMITATOR_H
#define COBRA2_IMITATOR_H

#include "Common/Imitator.h"

class T_COBRA2_ControlsImitator : public T_ControlsImitator
{
    Q_OBJECT
public:
    T_COBRA2_ControlsImitator();
    ~T_COBRA2_ControlsImitator();
    void Init();                       // инициализация параметров
};

#endif // COBRA2_IMITATOR_H
