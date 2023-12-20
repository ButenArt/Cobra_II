#include "Imager.h"

T_Imager::T_Imager(QObject *parent) : QObject(parent)
{
    ImagerActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;
/*
    wheel.wheelSpeed = 0;                  // Значение скорости колеса (км/ч)
*/
    ImagerState = new ImagerStateDataPacket();
    ImagerState->PacketReady = false;
    ImagerState->Z = 0;                    // координата X (метры)
    ImagerState->X = 0;                    // координата Y (метры)
    ImagerState->Y = 0;                    // высота (метры)
    ImagerState->Roll = 0;                 // крен (градусы -360 .. 360)
    ImagerState->Pitch = 0;                // тангаж (градусы -80 .. 80)
    ImagerState->Course = 0;               // курс (градусы 0 .. 359)
    ImagerState->Sail = false;             // плывем
    ImagerState->Surface = 0;              // поверхность
    ImagerState->Speed = 0;                // скорость
    ImagerState->WheelsCount = 0;          // количество колес
    ImagerState->WheelsList.clear();       // список параметров колес
    ImagerState->EngineTorque = 0;         // противомомент двигателя
    ImagerState->AmmoLoaded = false;

    Ballistic = new BallisticDataPacket();
    Ballistic->PacketReady = false;
    Ballistic->ObjectiveUID = 0;           // UID цели
    Ballistic->TargetUID = 0;              // UID мишени
    Ballistic->ObjectiveType = 0;          // Тип цели
    Ballistic->Hit = false;                // Попадание
    Ballistic->Radius = 0;                 // Радиус попадания
    Ballistic->GunType = 0;                // Тип оружия
    Ballistic->TargetState = false;        // Мишень опущена/поднята
    Ballistic->TriggerID = 0;              // ID Триггера
    Ballistic->Direction = 0;              // Направление
    Ballistic->Hit_X = 0;                  // Координаты пробоины по X (в % от левого края)
    Ballistic->Hit_Y = 0;                  // Координаты пробоины по Y (в % от верхнего края)
    Ballistic->ammoAmountMG = 0;

    ImagerReady = new ImagerReadyDataPacket();
    ImagerReady->PacketReady = false;
    ImagerReady->simulatorType = SimulatorTypes::Unknown; // тип техники
    ImagerReady->ImagerReady = false;      // Готов

    Triggers = new TriggerDataPacket();
    Triggers->PacketReady = false;
    Triggers->ObstacleUID = 0;             // UID препятствия
    Triggers->ObstacleType = 0;            // Тип препятствия
    Triggers->ObstacleIn = false;          // Въезд на препятствие
    Triggers->ObstacleOut = false;         // Выезд из препятствия
    Triggers->Trigger = 0;                 // Триггер

    TargetsControl = new TargetsControlDataPacket();
    TargetsControl->PacketReady = false;
    TargetsControl->mapTargetsCount = 0;   // количество мишеней
    TargetsControl->targetControlList.clear();
/*
    trajectories.WeaponID = 0;             // номер оружия
    trajectories.WeaponType = 0;           // тип оружия
    trajectories.ProjectileType = 0;       // тип пули
    trajectories.ProjectileSubType = 0;    // подтип пули
    trajectories.InitialVelocity = 0;      // начальная скорость пули
    trajectories.ShotID = 0;               // Номер выстрела
    trajectories.time = 0;                 // Время
    trajectories.pos_offset = 0;           // Смещение
    trajectories.pos_height = 0;           // Высота
    trajectories.pos_distance = 0;         // Дистанция
*/
    Projectile = new ProjectileDataPacket();
    Projectile->PacketReady = false;
    Projectile->TrajectoriesCount = 0;     // количество наборов траекторий
    Projectile->TrajectoriesList.clear();  // Наборы траекторий (структура отрезка траектории снаряда)
/*
    elements.ElementType = 0;              // Элемент тренажера
    elements.ElementState = 0;             // Неисправность элемента тренажера
    elements.ElementNameSize = 0;          // Длина названия
    elements.ElementName = "";             // Название
    elements.CommentSize = 0;              // Длина описания
    elements.Comment = "";                 // Описание
*/
    ImagerElementsState = new ImagerElementsStateDataPacket();
    ImagerElementsState->PacketReady = false;        // готовность пакета
    ImagerElementsState->ElementsCount = 0;          // Число структур теста функционирования
    ImagerElementsState->ElementsStateList.clear();  // Структура теста функционирования
    ImagerElementsState->OfflineCounter = 0;         // Счетчик отсутствия соединения
    
    ScoreDriving = new ScoreDrivingDataPacket();
    ScoreDrivingTruck = new ScoreDrivingTruckDataPacket();
    ScoreShooting = new ScoreShootingDataPacket();
}

T_Imager::~T_Imager()
{
    delete ImagerState;
    delete Ballistic;
    delete ImagerReady;
    delete Triggers;
    delete TargetsControl;
    delete Projectile;
    delete ImagerElementsState;
}

void T_Imager::Init()
{
    ImagerActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;
/*
    wheel.wheelSpeed = 0;                  // Значение скорости колеса (км/ч)
*/
    ImagerState->PacketReady = false;
    ImagerState->Z = 0;                    // координата X (метры)
    ImagerState->X = 0;                    // координата Y (метры)
    ImagerState->Y = 0;                    // высота (метры)
    ImagerState->Roll = 0;                 // крен (градусы -360 .. 360)
    ImagerState->Pitch = 0;                // тангаж (градусы -80 .. 80)
    ImagerState->Course = 0;               // курс (градусы 0 .. 359)
    ImagerState->Sail = false;             // плывем
    ImagerState->Surface = 0;              // поверхность
    ImagerState->Speed = 0;                // скорость
    ImagerState->WheelsCount = 0;          // количество колес
    ImagerState->WheelsList.clear();       // список параметров колес
    ImagerState->EngineTorque = 0;         // противомомент двигателя
    ImagerState->AmmoLoaded = false;

    Ballistic->PacketReady = false;
    Ballistic->ObjectiveUID = 0;           // UID цели
    Ballistic->TargetUID = 0;              // UID мишени
    Ballistic->ObjectiveType = 0;          // Тип цели
    Ballistic->Hit = false;                // Попадание
    Ballistic->Radius = 0;                 // Радиус попадания
    Ballistic->GunType = 0;                // Тип оружия
    Ballistic->TargetState = false;        // Мишень опущена/поднята
    Ballistic->TriggerID = 0;              // ID Триггера
    Ballistic->Direction = 0;              // Направление
    Ballistic->Hit_X = 0;                  // Координаты пробоины по X (в % от левого края)
    Ballistic->Hit_Y = 0;                  // Координаты пробоины по Y (в % от верхнего края)
    Ballistic->ammoAmountMG = 0;

    ImagerReady->PacketReady = false;
    ImagerReady->simulatorType = SimulatorTypes::Unknown; // тип техники
    ImagerReady->ImagerReady = false;      // Готов

    Triggers->PacketReady = false;
    Triggers->ObstacleUID = 0;             // UID препятствия
    Triggers->ObstacleType = 0;            // Тип препятствия
    Triggers->ObstacleIn = false;          // Въезд на препятствие
    Triggers->ObstacleOut = false;         // Выезд из препятствия
    Triggers->Trigger = 0;                 // Триггер

    TargetsControl->PacketReady = false;
    TargetsControl->mapTargetsCount = 0;
    TargetsControl->targetControlList.clear();
/*
    trajectories.WeaponID = 0;             // номер оружия
    trajectories.WeaponType = 0;           // тип оружия
    trajectories.ProjectileType = 0;       // тип пули
    trajectories.ProjectileSubType = 0;    // подтип пули
    trajectories.InitialVelocity = 0;      // начальная скорость пули
    trajectories.ShotID = 0;               // Номер выстрела
    trajectories.time = 0;                 // Время
    trajectories.pos_offset = 0;           // Смещение
    trajectories.pos_height = 0;           // Высота
    trajectories.pos_distance = 0;         // Дистанция
*/
    Projectile->PacketReady = false;
    Projectile->TrajectoriesCount = 0;     // количество наборов траекторий
    Projectile->TrajectoriesList.clear();  // Наборы траекторий (структура отрезка траектории снаряда)
/*
    elements.ElementType = 0;              // Элемент тренажера
    elements.ElementState = 0;             // Неисправность элемента тренажера
    elements.ElementNameSize = 0;          // Длина названия
    elements.ElementName = "";             // Название
    elements.CommentSize = 0;              // Длина описания
    elements.Comment = "";                 // Описание
*/
    ImagerElementsState->PacketReady = false;        // готовность пакета
    ImagerElementsState->ElementsCount = 0;          // Число структур теста функционирования
    ImagerElementsState->ElementsStateList.clear();  // Структура теста функционирования
    ImagerElementsState->OfflineCounter = 0;         // Счетчик отсутствия соединения
}

int T_Imager::GetQueueBuffer()
{
    int buffer = 0;
    buffer = ImagerStateQueue.count() +
             BallisticQueue.count() +
             ImagerReadyQueue.count() +
             TriggersQueue.count() +
             TargetsControlQueue.count() +
             ProjectileQueue.count() +
             ImagerElementsStateQueue.count();
    return buffer;
}

void T_Imager::ClearQueueBuffer()
{
    ImagerStateQueue.clear();
    BallisticQueue.clear();
    ImagerReadyQueue.clear();
    TriggersQueue.clear();
    TargetsControlQueue.clear();
    ProjectileQueue.clear();
    ImagerElementsStateQueue.clear();
}

void T_Imager::ImagerStateEnqueuePacket(ImagerStateDataPacket packet)
{
    ImagerStateQueue.enqueue(packet);
    if(ImagerStateQueue.count() > QueueMaxLength)
    {
        ImagerStateDequeuePacket();
    }
}

void T_Imager::ImagerStateDequeuePacket()
{
    if(!ImagerStateQueue.isEmpty()) *ImagerState = ImagerStateQueue.dequeue();
}

void T_Imager::BallisticEnqueuePacket(BallisticDataPacket packet)
{
    BallisticQueue.enqueue(packet);
    if(BallisticQueue.count() > QueueMaxLength)
    {
        BallisticDequeuePacket();
    }
}

void T_Imager::BallisticDequeuePacket()
{
    if(!BallisticQueue.isEmpty()) *Ballistic = BallisticQueue.dequeue();
}

void T_Imager::ImagerReadyEnqueuePacket(ImagerReadyDataPacket packet)
{
    ImagerReadyQueue.enqueue(packet);
    if(ImagerReadyQueue.count() > QueueMaxLength)
    {
        ImagerReadyDequeuePacket();
    }
}

void T_Imager::ImagerReadyDequeuePacket()
{
    if(!ImagerReadyQueue.isEmpty()) *ImagerReady = ImagerReadyQueue.dequeue();
}

void T_Imager::TriggersEnqueuePacket(TriggerDataPacket packet)
{
    TriggersQueue.enqueue(packet);
    if(TriggersQueue.count() > QueueMaxLength)
    {
        TriggersDequeuePacket();
    }
}

void T_Imager::TriggersDequeuePacket()
{
    if(!TriggersQueue.isEmpty()) *Triggers = TriggersQueue.dequeue();
}

void T_Imager::TargetsControlEnqueuePacket(TargetsControlDataPacket packet)
{
    TargetsControlQueue.enqueue(packet);
    if(TargetsControlQueue.count() > QueueMaxLength)
    {
        TargetsControlDequeuePacket();
    }
}

void T_Imager::TargetsControlDequeuePacket()
{
    if(!TargetsControlQueue.isEmpty()) *TargetsControl = TargetsControlQueue.dequeue();
}

void T_Imager::ProjectileEnqueuePacket(ProjectileDataPacket packet)
{
    ProjectileQueue.enqueue(packet);
    if(ProjectileQueue.count() > QueueMaxLength)
    {
        ProjectileDequeuePacket();
    }
}

void T_Imager::ProjectileDequeuePacket()
{
    if(!ProjectileQueue.isEmpty()) *Projectile = ProjectileQueue.dequeue();
}

void T_Imager::ImagerElementsStateEnqueuePacket(ImagerElementsStateDataPacket packet)
{
    ImagerElementsStateQueue.enqueue(packet);
    if(ImagerElementsStateQueue.count() > QueueMaxLength)
    {
        ImagerElementsStateDequeuePacket();
    }
}

void T_Imager::ImagerElementsStateDequeuePacket()
{
    if(!ImagerElementsStateQueue.isEmpty()) *ImagerElementsState = ImagerElementsStateQueue.dequeue();
}

void T_Imager::ScoreDrivingEnqueuePacket( ScoreDrivingDataPacket packet)
{
    ScoreDrivingQueue.enqueue(packet);
    if( ScoreDrivingQueue.count() > QueueMaxLength)
    {
        ScoreDrivingDequeuePacket();
    }
}

void T_Imager::ScoreDrivingDequeuePacket()
{
    if(!ScoreDrivingQueue.isEmpty()) *ScoreDriving = ScoreDrivingQueue.dequeue();
}

void T_Imager::ScoreDrivingTruckEnqueuePacket( ScoreDrivingTruckDataPacket packet)
{
    ScoreDrivingTruckQueue.enqueue(packet);
    if( ScoreDrivingTruckQueue.count() > QueueMaxLength)
    {
        ScoreDrivingTruckDequeuePacket();
    }
}

void T_Imager::ScoreDrivingTruckDequeuePacket()
{
    if(!ScoreDrivingTruckQueue.isEmpty()) *ScoreDrivingTruck = ScoreDrivingTruckQueue.dequeue();
}

void T_Imager::ScoreShootingEnqueuePacket( ScoreShootingDataPacket packet)
{
    ScoreShootingQueue.enqueue(packet);
    if( ScoreShootingQueue.count() > QueueMaxLength)
    {
        ScoreShootingDequeuePacket();
    }
}

void T_Imager::ScoreShootingDequeuePacket()
{
    if(!ScoreShootingQueue.isEmpty()) *ScoreShooting = ScoreShootingQueue.dequeue();
}

QDebug operator<<( QDebug out, const Elements& ep )
{
    out << "ElementType = " << ep.ElementType
        << ", ElementState = " << ep.ElementState
        << ", ElementNameSize = " << ep.ElementNameSize
        << ", ElementName = " << ep.ElementName
        << ", CommentSize = " << ep.CommentSize
        << ", Comment = " << ep.Comment
        ;
    return out;
}
QDebug operator<<( QDebug out, const ImagerElementsStateDataPacket& ep )
{
    out << "PacketReady =" << ep.PacketReady
        << ", ElementsCount =" << ep.ElementsCount
        << ", ElementsStateList =" << ep.ElementsStateList
        << ", OfflineCounter =" << ep.OfflineCounter
        ;
    return out;
}
