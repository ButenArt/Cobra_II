#ifndef MOTIONSYSTEM_H
#define MOTIONSYSTEM_H

#include <QObject>
#include <QQueue>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct
{
    quint32 MagicNumber;
    quint16 PacketType;
    quint16 DataSize;
} MotionSystemTCPHeader;
#pragma pack(pop)

typedef struct
{
    bool PacketReady;                  // готовность пакета
    double matmodelZ;                  // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
} MotionSystemStateDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    double X;                          // дистанция по оси Х от стартового положения из мат модели (метры)
    double Y;                          // дистанция по оси Y от стартового положения из мат модели (метры)
    double Z;                          // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
} MotionSystemFeedbackDataPacket;


class T_MotionSystem : public QObject
{
    Q_OBJECT
public:
    bool DriverMotionSystemActive;
    bool GunnerMotionSystemActive;
    double Speed = 0;
    bool Waveshield_down = true;
    float RPM = 0;                         // обороты двигателя
    //макимальная длина очереди//
    const quint8 QueueMaxLength = 5;
    //заголовок пакетов//
    MotionSystemTCPHeader DriverPacketHeader;
    MotionSystemTCPHeader GunnerPacketHeader;
    //заданное положение платформы//
    MotionSystemStateDataPacket *DriverMotionState;
    QQueue<MotionSystemStateDataPacket> DriverMotionStateQueue;
    MotionSystemStateDataPacket *GunnerMotionState;
    QQueue<MotionSystemStateDataPacket> GunnerMotionStateQueue;
    //возвращенное состояние платформы//
    MotionSystemFeedbackDataPacket *DriverMotionFeedback;
    QQueue<MotionSystemFeedbackDataPacket> DriverMotionFeedbackQueue;
    MotionSystemFeedbackDataPacket *GunnerMotionFeedback;
    QQueue<MotionSystemFeedbackDataPacket> GunnerMotionFeedbackQueue;

    explicit T_MotionSystem(QObject *parent = 0);
    ~T_MotionSystem();
    virtual void Init();
    virtual int GetDriverQueueBuffer();
    virtual void ClearDriverQueueBuffer();
    virtual int GetGunnerQueueBuffer();
    virtual void ClearGunnerQueueBuffer();
    void DriverMotionStateEnqueuePacket(MotionSystemStateDataPacket packet);
    void DriverMotionStateDequeuePacket();
    void DriverMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet);
    void DriverMotionFeedbackDequeuePacket();
    void GunnerMotionStateEnqueuePacket(MotionSystemStateDataPacket packet);
    void GunnerMotionStateDequeuePacket();
    void GunnerMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet);
    void GunnerMotionFeedbackDequeuePacket();
};


#endif // MOTIONSYSTEM_H
