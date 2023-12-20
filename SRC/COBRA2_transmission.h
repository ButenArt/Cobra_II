#ifndef COBRA2_TRANSMISSION_H
#define COBRA2_TRANSMISSION_H

#include <windows.h>
#include <QObject>
#include <QVector>


class COBRA2_Transmission : public QObject
{
  Q_OBJECT

  public:
    COBRA2_Transmission();
    ~COBRA2_Transmission();
    void Transmission_Reset();
    void Transmission_SetCoupling(float coupling);
    void Transmission_SetBrake(float brake);
    void Transmission_SetDiff(bool diffLock);
    void Transmission_SetGear(int gear);
    void Transmission_SetTransferGear(int gear);
    float getCurrentGearRatio();
    float getCurrentTransferGearRatio();
    float getCurrentGearMaxVelocity();
    float getCurrentGearMinVelocity();
    struct TTransmission;
    TTransmission *Transmission;
  private:
    QVector<float> gearRatios;             //передаточные числа коробки передач
    QVector<float> gearMaxVelocities;      //максимальные скорости на передачах
    QVector<float> gearMinVelocities;      //минимальные скорости на передачах
    QVector<float> gearTransferRatios;     //передаточные числа раздаточной коробки передач (понижающая/0/повышающая)
};


#pragma pack(push, 1)
struct COBRA2_Transmission::TTransmission
{
    const float AxleRatio = 1.846f;        //передаточное число ведущего моста
    const float ReducerRatio = 2.83f;      //передаточное число колесных редукторов
    const BYTE K_FRIC = 10;                //коэф трения сцепления
    const WORD K_BRAKE = 5000;             //коэф торможения
    const float K_TRANSMISSION = 0.8f;      //КПД трансмиссии
    float HandBrake;                       //стояночный тормоз
    float Brake;                           //торможение от 0 до 1
    int Gear;                              //передача
    int TransferGear;                      //передача раздаточной коробки
    bool GearDivider;                      //делитель передач
    float Coupling;                        //сцепление мотора с муфтой от 0 до 1
    bool DiffLock;                         //блокировка межосевого дифференциала
};
#pragma pack(pop)



#endif // COBRA2_TRANSMISSION_H
