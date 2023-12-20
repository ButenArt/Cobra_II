#ifndef COMMON_DICTIONARY_H
#define COMMON_DICTIONARY_H

enum class Seasons                         // Сезоны
{
	Unknown = 0,
	Summer = 1,
	Winter = 2,
	Autumn = 3,
	Spring = 4
};

enum class SimulatorTypes                  // Виды симуляторов
{
	Unknown = 0,
	BTR80 = 1,
	BMP2 = 2,
	KAMAZ_43114 = 3,
	URAL_4320 = 4,
	UAZ = 5,
	MBT2000 = 6,
	TIGER_233014 = 7,
	RPG7 = 8,
	SPG9M = 9,
	AGS17 = 10,
	GP25 = 11,
	MTLB = 13,
	T59G = 14,
	PSO = 15,
	T72B = 16,
	Mi17V5 = 17,
	L39C = 18,
	Mi171 = 19,
	Mi171pod = 20,
	L39Cpod = 21,
	FAGOT = 22,
	COBRA2 = 23
};

enum class ElementsTypes                   // Элементы тренажера
{
	Unknown = 0,
	Server = 1,
	CAN = 2,
	Resident = 3,
	MotionSystem = 4,
	VisualServer = 5,
	OU = 6,
	VisualClient = 7,
	IPcamera = 8,
	Joystick = 9,
	Instructor = 10,
	PositionTracking = 11,
	BackFireSystem = 12,
	ScriptServer = 13,
	ProjectionSystem = 14
};

enum class ElementsStateTypes              // Неисправности элементов тренажера
{
	Good = 0,                                // Исправен
	CanBlockNoAnswer = 1,                    // Не отвечает 1 или несколько CAN блоков
	NoPing = 2,                              // Компютер не отвечает на пинг команду
	NoSoftwareAnswer = 3,                    // Не отвечает программа
	NoJoystickAnswer = 4                     // Джойстик не подключен
};

enum class MessagesTypes                   // Типы текстовых сообщений
{
	Unknown = 0,
	Warning = 1,
	Error = 2,
	Normal = 3
};

enum class GunTypes                        // Типы оружия
{
	Unknown = 0,
	KPVT = 1,
	PKT = 2,
	Cannon_2A42 = 3,
	ATGM = 4,
	RPG7 = 5,
	SPG9M = 6,
	GP25 = 7,
	AGS17 = 8,
	ZPT98 = 9,
	MBT2000_MG = 10,
	MBT2000_AIRCRAFT_MG = 11,
	MG_7_62 = 12,
	AAMG_12_7 = 13
};

enum class AmmoSubTypes                    // ПодТипы снарядов
{
	Unknown = 0,
	Normal = 1,
	Tracer = 2,
	Antipersonnel = 3,
	ArmorPiercing = 4
};

enum class ClientPacketTypes               // Виды сетевых пакетов
{
	InstructorPacket = 1,                    // Инструктор
	ImagerPacket = 2,                        // Визуализация
	ScriptServerPacket = 3,                  // Сервер сценариев
	ControlsImitatorPacket = 4,              // Имитатор органов управления
	OU_IND_MV_Packet = 5,                    // Органы управления (механик-водитель) индикация
	OU_UPR_MV_Packet = 6,                    // Органы управления (механик-водитель) управление
	OU_IND_BO_Packet = 7,                    // Органы управления (боевое отделение) индикация
	OU_UPR_BO_Packet = 8,                    // Органы управления (боевое отделение) управление
	MBT2000_FCC_Packet = 9,                  // Компьютер управления огнем танка MBT2000
	MotionSystemPacket = 10,                 // Система подвижности
	T59G_FCC_Packet = 11,                    // Компьютер управления огнем танка T59G
	ScreenItems_Packet = 12,                 // Приборы отрисованные на экране
	UAZ_PATRIOT_CARCOMP_Packet = 13          // Бортовой компьютер УАЗ ПАТРИОТ
};

enum class ImagerTCPPacketTypes            // Типы пакетов между визуалкой (IG) и сервером (SR)
{
	SR2IG_ExerciseParametersPacket = 1,      // Параметры упражнения
	SR2IG_ObjectsDataPacket = 2,             // Объекты на карте
	SR2IG_EnvironmentDataPacket = 3,         // Параметры окружающей среды
	IG2SR_ImagerStateDataPacket = 4,         // Состояние визуалки
	SR2IG_TargetsDataPacket = 5,             // Мишени на карте
	IG2SR_ImagerReadyDataPacket = 6,         // Готов к упражнению
	SR2IG_CommandsDataPacket = 13,           // Команды инструктора
	SR2IG_MessagesDataPacket = 14,           // Текстовые сообщения
	IG2SR_ImagerElementsStateDataPacket = 15,// Данные о готовности компонентов
	SR2IG_TargetsControlDataPacket = 16,     // Управление мишенями
	SR2IG_ProjectileDataPacket = 17,         // Расчет траекторий боеприпасов
	SR2IG_WeaponStateDataPacket=18,
	SR2IG_ScoreRequestDataPacket = 27,     // [ ID=27 # Server >> Imager ] Запрос оценки
	SR2IG_DriverStateDataPacket = 101,       // Состояние имитатора - механик-водитель
	SR2IG_CommanderStateDataPacket = 102,    // Состояние имитатора - командир
	SR2IG_GunnerStateDataPacket = 103,       // Состояние имитатора - наводчик
	SR2IG_LoaderStateDataPacket = 104,       // Состояние имитатора - заряжающий
	IG2SR_ScoreDrivingDataPacket = 111,      // [ ID=111 # Imager >> Server ] Оценка подсчитана (вождение/полеты)
	IG2SR_ScoreShootingDataPacket = 112,     // [ ID=112 # Imager >> Server ] Оценка подсчитана (стрельба)
	IG2SR_ScoreDrivingTruckDataPacket = 113, // [ ID=113 # Imager >> Server ] Оценка подсчитана (грузовика)
	IG2SR_GunnerAimStateDataPacket = 303,    // Состояние прицелов наводчика
	IG2SR_CommanderAimStateDataPacket = 304, // Состояние прицелов командира
	IG2SR_SensorsDataPacket = 305,           // Параметры автоматических сенсоров
	IG2SR_BallisticEventsPacket = 1001,      // Событие попадания снаряда
	IG2SR_TriggerEventsPacket = 1002         // Событие прохождения препятствий (тригеры)
};

enum class InstructorTCPPacketTypes        // Типы пакетов между инструктором (IR) и сервером (SR)
{
	IR2SR_ExerciseParametersPacket = 1,      // Параметры упражнения
	IR2SR_ObjectsDataPacket = 2,             // Объекты на карте
	IR2SR_TargetsDataPacket = 3,             // Мишени на карте
	IR2SR_EnvironmentDataPacket = 4,         // Параметры окружающей среды
	IR2SR_AmmunitionDataPacket = 5,          // Боеприпасы
	IR2SR_ControlDataPacket = 11,            // Управление комплексом
	IR2SR_FailuresDataPacket = 12,           // Ввод отказов
	IR2SR_CommandsDataPacket = 13,           // Команды инструктора
	IR2SR_MessagesDataPacket = 14,           // Текстовые сообщения
	IR2SR_ExerciseResultDataPacket = 15,     // Результаты упражнения
	IR2SR_AutoLoadSystemTestDataPacket = 17, // Протестировать работу автомата заряжания
	IR2SR_ScoreRequestDataPacket = 27,       // [ ID=27 # Instructor >> Server ] Запрос оценки
	IR2SR_StatisticsDataPacket = 50,         // Статистика
	SR2IR_DriverStateDataPacket = 100,       // Состояние имитатора - механик-водитель
	SR2IR_BallisticEventsPacket = 101,       // Событие изменения состояния мишеней
	SR2IR_TriggerEventsPacket = 102,         // Событие прохождения препятствий (тригеры)
	SR2IR_ImagerReadyStatePacket = 103,      // Готовность к упражнению
	SR2IR_CommanderStateDataPacket = 104,    // Состояние имитатора - командир
	SR2IR_GunnerStateDataPacket = 105,       // Состояние имитатора - наводчик
	SR2IR_ServerReadyStatePacket = 106,      // Готовность к упражнению
	SR2IR_AutoLoadSystemStatePacket = 109,   // Результат тестирования работы автомата заряжания
	SR2IR_ScoreDrivingDataPacket = 111,      // [ ID=111 # Server >> Instructor ] Оценка подсчитана (вождение/полеты)
	SR2IR_ScoreShootingDataPacket = 112,     // [ ID=112 # Server >> Instructor ] Оценка подсчитана (стрельба)
	SR2IR_ScoreDrivingTruckDataPacket = 113, // [ ID=113 # Server >> Instructor ] Оценка подсчитана (грузовика)
	SR2IR_ComponentsStatePacket = 200        // Данные о готовности компонентов тренажера
};

enum class ScriptServerTCPPacketTypes      // Типы пакетов между сервером (SR) и сервером сценариев (ST)
{
	SR2ST_ControlDataPacket = 11             // Управление комплексом
};

enum class ControlsImitatorTCPPacketTypes  // Типы пакетов между сервером (SR) и имитатором органов управления (CI)
{
	SR2CI_DriverControlsDataPacket = 500,    // Органы управления механика-водителя
	SR2CI_GunnerControlsDataPacket = 501,    // Органы управления наводчика
	SR2CI_CommanderControlsDataPacket = 502, // Органы управления командира
	SR2CI_EngineStartDependentEvents = 505,  // Необходимые условия запуска двигателя
	SR2CI_ReplenishingUnloadDependentEvents = 506, // Необходимые условия загрузки/выгрузки снарядов в карусель
	SR2CI_StartingFCSDependentEvents = 507,        // Необходимые условия запуска системы управления орудием
	SR2CI_GunnerAmmoLoadDependentEvents = 508,     // Необходимые условия заряжания наводчиком снаряда в ствол пушки
	SR2CI_OverrideFiringModeDependentEvents = 509, // Необходимые условия включения режима управления орудием командиром
	SR2CI_CommanderAmmoLoadDependentEvents = 510,  // Необходимые условия заряжания командиром снаряда в ствол пушки
	SR2CI_ManualAmmoLoadDependentEvents = 511      // Необходимые условия ручного заряжания снаряда в ствол пушки
};

enum class MBT2000_FCC_TCPPacketTypes      // Типы пакетов между сервером (SR) и имитатором компьютера управления огнем танка MBT2000 (FC)
{
	SR2FC_FCCControlsDataPacket = 400,       // Состояние элементов управления компьютера
	SR2FC_AutotrackingDataPacket = 401,      // Состояние режима автосопровождения цели
	SR2FC_SensorsDataPacket = 402,           // Данные от внешних сенсоров
	SR2FC_SelfTestDataPacket = 403,          // Данные самотестирования
	FC2SR_ManualDataPacket = 404,            // Данные скорректированные вручную
	FC2SR_DisplayDataPacket = 405            // Картинка с дисплея ТБК
};

enum class T59G_FCC_TCPPacketTypes         // Типы пакетов между сервером (SR) и имитатором компьютера управления огнем танка T59G (FC)
{
	SR2FC_FCCControlsDataPacket = 410,       // Состояние элементов управления компьютера
	SR2FC_AutotrackingDataPacket = 411,      // Состояние режима автосопровождения цели
	SR2FC_SensorsDataPacket = 412,           // Данные от внешних сенсоров
	SR2FC_SelfTestDataPacket = 413,          // Данные самотестирования
	FC2SR_ManualDataPacket = 414,            // Данные скорректированные вручную
	FC2SR_DisplayDataPacket = 415            // Картинка с дисплея ТБК
};

enum class ScreenItems_TCPPacketTypes      // Типы пакетов между сервером (SR) и приборами отрисованными на экране (SI)
{
	SR2SI_AzimuthDataPacket = 420,           // Азимутальный указатель
	SR2SI_NavigatorDataPacket = 421          // ГПК
};

enum class UAZ_PATRIOT_CARCOMP_TCPPacketTypes   // Типы пакетов между сервером (SR) и имитатором бортового компьютера УАЗ ПАТРИОТ (CC)
{
	SR2CC_ControlsDataPacket = 430,          // Состояние элементов управления компьютера
	SR2CC_SensorsDataPacket = 431,           // Данные от внешних сенсоров
	CC2SR_DisplayDataPacket = 432            // Картинка с дисплея БК
};

enum class MotionSystemTCPPacketTypes      // Типы пакетов между сервером (SR) и системой подвижности (MS)
{
	SR2MS_MotionSystemStateDataPacket = 0,   // Положение имитатора из матмодели
	MS2SR_MotionSystemFeedbackDataPacket = 1 // Положение имитатора с учетом положения системы подвижности
};

enum class NetworkProtocols
{
	Unknown = 0,
	UDP = 1,
	TCP = 2
};

enum class LogEvents
{
	Application_start = 0,
	Application_close = 1,
	TCP_Imager_connected = 2,
	TCP_Imager_disconnected = 3,
	TCP_Imager_Error = 4,
	TCP_Imager_Unknown_PacketType = 5,
	TCP_Instructor_connected = 6,
	TCP_Instructor_disconnected = 7,
	TCP_Instructor_Error = 8,
	TCP_Instructor_Unknown_PacketType = 9,
	TCP_DriverMotionSystem_connected = 10,
	TCP_DriverMotionSystem_disconnected = 11,
	TCP_DriverMotionSystem_Error = 12,
	TCP_DriverMotionSystem_Unknown_PacketType = 13,
	TCP_GunnerMotionSystem_connected = 14,
	TCP_GunnerMotionSystem_disconnected = 15,
	TCP_GunnerMotionSystem_Error = 16,
	TCP_GunnerMotionSystem_Unknown_PacketType = 17,
	TCP_ScriptServer_connected = 18,
	TCP_ScriptServer_disconnected = 19,
	TCP_ScriptServer_Error = 20,
	TCP_ScriptServer_Unknown_PacketType = 21,
	TCP_ControlsImitator_connected = 22,
	TCP_ControlsImitator_disconnected = 23,
	TCP_ControlsImitator_Error = 24,
	TCP_ControlsImitator_Unknown_PacketType = 25,
	TCP_FCC_connected = 26,
	TCP_FCC_disconnected = 27,
	TCP_FCC_Error = 28,
	TCP_FCC_Unknown_PacketType = 29,
	DB_Replay_Open_Error = 30
};

#endif // COMMON_DICTIONARY_H
