#ifndef SERVICEERRORS_H
#define SERVICEERRORS_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <Windows.h>


const BYTE N_ERRORS = 32;
const BYTE N_FAULTS = 32;

#pragma pack(push, 1)
typedef struct
{
    QString Name[N_ERRORS];     // название ошибки
    bool Active[N_ERRORS];      // состояние ошибки
    WORD DeltaT[N_ERRORS];      // время в секундах в течении которого ошибка не фиксируется
    QTime Time[N_ERRORS];       // время фиксации ошибки
    bool Send[N_ERRORS];        // отправлять сообщение об ошибке
} TErrors, *PTErrors;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    QString Name[N_FAULTS];     // название отказа
    bool Active[N_FAULTS];      // активность неисправности
    QTime Time[N_FAULTS];       // время введенния руководителем неисправностей
    WORD DeltaT[N_FAULTS];      // время в секундах действия неисправностей
} TFaultInjection, *PTFaultInjection;
#pragma pack(pop)

class T_ServiceErrors : public QObject
{

  Q_OBJECT

  public:
    T_ServiceErrors();
    ~T_ServiceErrors();
    TErrors Errors;
    TFaultInjection FaultInjection;
    void Init();
  protected:
    QTimer *ErrorsTimer;

};

#endif // SERVICEERRORS_H
