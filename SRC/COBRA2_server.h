#ifndef COBRA2_SERVER_H
#define COBRA2_SERVER_H

#include "Common/Server.h"

class T_COBRA2_Server : public T_Server
{
    Q_OBJECT
public:
    T_COBRA2_Server();
    ~T_COBRA2_Server();
    void Init();                       // инициализация параметров

};

#endif // COBRA2_SERVER_H
