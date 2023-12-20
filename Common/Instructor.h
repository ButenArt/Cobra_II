#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <QObject>
#include <Windows.h>
#include <QUuid>
#include <QTimer>
#include <QQueue>
#include "common_dictionary.h"


#pragma pack(push, 1)
typedef struct
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
} InstructorTCPHeader;
#pragma pack(pop)

typedef struct
{
    qint32 objUID;                     // UID объекта
    quint8 type;                       // Тип объекта
    quint8 subType;                    // Подтип объекта
    qint32 azimuth;                    // Азимут
    float latitude;                    // Широта
    float longitude;                   // Долгота
} MapObjects;

typedef struct
{
    quint32 startTime;                 // Время старта
    quint32 dayTime;                   // Время показа днем
    quint32 nightTime;                 // Время показа ночью
} TargetsShow;

typedef struct
{
    qint32 ObjectiveUID;               // UID цели
    qint32 TargetUID;                  // UID мишени
    quint8 TargetType;                 // Тип мишени
    quint8 TargetColor;                // Цвет мишени
    qint32 azimuth;                    // Азимут
    float latitude;                    // Широта
    float longitude;                   // Долгота
    bool running;                      // Дижущаяся или нет
    float speed;                       // Скорость движения (м/с)
    qint32 moveAzimuth;                // Азимут движения
    qint32 moveDistance;               // Расстояние движения
    bool onlyNight;                    // Показывать только ночью
    bool onlyDay;                      // Показывать только днем
    bool dropTarget;                   // Выключать (опускать при поражении)
    bool blockTarget;                  // Блокировать при поражении
    quint8 targetShowNumber;           // Количество показов
    QList<TargetsShow> targetsShowList;
    //qint32 delay;                      // Задержка перед появлением (сек)
    //qint32 activeTime;                 // Время показа (сек)
} MapTargets;

typedef struct
{
    quint8 gunType;                    // Тип оружия
    quint8 ammoType;                   // Тип снарядов
    quint32 amount;                    // Количество
} Ammunition;


typedef struct
{
    bool PacketReady;                  // готовность пакета
    bool ReStart;                      // Старт/Перезапуск упражнения
    bool Pause;                        // Пауза (Визуализация замирает)
    bool Stop;                         // Стоп (Прервать упражнение)
    quint8 ExerciseNum;                // Номер упражнения
    quint8 MapType;                    // Тип карты
    bool night;                        // Ночь
    quint8 season;                     // Сезон
    bool Joystick;                     // Джойстик
    bool AutoloadingSystem;            // Автомат заряжания
    qint32 uuidSize;                   // размер GUID
    QByteArray Uuid;                   // GUID карты и упражнения
    quint8 directionsNumber;           // Количество направлений
    bool backfire;                     // Ответный огонь
    quint8 selectedKit;                // Выбранный комплект
    quint8 selectedMode;               // Выбранный режим
    quint8 distance2screen;            // Расстояние до экрана
    QString guid_run;                  //  
    QString lang;                      //  
} ExerciseParametersPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 mapObjCount;                // Число объектов
    QList<MapObjects> mapObjList;
} ObjectsDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 mapTargetsCount;            // Число мишеней
    QList<MapTargets> mapTargetsList;
} TargetsDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    qint32 unixtime;                   // Дата и время
    qint32 temperature;                // Температура (градусы цельсия)
    qint32 cloudsGrade;                // Бальность облаков
    float bottomCloudsLevel;           // Нижняя кромка облаков [m]
    float topCloudsLevel;              // Верхняя кромка облаков [m]
    bool moistureLevel;                // Наличие осадков (дождь,снег)
    float windSpeed;                   // Скорость ветра [м/c]
    float windDirection;               // Направление ветра градусы
    float visibility;                  // Метеовидимость
    float verticalWindSpeed;           // Скорость ветра вертикальная [м/c]
    float windBlows;                   // Порывы ветра [м/c]
    float pressure;                    // Атмосферное давление
    qint32 snowLevel;                  // Заснеженность в баллах
    qint32 camNumber;                  // Номер камеры
    bool fog;                          // Туман
    qint32 amount_precipitation;    // Количество выпавших осадков [мм]
    qint32 intensity_precipitation; // Интенсивность атмосферных осадков [мм/мин]
    qint32 intensity_vvp;           // Интенсивность огней взлетно-посадочной полосы [%]
} EnvironmentDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    bool isNewPacket;                  // состояние пакета
    quint8 packageCount;               // Число наборов
    QList<Ammunition> AmmunitionList;
} AmmunitionDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    quint8 CamNum;                     // номер камеры
} CameraDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    quint32 CommandSize;               // длина команды
    QString CommandText;               // текст команды
} ControlDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    quint8 failureType;                // Тип отказа
    bool failureActive;                // Отказ включен (активирован)
} FailuresDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    bool MoveForward;                  // команда "Вперед"
} CommandsDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    quint8 MessageType;                // тип сообщения
    qint32 MessageSize;                // длина сообщения
    QString MessageText;               // текст сообщения
} MessagesDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    bool MakeTest;                     // тестировать работу автомата заряжания
} AutoLoadSystemTestDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    bool TestRequired;                 // требуется тестирование работы автомата заряжания
    bool TestSuccessful;               // работа автомата заряжания протестирована успешно
} AutoLoadSystemStatePacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    quint32 Odometer;                  // Показание одометра (в метрах)
    quint32 EngineClock;               // Моточасы двигателя (в секундах)
} StatisticsDataPacket;

// [ ID=27 # Instructor >> Server ] Запрос оценки
struct ScoreRequestDataPacket
{
    bool PacketReady = false;                   // готовность пакета
    bool flag = false;
};


class T_Instructor : public QObject
{
    Q_OBJECT

public:
    bool InstructorActive;
    //служебные параметры упражнения//
    bool exerciseReInit;               // переинициализировать все параметры имитатора
    bool exerciseStarted;              // упражнение начато
    qint32 exerciseTime;               // прошедшее время с момента старта для текущего упражнения (сек)
    qint32 exerciseTimeOld;            // прошедшее время с момента старта для предыдущего упражнения (сек)
    bool commandMoveForward;           // команда "Вперед" визуализацией обработана
    //макимальная длина очереди//
    const quint8 QueueMaxLength = 5;
    //заголовок пакетов//
    InstructorTCPHeader PacketHeader;
    //параметры упражнения//
    //typedef struct ExerciseParametersPacket;
    ExerciseParametersPacket *ExerciseParameters;
    QQueue<ExerciseParametersPacket> ExerciseParametersQueue;
    //объекты на карте//
    //MapObjects mapObj;
    //struct ObjectsDataPacket;
    ObjectsDataPacket *Objects;
    QQueue<ObjectsDataPacket> ObjectsQueue;
    //мишени на карте//
    //MapTargets mapTargets;
    //struct TargetsDataPacket;
    TargetsDataPacket *Targets;
    QQueue<TargetsDataPacket> TargetsQueue;
    //параметры окружающей среды//
    //struct EnvironmentDataPacket;
    EnvironmentDataPacket *Environment;
    QQueue<EnvironmentDataPacket> EnvironmentQueue;
    //боеприпасы//
    //struct AmmunitionDataPacket;
    AmmunitionDataPacket *AmmunitionInPack;   // начальное состояние
    QQueue<AmmunitionDataPacket> AmmunitionInPackQueue;
    AmmunitionDataPacket *AmmunitionOutPack;  // текущее состояние
    QQueue<AmmunitionDataPacket> AmmunitionOutPackQueue;
    //переключение вида//
    //struct CameraDataPacket;
    CameraDataPacket *Camera;
    QQueue<CameraDataPacket> CameraQueue;
    //управление комплексом//
    //struct ControlDataPacket;
    ControlDataPacket *Control;
    QQueue<ControlDataPacket> ControlQueue;
    //ввод отказов//
    //struct FailuresDataPacket;
    FailuresDataPacket *Failures;
    QQueue<FailuresDataPacket> FailuresQueue;
    //команды инструктора//
    //struct CommandsDataPacket;
    CommandsDataPacket *Commands;
    QQueue<CommandsDataPacket> CommandsQueue;
    //текстовые сообщения//
    //struct MessagesDataPacket;
    MessagesDataPacket *Messages;
    QQueue<MessagesDataPacket> MessagesQueue;
    //тестировать автомат заряжания//
    AutoLoadSystemTestDataPacket *AutoLoadSystemTest;
    QQueue<AutoLoadSystemTestDataPacket> AutoLoadSystemTestQueue;
    //состояние автомата заряжания//
    AutoLoadSystemStatePacket *AutoLoadSystemState;
    QQueue<AutoLoadSystemStatePacket> AutoLoadSystemStateQueue;
    //статистика//
    //struct StatisticsDataPacket;
    StatisticsDataPacket *Statistics;
    QQueue<StatisticsDataPacket> StatisticsQueue;
    // запрос оценки
    ScoreRequestDataPacket* ScoreRequest;
    QQueue<ScoreRequestDataPacket> ScoreRequestQueue;

    explicit T_Instructor(QObject *parent = 0);
    ~T_Instructor();
    virtual void Init();
    void ExerciseReStart();
    void ExerciseStop();
    void UnloadAmmo();
    virtual int GetQueueBuffer();
    virtual void ClearQueueBuffer();
    void ExerciseParametersEnqueuePacket(ExerciseParametersPacket packet);
    void ExerciseParametersDequeuePacket();
    void ObjectsEnqueuePacket(ObjectsDataPacket packet);
    void ObjectsDequeuePacket();
    void TargetsEnqueuePacket(TargetsDataPacket packet);
    void TargetsDequeuePacket();
    void EnvironmentEnqueuePacket(EnvironmentDataPacket packet);
    void EnvironmentDequeuePacket();
    void AmmunitionInEnqueuePacket(AmmunitionDataPacket packet);
    void AmmunitionInDequeuePacket();
    void AmmunitionOutEnqueuePacket(AmmunitionDataPacket packet);
    void AmmunitionOutDequeuePacket();
    void CameraEnqueuePacket(CameraDataPacket packet);
    void CameraDequeuePacket();
    void ControlEnqueuePacket(ControlDataPacket packet);
    void ControlDequeuePacket();
    void FailuresEnqueuePacket(FailuresDataPacket packet);
    void FailuresDequeuePacket();
    void CommandsEnqueuePacket(CommandsDataPacket packet);
    void CommandsDequeuePacket();
    void MessagesEnqueuePacket(MessagesDataPacket packet);
    void MessagesDequeuePacket();
    void AutoLoadSystemTestEnqueuePacket(AutoLoadSystemTestDataPacket packet);
    void AutoLoadSystemTestDequeuePacket();
    void AutoLoadSystemStateEnqueuePacket(AutoLoadSystemStatePacket packet);
    void AutoLoadSystemStateDequeuePacket();
    void StatisticsEnqueuePacket(StatisticsDataPacket packet);
    void StatisticsDequeuePacket();
    void ScoreRequestEnqueuePacket( ScoreRequestDataPacket packet);
    void ScoreRequestDequeuePacket();
    qint32 GetAmmunitionAmount(quint8 ammunitionType);
    void SetAmmunitionAmount(quint8 ammunitionType, int amount);
protected:
    QTimer *TimerExerciseTime;
private slots:
    void OnTimer1sec();
};


#endif // INSTRUCTOR_H
