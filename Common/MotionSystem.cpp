#include "MotionSystem.h"

T_MotionSystem::T_MotionSystem(QObject *parent) : QObject(parent)
{
    DriverMotionSystemActive = false;
    GunnerMotionSystemActive = false;

    DriverPacketHeader.MagicNumber = 0x7AADF00D;
    DriverPacketHeader.PacketType = 0;
    DriverPacketHeader.DataSize = 0;
    GunnerPacketHeader.MagicNumber = 0x7AADF00D;
    GunnerPacketHeader.PacketType = 0;
    GunnerPacketHeader.DataSize = 0;

    DriverMotionState = new MotionSystemStateDataPacket();
    DriverMotionState->PacketReady = false;
    DriverMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    DriverMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    DriverMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    DriverMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    GunnerMotionState = new MotionSystemStateDataPacket();
    GunnerMotionState->PacketReady = false;
    GunnerMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    GunnerMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    GunnerMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    GunnerMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    DriverMotionFeedback = new MotionSystemFeedbackDataPacket();
    DriverMotionFeedback->PacketReady = false;
    DriverMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    DriverMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    DriverMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    DriverMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    DriverMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    DriverMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)

    GunnerMotionFeedback = new MotionSystemFeedbackDataPacket();
    GunnerMotionFeedback->PacketReady = false;
    GunnerMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    GunnerMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    GunnerMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)


    Speed = 0;
    Waveshield_down = true;
}

T_MotionSystem::~T_MotionSystem()
{
    delete DriverMotionState;
    delete DriverMotionFeedback;
    delete GunnerMotionState;
    delete GunnerMotionFeedback;
}

void T_MotionSystem::Init()
{
    DriverMotionSystemActive = false;
    GunnerMotionSystemActive = false;

    DriverPacketHeader.MagicNumber = 0x7AADF00D;
    DriverPacketHeader.PacketType = 0;
    DriverPacketHeader.DataSize = 0;
    GunnerPacketHeader.MagicNumber = 0x7AADF00D;
    GunnerPacketHeader.PacketType = 0;
    GunnerPacketHeader.DataSize = 0;

    DriverMotionState->PacketReady = false;
    DriverMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    DriverMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    DriverMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    DriverMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    GunnerMotionState->PacketReady = false;
    GunnerMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    GunnerMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    GunnerMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    GunnerMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    DriverMotionFeedback->PacketReady = false;
    DriverMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    DriverMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    DriverMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    DriverMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    DriverMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    DriverMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)

    GunnerMotionFeedback->PacketReady = false;
    GunnerMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    GunnerMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    GunnerMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    GunnerMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)

    Speed = 0;
}

int T_MotionSystem::GetDriverQueueBuffer()
{
    int buffer = 0;
    buffer = DriverMotionStateQueue.count() +
             DriverMotionFeedbackQueue.count();
    return buffer;
}

void T_MotionSystem::ClearDriverQueueBuffer()
{
    DriverMotionStateQueue.clear();
    DriverMotionFeedbackQueue.clear();
}

void T_MotionSystem::DriverMotionStateEnqueuePacket(MotionSystemStateDataPacket packet)
{
    DriverMotionStateQueue.enqueue(packet);
    if(DriverMotionStateQueue.count() > QueueMaxLength)
    {
        DriverMotionStateDequeuePacket();
    }
}

void T_MotionSystem::DriverMotionStateDequeuePacket()
{
    if(!DriverMotionStateQueue.isEmpty()) *DriverMotionState = DriverMotionStateQueue.dequeue();
}

void T_MotionSystem::DriverMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet)
{
    DriverMotionFeedbackQueue.enqueue(packet);
    if(DriverMotionFeedbackQueue.count() > QueueMaxLength)
    {
        DriverMotionFeedbackDequeuePacket();
    }
}

void T_MotionSystem::DriverMotionFeedbackDequeuePacket()
{
    if(!DriverMotionFeedbackQueue.isEmpty()) *DriverMotionFeedback = DriverMotionFeedbackQueue.dequeue();
}

int T_MotionSystem::GetGunnerQueueBuffer()
{
    int buffer = 0;
    buffer = GunnerMotionStateQueue.count() +
             GunnerMotionFeedbackQueue.count();
    return buffer;
}

void T_MotionSystem::ClearGunnerQueueBuffer()
{
    GunnerMotionStateQueue.clear();
    GunnerMotionFeedbackQueue.clear();
}

void T_MotionSystem::GunnerMotionStateEnqueuePacket(MotionSystemStateDataPacket packet)
{
    GunnerMotionStateQueue.enqueue(packet);
    if(GunnerMotionStateQueue.count() > QueueMaxLength)
    {
        GunnerMotionStateDequeuePacket();
    }
}

void T_MotionSystem::GunnerMotionStateDequeuePacket()
{
    if(!GunnerMotionStateQueue.isEmpty()) *GunnerMotionState = GunnerMotionStateQueue.dequeue();
}

void T_MotionSystem::GunnerMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet)
{
    GunnerMotionFeedbackQueue.enqueue(packet);
    if(GunnerMotionFeedbackQueue.count() > QueueMaxLength)
    {
        GunnerMotionFeedbackDequeuePacket();
    }
}

void T_MotionSystem::GunnerMotionFeedbackDequeuePacket()
{
    if(!GunnerMotionFeedbackQueue.isEmpty()) *GunnerMotionFeedback = GunnerMotionFeedbackQueue.dequeue();
}
