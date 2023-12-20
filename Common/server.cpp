#include "server.h"

T_Server::T_Server(QObject *parent) : QObject(parent)
{
    ServerReady = new ServerReadyDataPacket();
    ServerReady->PacketReady = false;
    ServerReady->simulatorType = SimulatorTypes::Unknown; // тип техники
    ServerReady->ServerReady = false;      // Готов

    CANBusStateMsg = "";
    CANBusState = ElementsStateTypes::CanBlockNoAnswer;
    ResidentDriverState = ElementsStateTypes::NoSoftwareAnswer;
    ResidentGunnerState = ElementsStateTypes::NoSoftwareAnswer;
    MotionSystemDriverState = ElementsStateTypes::NoSoftwareAnswer;
    MotionSystemGunnerState = ElementsStateTypes::NoSoftwareAnswer;
    JoystickState = ElementsStateTypes::NoJoystickAnswer;
    ResetPower = false;
    ResetPowerTickCounter = 0;
    clockGenerator = 0;

    elements.ElementType = 0;              // Элемент тренажера
    elements.ElementState = 0;             // Неисправность элемента тренажера
    elements.ElementNameSize = 0;          // Длина названия
    elements.ElementName = "";             // Название
    elements.CommentSize = 0;              // Длина описания
    elements.Comment = "";                 // Описание

    startingDateTime = QDateTime::currentDateTimeUtc();
    clockGeneratorTimer = new QTimer(this);
    clockGeneratorTimer->setInterval(500);
    connect(clockGeneratorTimer,SIGNAL(timeout()),this, SLOT(slotClockGenerator()));
    clockGeneratorTimer->start();
}

T_Server::~T_Server()
{
    delete clockGeneratorTimer;
    delete ServerReady;
}

void T_Server::Init()
{
    ServerReady->PacketReady = false;
    ServerReady->simulatorType = SimulatorTypes::Unknown; // тип техники
    ServerReady->ServerReady = false;      // Готов

    CANBusStateMsg = "";
    CANBusState = ElementsStateTypes::CanBlockNoAnswer;
    ResidentDriverState = ElementsStateTypes::NoSoftwareAnswer;
    ResidentGunnerState = ElementsStateTypes::NoSoftwareAnswer;
    MotionSystemDriverState = ElementsStateTypes::NoSoftwareAnswer;
    MotionSystemGunnerState = ElementsStateTypes::NoSoftwareAnswer;
    JoystickState = ElementsStateTypes::NoJoystickAnswer;
    ResetPower = false;
    ResetPowerTickCounter = 0;
    clockGenerator = 0;

    elements.ElementType = 0;              // Элемент тренажера
    elements.ElementState = 0;             // Неисправность элемента тренажера
    elements.ElementNameSize = 0;          // Длина названия
    elements.ElementName = "";             // Название
    elements.CommentSize = 0;              // Длина описания
    elements.Comment = "";                 // Описание
}

void T_Server::slotClockGenerator()
{
    clockGenerator = startingDateTime.msecsTo(QDateTime::currentDateTimeUtc());
}
