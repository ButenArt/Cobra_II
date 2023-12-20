#ifndef COBRA2_SERVICEERRORS_H
#define COBRA2_SERVICEERRORS_H

#include <QObject>
#include <Windows.h>
#include "Common/ServiceErrors.h"
#include "COBRA2.h"
#include "COBRA2_move.h"
#include "COBRA2_instructor.h"

//типы отказов КАМАЗ вводимые руководителем
enum COBRA2_FAULTS
{
    UNKNOWN=0,                 //неопределено
    NO_BRAKE=1,                //отказ тормозной системы
    LOW_TYRE_PRESSURE=2,       //низкое давление в шинах
    LOW_ENGINE_OIL=4,          //падение давления масла в двигателе
    NO_STEER_HYDRAULIC=5,      //поломка насоса гидроусилителя рулевого управлениия
    NO_COUPLING=6,             //отказ привода сцепления
    COOLANT_OVERHEAT=10,       //перегрев охлаждающей жидкости в двигателе
    NO_BATTERY_CHARGING=11,    //отказ зарадки аккумуляторных батарей
    NO_FUEL=12,                //окончание топлива в одном из топливных баков
    AIR_ON_FUEL_LINE=13,       //подсос воздуха в систему питания топлива
    BATTERY_SHORT=16,          //КЗ в аккумуляторе
};


class T_COBRA2_ServiceErrors : public T_ServiceErrors
{

  Q_OBJECT

  public:
    T_COBRA2_ServiceErrors(T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, COBRA2_Move *move);
  private:
    T_COBRA2_Instructor *_instructor;
    T_COBRA2_driver *_driver;
    COBRA2_Move *_move;
private slots:
  void slotCheckErrors();
};


#endif // COBRA2_SERVICEERRORS_H
