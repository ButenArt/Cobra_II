#include "Instructor.h"

T_Instructor::T_Instructor(QObject *parent) : QObject(parent)
{
    TimerExerciseTime = new QTimer(this);
    TimerExerciseTime->setInterval(1000);
    connect(TimerExerciseTime,SIGNAL(timeout()),this, SLOT(OnTimer1sec()));
    TimerExerciseTime->stop();

    InstructorActive = false;

    exerciseReInit = false;
    exerciseStarted = false;
    exerciseTime = 0;
    exerciseTimeOld = 0;

    commandMoveForward = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;

    ExerciseParameters = new ExerciseParametersPacket();
    ExerciseParameters->PacketReady = false;
    ExerciseParameters->ReStart = false;        // Старт/Перезапуск упражнения
    ExerciseParameters->Pause = false;          // Пауза (Визуализация замирает)
    ExerciseParameters->Stop = false;           // Стоп (Прервать упражнение)
    ExerciseParameters->ExerciseNum = 0;        // Номер упражнения
    ExerciseParameters->MapType = 0;            // Тип карты
    ExerciseParameters->night = false;          // Ночь
    ExerciseParameters->season = static_cast<quint8>(Seasons::Unknown); // Сезон
    ExerciseParameters->Joystick = false;       // Джойстик
    ExerciseParameters->AutoloadingSystem = false;                      // Автомат заряжания
    ExerciseParameters->uuidSize = 38;          // размер GUID
    ExerciseParameters->Uuid = QUuid::QUuid().toByteArray(); // GUID карты и упражнения
    ExerciseParameters->directionsNumber = 0;   // Количество направлений
    ExerciseParameters->backfire = false;       // Ответный огонь
    ExerciseParameters->selectedKit = 0;        // Выбранный комплект
    ExerciseParameters->selectedMode = 0;       // Выбранный режим
    ExerciseParameters->distance2screen = 0;    // Расстояние до экрана

    Objects = new ObjectsDataPacket();
    Objects->PacketReady = false;
    Objects->mapObjCount = 0;                   // Число объектов
    Objects->mapObjList.clear();                // Список

    Targets = new TargetsDataPacket();
    Targets->PacketReady = false;
    Targets->mapTargetsCount = 0;               // Число мишеней
    Targets->mapTargetsList.clear();            // Список

    Environment = new EnvironmentDataPacket();
    Environment->PacketReady = false;
    Environment->unixtime = 0;                  // Дата и время
    Environment->temperature = 20;              // Температура (градусы цельсия)
    Environment->cloudsGrade = 0;               // Бальность облаков
    Environment->bottomCloudsLevel = 0;         // Нижняя кромка облаков [m]
    Environment->topCloudsLevel = 0;            // Верхняя кромка облаков [m]
    Environment->moistureLevel = false;         // Наличие осадков (дождь,снег)
    Environment->windSpeed = 3;                 // Скорость ветра [м/c]
    Environment->windDirection = 0;             // Направление ветра градусы
    Environment->visibility = 0;                // Метеовидимость
    Environment->verticalWindSpeed = 5;         // Скорость ветра вертикальная [м/c]
    Environment->windBlows = 0;                 // Порывы ветра [м/c]
    Environment->pressure = 780;                // Атмосферное давление
    Environment->snowLevel = 0;                 // Заснеженность в баллах
    Environment->camNumber = 0;                 // Номер камеры
    Environment->fog = false;                   // Туман

    AmmunitionInPack = new AmmunitionDataPacket();
    AmmunitionInPack->PacketReady = false;
    AmmunitionInPack->isNewPacket = false;
    AmmunitionInPack->packageCount = 0;         // Число наборов
    AmmunitionInPack->AmmunitionList.clear();   // Список

    AmmunitionOutPack = new AmmunitionDataPacket();
    AmmunitionOutPack->PacketReady = false;
    AmmunitionOutPack->packageCount = 0;        // Число наборов
    AmmunitionOutPack->AmmunitionList.clear();  // Список

    Camera = new CameraDataPacket();
    Camera->PacketReady = false;
    Camera->CamNum = 0;                         // номер камеры

    Control = new ControlDataPacket();
    Control->PacketReady = false;
    Control->CommandSize = 0;                   // длина команды
    Control->CommandText = "";                  // текст команды

    Failures = new FailuresDataPacket();
    Failures->PacketReady = false;
    Failures->failureType = 0;                  // Тип отказа
    Failures->failureActive = false;            // Отказ включен (активирован)

    Commands = new CommandsDataPacket();
    Commands->PacketReady = false;
    Commands->MoveForward = false;              // команда "Вперед"

    Messages = new MessagesDataPacket();
    Messages->PacketReady = false;
    Messages->MessageType = 0;                  // тип сообщения
    Messages->MessageSize = 0;                  // длина сообщения
    Messages->MessageText = "";                 // текст сообщения

    AutoLoadSystemTest = new AutoLoadSystemTestDataPacket();
    AutoLoadSystemTest->PacketReady = false;
    AutoLoadSystemTest->MakeTest = false;       // тестировать работу автомата заряжания

    AutoLoadSystemState = new AutoLoadSystemStatePacket();
    AutoLoadSystemState->PacketReady = false;
    AutoLoadSystemState->TestRequired = false;  // требуется тестирование работы автомата заряжания
    AutoLoadSystemState->TestSuccessful = false;// работа автомата заряжания протестирована успешно

    Statistics = new StatisticsDataPacket();
    Statistics->PacketReady = false;
    Statistics->Odometer = 0;                   // Показание одометра (в метрах)
    Statistics->EngineClock = 0;                // Моточасы двигателя (в секундах)

    ScoreRequest = new ScoreRequestDataPacket( );
}

T_Instructor::~T_Instructor()
{
    TimerExerciseTime->stop();
    delete TimerExerciseTime;
    delete ExerciseParameters;
    delete Objects;
    delete Targets;
    delete Environment;
    delete AmmunitionInPack;
    delete AmmunitionOutPack;
    delete Camera;
    delete Control;
    delete Failures;
    delete Commands;
    delete Messages;
    delete AutoLoadSystemTest;
    delete AutoLoadSystemState;
    delete Statistics;
}

void T_Instructor::Init()
{
    TimerExerciseTime->stop();

    InstructorActive = false;

    exerciseReInit = false;
    exerciseStarted = false;
    exerciseTime = 0;
    exerciseTimeOld = 0;

    commandMoveForward = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;

    ExerciseParameters->PacketReady = false;
    ExerciseParameters->ReStart = false;        // Старт/Перезапуск упражнения
    ExerciseParameters->Pause = false;          // Пауза (Визуализация замирает)
    ExerciseParameters->Stop = false;           // Стоп (Прервать упражнение)
    ExerciseParameters->ExerciseNum = 0;        // Номер упражнения
    ExerciseParameters->MapType = 0;            // Тип карты
    ExerciseParameters->night = false;          // Ночь
    ExerciseParameters->season = static_cast<quint8>(Seasons::Unknown); // Сезон
    ExerciseParameters->Joystick = false;       // Джойстик
    ExerciseParameters->AutoloadingSystem = false;                      // Автомат заряжания
    ExerciseParameters->uuidSize = 38;          // размер GUID
    ExerciseParameters->Uuid = QUuid::QUuid().toByteArray(); // GUID карты и упражнения
    ExerciseParameters->directionsNumber = 0;   // Количество направлений
    ExerciseParameters->backfire = false;       // Ответный огонь
    ExerciseParameters->selectedKit = 0;        // Выбранный комплект
    ExerciseParameters->selectedMode = 0;       // Выбранный режим
    ExerciseParameters->distance2screen = 0;    // Расстояние до экрана

    Objects->PacketReady = false;
    Objects->mapObjCount = 0;                   // Число объектов
    Objects->mapObjList.clear();                // Список

    Targets->PacketReady = false;
    Targets->mapTargetsCount = 0;               // Число мишеней
    Targets->mapTargetsList.clear();            // Список

    Environment->PacketReady = false;
    Environment->unixtime = 0;                  // Дата и время
    Environment->temperature = 20;              // Температура (градусы цельсия)
    Environment->cloudsGrade = 0;               // Бальность облаков
    Environment->bottomCloudsLevel = 0;         // Нижняя кромка облаков [m]
    Environment->topCloudsLevel = 0;            // Верхняя кромка облаков [m]
    Environment->moistureLevel = false;         // Наличие осадков (дождь,снег)
    Environment->windSpeed = 0;                 // Скорость ветра [м/c]
    Environment->windDirection = 0;             // Направление ветра градусы
    Environment->visibility = 0;                // Метеовидимость
    Environment->verticalWindSpeed = 0;         // Скорость ветра вертикальная [м/c]
    Environment->windBlows = 0;                 // Порывы ветра [м/c]
    Environment->pressure = 0;                  // Атмосферное давление
    Environment->snowLevel = 0;                 // Заснеженность в баллах
    Environment->camNumber = 0;                 // Номер камеры
    Environment->fog = false;                   // Туман

    AmmunitionInPack->PacketReady = false;
    AmmunitionInPack->isNewPacket = false;
    AmmunitionInPack->packageCount = 0;         // Число наборов
    AmmunitionInPack->AmmunitionList.clear();   // Список

    AmmunitionOutPack->PacketReady = false;
    AmmunitionOutPack->packageCount = 0;        // Число наборов
    AmmunitionOutPack->AmmunitionList.clear();  // Список

    Camera->PacketReady = false;
    Camera->CamNum = 0;                         // номер камеры

    Control->PacketReady = false;
    Control->CommandSize = 0;                   // длина команды
    Control->CommandText = "";                  // текст команды

    Failures->PacketReady = false;
    Failures->failureType = 0;                  // Тип отказа
    Failures->failureActive = false;            // Отказ включен (активирован)

    Commands->PacketReady = false;
    Commands->MoveForward = false;              // команда "Вперед"

    Messages->PacketReady = false;
    Messages->MessageType = 0;                  // тип сообщения
    Messages->MessageSize = 0;                  // длина сообщения
    Messages->MessageText = "";                 // текст сообщения

    AutoLoadSystemTest->PacketReady = false;
    AutoLoadSystemTest->MakeTest = false;       // тестировать работу автомата заряжания

    AutoLoadSystemState->PacketReady = false;
    AutoLoadSystemState->TestRequired = false;  // требуется тестирование работы автомата заряжания
    AutoLoadSystemState->TestSuccessful = false;// работа автомата заряжания протестирована успешно

    Statistics->PacketReady = false;
    Statistics->Odometer = 0;                   // Показание одометра (в метрах)
    Statistics->EngineClock = 0;                // Моточасы двигателя (в секундах)
}

void T_Instructor::ExerciseReStart()
{
    ExerciseStop();
    exerciseStarted = true;
    TimerExerciseTime->start();
}

void T_Instructor::ExerciseStop()
{
    exerciseStarted = false;
    TimerExerciseTime->stop();
    exerciseTimeOld = exerciseTime;
    exerciseTime = 0;
    exerciseReInit = true;
}

void T_Instructor::UnloadAmmo()
{
    AmmunitionDataPacket AmmunitionInPack;
    AmmunitionInPack.PacketReady = false;
    AmmunitionInPack.AmmunitionList.clear();
    AmmunitionInPack.packageCount = 0;

    AmmunitionInEnqueuePacket(AmmunitionInPack);

//    AmmunitionOutPack->PacketReady = false;
//    AmmunitionOutPack->AmmunitionList.clear();
//    AmmunitionOutPack->packageCount = 0;
}

void T_Instructor::OnTimer1sec()
{
    exerciseTime = exerciseTime + 1;
}

int T_Instructor::GetQueueBuffer()
{
    int buffer = 0;
    buffer = ExerciseParametersQueue.count() +
             ObjectsQueue.count() +
             TargetsQueue.count() +
             EnvironmentQueue.count() +
             AmmunitionInPackQueue.count() +
             AmmunitionOutPackQueue.count() +
             CameraQueue.count() +
             ControlQueue.count() +
             FailuresQueue.count() +
             CommandsQueue.count() +
             MessagesQueue.count() +
             AutoLoadSystemTestQueue.count() +
             AutoLoadSystemStateQueue.count() +
             StatisticsQueue.count();
    return buffer;
}

void T_Instructor::ClearQueueBuffer()
{
    ExerciseParametersQueue.clear();
    ObjectsQueue.clear();
    TargetsQueue.clear();
    EnvironmentQueue.clear();
    AmmunitionInPackQueue.clear();
    AmmunitionOutPackQueue.clear();
    CameraQueue.clear();
    ControlQueue.clear();
    FailuresQueue.clear();
    CommandsQueue.clear();
    MessagesQueue.clear();
    AutoLoadSystemTestQueue.clear();
    AutoLoadSystemStateQueue.clear();
    StatisticsQueue.clear();
}

void T_Instructor::ExerciseParametersEnqueuePacket(ExerciseParametersPacket packet)
{
    ExerciseParametersQueue.enqueue(packet);
    if(ExerciseParametersQueue.count() > QueueMaxLength)
    {
        ExerciseParametersDequeuePacket();
    }
}

void T_Instructor::ExerciseParametersDequeuePacket()
{
    if(!ExerciseParametersQueue.isEmpty()) *ExerciseParameters = ExerciseParametersQueue.dequeue();
}

void T_Instructor::ObjectsEnqueuePacket(ObjectsDataPacket packet)
{
    ObjectsQueue.enqueue(packet);
    if(ObjectsQueue.count() > QueueMaxLength)
    {
        ObjectsDequeuePacket();
    }
}

void T_Instructor::ObjectsDequeuePacket()
{
    if(!ObjectsQueue.isEmpty()) *Objects = ObjectsQueue.dequeue();
}

void T_Instructor::TargetsEnqueuePacket(TargetsDataPacket packet)
{
    TargetsQueue.enqueue(packet);
    if(TargetsQueue.count() > QueueMaxLength)
    {
        TargetsDequeuePacket();
    }
}

void T_Instructor::TargetsDequeuePacket()
{
    if(!TargetsQueue.isEmpty()) *Targets = TargetsQueue.dequeue();
}

void T_Instructor::EnvironmentEnqueuePacket(EnvironmentDataPacket packet)
{
    EnvironmentQueue.enqueue(packet);
    if(EnvironmentQueue.count() > QueueMaxLength)
    {
        EnvironmentDequeuePacket();
    }
}

void T_Instructor::EnvironmentDequeuePacket()
{
    if(!EnvironmentQueue.isEmpty()) *Environment = EnvironmentQueue.dequeue();
}

void T_Instructor::AmmunitionInEnqueuePacket(AmmunitionDataPacket packet)
{
    AmmunitionInPackQueue.enqueue(packet);
    if(AmmunitionInPackQueue.count() > QueueMaxLength)
    {
        AmmunitionInDequeuePacket();
    }
}

void T_Instructor::AmmunitionInDequeuePacket()
{
    if(!AmmunitionInPackQueue.isEmpty())
    {
        *AmmunitionInPack = AmmunitionInPackQueue.dequeue();
        //AmmunitionInPack->isNewPacket = true;
    }
}

void T_Instructor::AmmunitionOutEnqueuePacket(AmmunitionDataPacket packet)
{
    AmmunitionOutPackQueue.enqueue(packet);
    if(AmmunitionOutPackQueue.count() > QueueMaxLength)
    {
        AmmunitionOutDequeuePacket();
    }
}

void T_Instructor::AmmunitionOutDequeuePacket()
{
    if(!AmmunitionOutPackQueue.isEmpty()) *AmmunitionOutPack = AmmunitionOutPackQueue.dequeue();
}

void T_Instructor::CameraEnqueuePacket(CameraDataPacket packet)
{
    CameraQueue.enqueue(packet);
    if(CameraQueue.count() > QueueMaxLength)
    {
        CameraDequeuePacket();
    }
}

void T_Instructor::CameraDequeuePacket()
{
    if(!CameraQueue.isEmpty()) *Camera = CameraQueue.dequeue();
}

void T_Instructor::ControlEnqueuePacket(ControlDataPacket packet)
{
    ControlQueue.enqueue(packet);
    if(ControlQueue.count() > QueueMaxLength)
    {
        ControlDequeuePacket();
    }
}

void T_Instructor::ControlDequeuePacket()
{
    if(!ControlQueue.isEmpty()) *Control = ControlQueue.dequeue();
}

void T_Instructor::FailuresEnqueuePacket(FailuresDataPacket packet)
{
    FailuresQueue.enqueue(packet);
    if(FailuresQueue.count() > QueueMaxLength)
    {
        FailuresDequeuePacket();
    }
}

void T_Instructor::FailuresDequeuePacket()
{
    if(!FailuresQueue.isEmpty()) *Failures = FailuresQueue.dequeue();
}

void T_Instructor::CommandsEnqueuePacket(CommandsDataPacket packet)
{
    CommandsQueue.enqueue(packet);
    if(CommandsQueue.count() > QueueMaxLength)
    {
        CommandsDequeuePacket();
    }
}

void T_Instructor::CommandsDequeuePacket()
{
    if(!CommandsQueue.isEmpty()) *Commands = CommandsQueue.dequeue();
}

void T_Instructor::MessagesEnqueuePacket(MessagesDataPacket packet)
{
    MessagesQueue.enqueue(packet);
    if(MessagesQueue.count() > QueueMaxLength)
    {
        MessagesDequeuePacket();
    }
}

void T_Instructor::MessagesDequeuePacket()
{
    if(!MessagesQueue.isEmpty()) *Messages = MessagesQueue.dequeue();
}

void T_Instructor::AutoLoadSystemTestEnqueuePacket(AutoLoadSystemTestDataPacket packet)
{
    AutoLoadSystemTestQueue.enqueue(packet);
    if(AutoLoadSystemTestQueue.count() > QueueMaxLength)
    {
        AutoLoadSystemTestDequeuePacket();
    }
}

void T_Instructor::AutoLoadSystemTestDequeuePacket()
{
    if(!AutoLoadSystemTestQueue.isEmpty()) *AutoLoadSystemTest = AutoLoadSystemTestQueue.dequeue();
}

void T_Instructor::AutoLoadSystemStateEnqueuePacket(AutoLoadSystemStatePacket packet)
{
    AutoLoadSystemStateQueue.enqueue(packet);
    if(AutoLoadSystemStateQueue.count() > QueueMaxLength)
    {
        AutoLoadSystemStateDequeuePacket();
    }
}

void T_Instructor::AutoLoadSystemStateDequeuePacket()
{
    if(!AutoLoadSystemStateQueue.isEmpty()) *AutoLoadSystemState = AutoLoadSystemStateQueue.dequeue();
}

void T_Instructor::StatisticsEnqueuePacket(StatisticsDataPacket packet)
{
    StatisticsQueue.enqueue(packet);
    if(StatisticsQueue.count() > QueueMaxLength)
    {
        StatisticsDequeuePacket();
    }
}

void T_Instructor::StatisticsDequeuePacket()
{
    if(!StatisticsQueue.isEmpty()) *Statistics = StatisticsQueue.dequeue();
}

void T_Instructor::ScoreRequestEnqueuePacket( ScoreRequestDataPacket packet )
{
    ScoreRequestQueue.enqueue(packet);
    if( ScoreRequestQueue.count() > QueueMaxLength)
    {
        ScoreRequestDequeuePacket();
    }
}

void T_Instructor::ScoreRequestDequeuePacket()
{
    if(!ScoreRequestQueue.isEmpty()) *ScoreRequest = ScoreRequestQueue.dequeue();
}

qint32 T_Instructor::GetAmmunitionAmount(quint8 ammunitionType)
{
    for(int j=0; j<AmmunitionOutPack->AmmunitionList.count(); j++)
    {
       Ammunition ammunition = AmmunitionOutPack->AmmunitionList[j];
       if(ammunition.gunType == ammunitionType)
       {
           return ammunition.amount;
       }
    }
    return -1;
}

void T_Instructor::SetAmmunitionAmount(quint8 ammunitionType, int amount)
{
    for(int i = 0; i < AmmunitionOutPack->AmmunitionList.count(); i++)
    {
        if(AmmunitionOutPack->AmmunitionList[i].gunType == ammunitionType)
        {
            AmmunitionOutPack->AmmunitionList[i].amount = amount;
        }
    }
    return;
}
