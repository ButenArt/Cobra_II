#ifndef COBRA2_IMAGER_H
#define COBRA2_IMAGER_H

#include "Common/Imager.h"

class T_COBRA2_Imager : public T_Imager
{
    Q_OBJECT
public:
    T_COBRA2_Imager();
    ~T_COBRA2_Imager();
    void Init();                       // инициализация параметров
    int GetQueueBuffer();
    void ClearQueueBuffer();
};

#endif // COBRA2_IMAGER_H
