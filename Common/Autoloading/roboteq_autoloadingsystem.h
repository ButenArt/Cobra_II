#ifndef ROBOTEQ_AUTOLOADINGSYSTEM_H
#define ROBOTEQ_AUTOLOADINGSYSTEM_H

#include <QtCore>
/**
 * @brief The RoboteQ_AutoLoadingSystem class
 * Класс отвечает за автомат заряжания
 * Основной метод Process надо крутить в таймере
 * По отдельности можно вызывать методы каждой операции
 * алгоритма по отдельности.
 * В каждую операцию встроены проверки на допустимость
 * ее выполнения
 *
 * После запуска программы необходимо запарковать карусель
 * методом setCarouselParking
 *
 * Для работы запускаем в цикле Process, и, когда нужно
 * StartLoadGun, StartLoadAmmo.
 * В режиме LoadAmmo после того, как загрузили снаряды
 * в кассету и пользователь нажал кнопку, надо вызвать
 * метод setUserSetCassetteDown
 */
class RoboteQ_AutoLoadingSystem: public QObject
{
public:

    enum class AutoLoadingSystemStates  // Состояния АЗ, используются при автоматическом выполнении алгоритмов
    {
        Idle = 0,                       // Режим простоя
        SetGunOnLoadAngle = 1,          // Установка ствола на угол заряжания
        SetGunLock = 2,                 // Установка стопора ствола
        TransporterStart = 3,           // Запуск транспортера(убирает заряженные снаряды в лоток под стволом)
        SetFrameUp = 4,                 // Поднятие рамки вверх
        SetOrdnanceNumber = 5,          // Установка номера снаряда в карусели
        SetShellUp = 6,                 // Установка кассеты на уровень заряжания СНАРЯДА в ствол
        RammerOutShell = 7,             // Досылание СНАРЯДА в пушку(выезжает Rammer)
        RammerInShell = 8,              // Возвращение досылателя
        SetChargeUp = 9,                // Установка кассеты на уровень заряжания ЗАРЯДА в ствол
        PusherOutCharge = 10,           // Досылание ЗАРЯДА в пушку(выезжает Rammer)
        PusherInCharge = 11,            // Возвращение досылателя
        SetCassetteDown = 12,           // Опускание кассеты в карусель
        SetFrameDown = 13,              // Опускание рамки
        SetGunUnlock = 14,              // Снятие ствола со стопора
        TransporterStop = 15,           // Остановка транспортера
        SetGunOnFireAngle = 16,         // Установка пушка на угол стрельбы
        SetCassetteForLoad = 17,        // Поднятие кассеты на уровень для загрузки в кассету СНАРЯДА и ЗАРЯДА
        WaitUserToSetCassetteDown = 18, // Ожидание, пока пользователь нажмет кнопку, что СНАРЯД и ЗАРЯД в кассету загружены
        StopAll = 19,                   // Остановить работу
        SetGunOnAngle = 20,             // Установка ствола на какой-либо угол
        RammerIn = 21,                  // Спрятать досылатель
        RammerOut = 22,                 // Выпустить досылатель
        FrameStop = 23,                 // Остановить движение рамки
        DisableGunLock = 24,            // Отключить питание стопора башни
        CarouselParking = 25            // Припарковать карусель
    };
    Q_ENUM(AutoLoadingSystemStates)

    enum class AutoLoadingSystemErrors // Ошибки, не позволяющие выполнить операцию
    {
        OK = 0,
        RammerIsNotIn = 1,              // Досылатель не внутри
        FrameIsNotDown = 2,             // Рамка не опущена
        GunIsLocked = 3,                // Ствол стоит на стопоре
        OrdnanceIsNotArrived = 4,       // Запрошенный номер снаряда еще не доехал(поворот карусели)
        WrongOrdnanceNumber = 5,        // Номер снаряда под подачу не совпадает с запрошенным
        FrameIsNotUp = 6,               // Рамка не поднята
        GunIsNotLocked = 7,             // Ствол НЕ стоит на стопоре
        CassetteIsNotDown = 8,          // Кассета не опущена
        CassetteIsMoving = 9,           // Кассета находится в движении
        GunIsNotOnLoadAngle = 10        // Ствол стоит НЕ на угле заряжания
    };
    Q_ENUM(AutoLoadingSystemErrors)

    enum class AutoLoadingSystemModes   // Режимы работы автомата заряжания, используется для выполнения алгоритмов
    {
        Idle = 0,                       // Режим простоя
        GunLoad = 1,                    // Режим заряжания пушки
        AmmoLoad = 2                    // Режим загрузки снарядов в карусель
    };
    Q_ENUM(AutoLoadingSystemModes)

    enum class FrameCommands            // Команды управления рамкой
    {
        Idle = 0,                       // Простой
        Down = 1,                       // Вниз
        Up = 2                          // Вверх
    };
    Q_ENUM(FrameCommands)

    enum class FrameStates              // Состояния рамки
    {
        Moving = 0,                     // Движется
        Down = 1,                       // Опущена
        Up = 2                          // Поднята
    };
    Q_ENUM(FrameStates)

    enum class CassetteCommands         // Команды управления кассетой
    {
        Idle = 0,                       // Простой
        Down = 1,                       // Двигаться вниз в карусель
        Shell = 2,                      // Двигаться на уровень заряжания СНАРЯДА в пушку
        Charge = 3,                     // Двигаться на уровень заряжания ЗАРЯДА в пушку
        AmmoLoad = 4                    // Двигаться на уровень загрузки СНАРЯДА и ЗАРЯДА в кассету
    };
    Q_ENUM(CassetteCommands)

    enum class CassetteStates           // Состояния кассеты
    {
        Moving = 0,                     // Движется
        Down = 1,                       // Внизу, в карусели
        Shell = 2,                      // На уровне заряжания СНАРЯДА в пушку
        Charge = 3,                     // На уровне заряжания ЗАРЯДА в пушку
        AmmoLoad = 4,                   // На уровне загрузки СНАРЯДА и ЗАРЯДА в кассету
    };
    Q_ENUM(CassetteStates)

    enum class GunLockTransporterCommands   // Команды управления стопором пушки и транспортером
    {
        Idle = 0,                           // Простой
        StartTransporter = 1,               // Запустить транспортер
        SetGunLock = 2,                     // Поставить пушку на стопор
        UnsetGunLock = 3                    // Снять пушку со стопора
    };
    Q_ENUM(GunLockTransporterCommands)

    enum class GunLockStates                // Состояния стопора пушки
    {
        Moving = 0,                         // Движется
        Unlock = 1,                         // Пушка НЕ на стопоре
        Lock = 2                            // Пушка на стопоре
    };
    Q_ENUM(GunLockStates)

    enum class RammerCommands               // Команды управления досылателем
    {
        Idle = 0,                           // Простой
        In = 1,                             // Команда на выезд
        Out = 2                             // Команда на возвращение
    };
    Q_ENUM(RammerCommands)

    enum class RammerStates                 // Состояние досылателя
    {
        Moving = 0,                         // Движется
        In = 1,                             // Спрятан
        Out = 2                             // Выдвинут
    };
    Q_ENUM(RammerStates)

    enum class GunAngleCommands             // Команды установки угла пушки
    {
        Idle = 0,                           // Простой
        Load = 1,                           // Установить пушку на угол заряжания
        Fire = 2                            // Установить пушку на угол огня
    };
    Q_ENUM(GunAngleCommands)

    enum class GunAngleStates               // Состояние пушки
    {
        Moving = 0,                         // Движется
        Load = 1,                           // На угле заряжания
        Fire = 2,                           // На угле ведения огня
    };
    Q_ENUM(GunAngleStates)

    enum class CarouselParkingCommands      // Команда на парковку карусели
    {
        Idle = 0,                           // Простой
        Park = 1                            // Запарковать карусель
    };
    Q_ENUM(CarouselParkingCommands)
    Q_OBJECT

public:
    RoboteQ_AutoLoadingSystem();

    void Process();                         // Основной процесс, который должен крутиться в таймере
    void clearCommands();                   // Установка переменных, содержащих команды в состояние простоя

    void setOrdnanceNumberToLoad(quint32 number,
                                 bool force = false);   // Установка переменной номера снаряда в карусели, force - выполнить, игнорируя ошибки

    void setGunOnLoadAngleCommand(bool force = false);  // Установка пушки на угол заряжания
    void setGunOnFireAngleCommand(bool force = false);  // Установка пушки на угол огня

    void setCassetteDownCommand(bool force = false);    // Отправить кассету вниз, в карусель
    void setCassetteForLoadCommand(bool force = false); // Установить кассету на уровень загрузки СНАРЯДА и ЗАРЯДА в кассету
    void setShellUpCommand(bool force = false);         // Установить кассету на уровень заряжания СНАРЯДА в пушку
    void setChargeUpCommand(bool force = false);        // Установить кассету на уровень заряжания ЗАРЯДА в пушку

    void setFrameUpCommand(bool force = false);         // Поднять рамку
    void setFrameDown(bool force = false);              // Опустить рамку
    void setFrameStop(bool force = false);              // Остановить рамку

    void setGunLockCommand(bool force = false);         // Поставить пушку на стопор
    void setGunUnlockCommand(bool force = false);       // Снять пушку со стопора
    void disableGunLock(bool force = false);            // Отключить питание стопора
    void startTransporter(bool force = false);          // Запустить транспортер
    void stopTransporter(bool force = false);           // Остановить транспортер

    void setOrdnanceNumberCommand(quint32 number,
                                  bool force = false);  // Повернуть карусель на снаряд number

    void setCarouselParking(bool force = false);        // Припарковать карусель
    void setCarouselIdle(bool force = false);           // Перевести карусель в режим простоя

    void setRammerInCommand(bool force = false);        // Выдвинуть досылатель
    void setRammerOutCommand(bool force = false);       // Спрятать досылатель


    void StartGunLoad(quint32 ordnanceNumber);          // Запуск алгоритма заряжания снаряда ordnanceNumber в пушку

    void StartAmmoLoad(quint32 ordnanceNumber);         // Запуск алгоритма загрузки снаряда ordnanceNumber в карусель

    void setUserSetCassetteDown();                      // Разрешение от пользователя на опускание кассеты после загрузки в нее СНАРЯДА и ЗАРЯДА

    // Установка переменных состояния от RoboteQ
    void setCurrentGunPosition(quint8 load,
                               quint8 fire);            // Положение пушки

    void setCurrentCassettePosition(quint8 down,
                                    quint8 ammoLoad,
                                    quint8 shell,
                                    quint8 charge);     // Положение кассеты

    void setCurrentFrameState(quint8 down,
                              quint8 up);               // Положение Рамки

    void setCurrentOrdnanceNumber(quint32 number,
                                  quint8 arrived);      // Положение карусели

    void setOrdnanceArrived(bool arrived);              // Приехал ли заказанный снаряд

    void setCurrentRammerState(quint8 out,
                               quint8 in);              // Положение досылателя

    void setCurrentGunLockState(quint8 lock,
                                quint8 unlock);         // Состояние стопора пушки

    void setCurrentTransporterState(quint16 state);     // Состояние транспортера

    // методы для получения значений команд для передачи RoboteQ
    quint16 getCassettePositionCommandInt();
    quint8 getPusherPositionCommandInt();
    quint16 getGunLockTransporterCommandInt();
    quint32 getGunAngleCommandInt();
    quint32 getFrameCommandInt();
    quint32 getOrdnanceNumberCommandInt();
    quint32 getCarouselParkingCommandInt();

    // Методы получения строковых значений состояний и режимов работы АЗ
    QString getCurrentStateString();
    QString getCurrentModeString();

    QString getCurrentGunAngleStateString();
    QString getCurrentCassetteStateString();
    QString getCurrentGunLockStateString();
    QString getCurrentTransporterStateString();
    QString getCurrentPusherStateString();
    QString getCurrentOrdnanceNumberString();
    QString getOrdnanceNumberToLoad();
    QString getCurrentOrdnanceArrivedString();
    QString getCurrentFrameStateString();

private:
    bool userSetCassetteDown;
    AutoLoadingSystemStates state;
    AutoLoadingSystemModes mode;
    AutoLoadingSystemErrors error;
    AutoLoadingSystemStates stateWithError;
    quint32 ordnanceNumberToLoad;

    GunAngleStates currentGunAngleState;
    CassetteStates currentCassetteState;
    FrameStates currentFrameState;
    GunLockStates currentGunLockState;
    quint32 currentTransporterState;

    quint32 currentOrdnanceNumber;
    bool currentOrdnanceArrived;

    RammerStates currentPusherState;

    CassetteCommands cassettePositionCommand;
    RammerCommands pusherPositionCommand;
    GunLockTransporterCommands gunLockTransporterStateCommand;
    GunAngleCommands gunAngleCommand;
    FrameCommands frameCommand;
    quint32 ordnanceNumberCommand;
    CarouselParkingCommands carouselParkingCommand;


    void setError(RoboteQ_AutoLoadingSystem::AutoLoadingSystemStates state, RoboteQ_AutoLoadingSystem::AutoLoadingSystemErrors error);
    void setErrorOk(RoboteQ_AutoLoadingSystem::AutoLoadingSystemStates state);

    void setCarouselParkingCommand(CarouselParkingCommands command, bool force = false);
    void setGunAngleCommand(GunAngleCommands command, bool force = false);
    void setCassettePosition(CassetteCommands position, bool force = false);
    void setFramePosition(FrameCommands command, bool force = false);
    void setGunLockAndTransporterCommand(GunLockTransporterCommands state, bool force = false);
    void setRammerPosition(RammerCommands position, bool force = false);

    void AmmoLoadProcess();
    void GunLoadProcess();

signals:
    // Сигнал возникновения ошибки в работе АЗ
    void newErrorOccured(AutoLoadingSystemStates state, AutoLoadingSystemErrors error);
};

#endif // ROBOTEQ_AUTOLOADINGSYSTEM_H
