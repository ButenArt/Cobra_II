#ifndef IMAGER_H
#define IMAGER_H

#include <QObject>
#include <QQueue>
#include <Windows.h>
#include "server.h"
#include "QDebug"

#pragma pack(push, 1)
typedef struct
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
} ImagerTCPHeader;
#pragma pack(pop)

typedef struct
{
    float wheelSpeed;                  // Значение скорости колеса (км/ч). Последовательно с переда на зад справа налево.
} Wheels;

typedef struct
{
    qint32 WeaponID;                   // номер оружия
    qint32 WeaponType;                 // тип оружия
    qint32 ProjectileType;             // тип пули
    qint32 ProjectileSubType;          // подтип пули
    float InitialVelocity;             // начальная скорость пули
    qint32 ShotID;                     // Номер выстрела
    float time;                        // Время
    float pos_offset;                  // Смещение
    float pos_height;                  // Высота
    float pos_distance;                // Дистанция
    float rise_angle;                  // Угол вылета снаряда
} Trajectories;

typedef struct
{
    qint32 ObjectiveUID;               // UID цели
    qint32 TargetUID;                  // UID мишени
    bool TargetState;                  // Мишень опущена/поднята
} TargetControl;


typedef struct
{
    bool PacketReady;                  // готовность пакета
    float X;                           // координата X (метры)
    float Y;                           // координата Y (метры)
    float Z;                           // высота (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
    bool Sail;                         // плывем
    quint8 Surface;                    // поверхность
    float Speed;                       // скорость
    qint32 WheelsCount;                // количество колес
    QList<Wheels> WheelsList;          // список параметров колес
    float EngineTorque;                // противомомент двигателя
    float RPM;                         // обороты двигателя
    bool AmmoLoaded;                   // Боеприпасы загружены
} ImagerStateDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 ObjectiveUID;               // UID цели
    qint32 TargetUID;                  // UID мишени
    quint8 ObjectiveType;              // Тип цели
    bool Hit;                          // Попадание
    float Radius;                      // Радиус попадания
    quint8 GunType;                    // Тип оружия
    bool TargetState;                  // Мишень опущена/поднята
    quint8 TriggerID;                  // ID Триггера
    quint8 Direction;                  // Направление
    quint8 Hit_X;                      // Координаты пробоины по X (в % от левого края)
    quint8 Hit_Y;                      // Координаты пробоины по Y (в % от верхнего края)
    qint32 ammoAmountMG;
} BallisticDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    SimulatorTypes simulatorType;      // тип техники
    bool ImagerReady;                  // Готов
} ImagerReadyDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 ObstacleUID;                // UID препятствия
    quint8 ObstacleType;               // Тип препятствия
    bool ObstacleIn;                   // Въезд на препятствие
    bool ObstacleOut;                  // Выезд из препятствия
    quint8 Trigger;                    // Триггер
} TriggerDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 mapTargetsCount;            // Число мишеней
    QList<TargetControl> targetControlList;
} TargetsControlDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 TrajectoriesCount;          // количество наборов траекторий
    QList<Trajectories> TrajectoriesList; // Наборы траекторий (структура отрезка траектории снаряда)
} ProjectileDataPacket;

struct ImagerElementsStateDataPacket
{
    bool PacketReady;                  // готовность пакета
    qint32 ElementsCount;              // Число структур теста функционирования
    QList<Elements> ElementsStateList; // Структура теста функционирования
    quint8 OfflineCounter;             // Счетчик отсутствия соединения
};

QDebug operator<<( QDebug out, const Elements& ep );
QDebug operator<<( QDebug out, const ImagerElementsStateDataPacket& ep );

// [ ID=111 # Server >> Instructor ] Оценка подсчитана (вождение/полеты)
struct ScoreDrivingDataPacket
{
    bool PacketReady = false;                   // готовность пакета
    double Total = 0.;                       // Финальная оценка
    double Speed = 0.;                       // Оценка за скорость
    double Indicator = 0;                   // Оценка за индикаторы
    double Normativ = 0;                    // Оценка за нормативы
    double Indicator1 = 0.;                  // Индикатор 1
    double Indicator2 = 0.;                  // Индикатор 2
    double Indicator3 = 0.;                  // Индикатор 3
    double Indicator4 = 0.;                  // Индикатор 4
    double Indicator5 = 0.;                  // Индикатор 5
    double AverageSpeed = 0;                // Средняя скорость
};

// [ID = 112 # Server >> Instructor] Оценка подсчитана( стрельба )
struct ScoreShootingDataPacket
{
    bool PacketReady = false;                   // готовность пакета
    double Total = 0.;                       // Финальная оценка
    double Target1 = 0.;                     // Цель 1
    double Target2 = 0.;                     // Цель 2
    double Target3 = 0.;                     // Цель 3
    double Target4 = 0.;                     // Цель 4
};

// [ ID=113 # Server >> Instructor ] Оценка подсчитана (грузовики)
struct ScoreDrivingTruckDataPacket
{
    bool PacketReady = false;                  // готовность пакета
    double Total = 0.;                       // Финальная оценка
    int    TimeDriving = 0;                 // Время вождения, сек
    double Distance = 0.;                    // Пройденная дистанция, м
};


class T_Imager : public QObject
{
    Q_OBJECT
public:
    bool ImagerActive;
    //макимальная длина очереди//
    const quint8 QueueMaxLength = 5;
    //заголовок пакетов//
    ImagerTCPHeader PacketHeader;
    //состояние имитатора//
    //Wheels wheel;
    //struct ImagerStateDataPacket;
    ImagerStateDataPacket *ImagerState;
    QQueue<ImagerStateDataPacket> ImagerStateQueue;
    //параметры мишеней//
    //struct BallisticDataPacket;
    BallisticDataPacket *Ballistic;
    QQueue<BallisticDataPacket> BallisticQueue;
    //готов к упражнению//
    //struct ImagerReadyDataPacket;
    ImagerReadyDataPacket *ImagerReady;
    QQueue<ImagerReadyDataPacket> ImagerReadyQueue;
    //параметры препятствий//
    //struct TriggerDataPacket;
    TriggerDataPacket *Triggers;
    QQueue<TriggerDataPacket> TriggersQueue;
    //управление мишенями
    //struct TargetsControlDataPacket;
    TargetsControlDataPacket *TargetsControl;
    QQueue<TargetsControlDataPacket> TargetsControlQueue;
    //расчет траекторий боеприпасов
    //Trajectories trajectories;
    //struct ProjectileDataPacket;
    ProjectileDataPacket *Projectile;
    QQueue<ProjectileDataPacket> ProjectileQueue;
    //данные о готовности компонентов//
    //Elements elements;
    //struct ImagerElementsStateDataPacket;
    ImagerElementsStateDataPacket *ImagerElementsState;
    QQueue<ImagerElementsStateDataPacket> ImagerElementsStateQueue;

    ScoreDrivingDataPacket * ScoreDriving;
    QQueue<ScoreDrivingDataPacket> ScoreDrivingQueue;

    ScoreDrivingTruckDataPacket * ScoreDrivingTruck;
    QQueue<ScoreDrivingTruckDataPacket> ScoreDrivingTruckQueue;

    ScoreShootingDataPacket * ScoreShooting;
    QQueue<ScoreShootingDataPacket> ScoreShootingQueue;

    explicit T_Imager(QObject *parent = 0);
    ~T_Imager();
    virtual void Init();
    virtual int GetQueueBuffer();
    virtual void ClearQueueBuffer();
    void ImagerStateEnqueuePacket(ImagerStateDataPacket packet);
    void ImagerStateDequeuePacket();
    void BallisticEnqueuePacket(BallisticDataPacket packet);
    void BallisticDequeuePacket();
    void ImagerReadyEnqueuePacket(ImagerReadyDataPacket packet);
    void ImagerReadyDequeuePacket();
    void TriggersEnqueuePacket(TriggerDataPacket packet);
    void TriggersDequeuePacket();
    void TargetsControlEnqueuePacket(TargetsControlDataPacket packet);
    void TargetsControlDequeuePacket();
    void ProjectileEnqueuePacket(ProjectileDataPacket packet);
    void ProjectileDequeuePacket();
    void ImagerElementsStateEnqueuePacket(ImagerElementsStateDataPacket packet);
    void ImagerElementsStateDequeuePacket();

    void ScoreDrivingEnqueuePacket( ScoreDrivingDataPacket packet);
    void ScoreDrivingDequeuePacket();

    void ScoreDrivingTruckEnqueuePacket( ScoreDrivingTruckDataPacket packet);
    void ScoreDrivingTruckDequeuePacket();

    void ScoreShootingEnqueuePacket( ScoreShootingDataPacket packet);
    void ScoreShootingDequeuePacket();

};


#endif // IMAGER_H
