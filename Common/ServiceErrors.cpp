#include "ServiceErrors.h"

T_ServiceErrors::T_ServiceErrors()
{
    ErrorsTimer = new QTimer(this);
    ErrorsTimer->setInterval(100);
    ErrorsTimer->stop();

    for(int i=0; i<N_ERRORS; i++)
    {
        Errors.Name[i] = "UNDEFINED";
        Errors.Time[i] = QTime::currentTime();
        Errors.Active[i] = false;
        Errors.Send[i] = false;
    }

    for(int i=0; i<N_FAULTS; i++)
    {
        FaultInjection.Name[i] = "UNDEFINED";
        FaultInjection.Active[i] = false;
        FaultInjection.Time[i] = QTime::currentTime();
        FaultInjection.DeltaT[i] = 0;
    }
}

T_ServiceErrors::~T_ServiceErrors()
{
    delete ErrorsTimer;
}

void T_ServiceErrors::Init()
{
    for(int i=0; i<N_ERRORS; i++)
    {
       Errors.Time[i] = QTime::currentTime();
       Errors.Active[i] = false;
       Errors.Send[i] = false;
    }

    for(int i=0; i<N_FAULTS; i++)
    {
        FaultInjection.Active[i] = false;
        FaultInjection.Time[i] = QTime::currentTime();
        FaultInjection.DeltaT[i] = 0;
    }
}
