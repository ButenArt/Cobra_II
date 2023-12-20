#ifndef COBRA2_SCRIPT_H
#define COBRA2_SCRIPT_H

#include "Common/Script.h"

class T_COBRA2_ScriptServer : public T_ScriptServer
{
    Q_OBJECT
public:
    T_COBRA2_ScriptServer();
    ~T_COBRA2_ScriptServer();
    void Init();                       // инициализация параметров
};

#endif // COBRA2_SCRIPT_H
