#ifndef COBRA2_INSTRUCTOR_H
#define COBRA2_INSTRUCTOR_H

#include "Common/Instructor.h"


class T_COBRA2_Instructor : public T_Instructor
{
    Q_OBJECT

public:
    T_COBRA2_Instructor();
    ~T_COBRA2_Instructor();
    void Init();                       // инициализация параметров
    int GetQueueBuffer();
    void ClearQueueBuffer();
};

#endif // COBRA2_INSTRUCTOR_H
