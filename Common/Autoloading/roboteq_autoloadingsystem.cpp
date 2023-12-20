#include "roboteq_autoloadingsystem.h"

RoboteQ_AutoLoadingSystem::RoboteQ_AutoLoadingSystem()
{
    state = AutoLoadingSystemStates::Idle;
    mode = AutoLoadingSystemModes::Idle;
    clearCommands();
}

void RoboteQ_AutoLoadingSystem::clearCommands()
{
    userSetCassetteDown = false;
    cassettePositionCommand = CassetteCommands::Idle;
    pusherPositionCommand = RammerCommands::Idle;
    gunLockTransporterStateCommand = GunLockTransporterCommands::Idle;
    gunAngleCommand = GunAngleCommands::Idle;
    frameCommand = FrameCommands::Idle;
    ordnanceNumberCommand = -1;
    carouselParkingCommand = CarouselParkingCommands::Idle;
}

void RoboteQ_AutoLoadingSystem::setError(AutoLoadingSystemStates state, AutoLoadingSystemErrors error)
{
    emit newErrorOccured(state, error);
}

void RoboteQ_AutoLoadingSystem::setErrorOk(AutoLoadingSystemStates state)
{
    setError(state, AutoLoadingSystemErrors::OK);
}

void RoboteQ_AutoLoadingSystem::setOrdnanceNumberToLoad(quint32 number, bool force)
{
    ordnanceNumberToLoad = number;
    setErrorOk(AutoLoadingSystemStates::Idle);
}

void RoboteQ_AutoLoadingSystem::setGunOnLoadAngleCommand(bool force)
{
    setGunAngleCommand(GunAngleCommands::Load, force);
}

void RoboteQ_AutoLoadingSystem::setGunOnFireAngleCommand(bool force)
{
    setGunAngleCommand(GunAngleCommands::Fire, force);
}

void RoboteQ_AutoLoadingSystem::setGunAngleCommand(GunAngleCommands command, bool force)
{
    if(currentPusherState != RammerStates::In ||
       currentFrameState != FrameStates::Down ||
       currentGunLockState == GunLockStates::Lock)
    {
        if(currentPusherState != RammerStates::In)
            setError(AutoLoadingSystemStates::SetGunOnAngle, AutoLoadingSystemErrors::RammerIsNotIn);
        if(currentFrameState != FrameStates::Down)
            setError(AutoLoadingSystemStates::SetGunOnAngle, AutoLoadingSystemErrors::FrameIsNotDown);
        if(currentGunLockState == GunLockStates::Lock)
            setError(AutoLoadingSystemStates::SetGunOnAngle, AutoLoadingSystemErrors::GunIsLocked);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    gunAngleCommand = command;
    setErrorOk(AutoLoadingSystemStates::SetGunOnAngle);
}

void RoboteQ_AutoLoadingSystem::setCassetteDownCommand(bool force)
{
    if(!currentOrdnanceArrived ||
       ordnanceNumberToLoad != currentOrdnanceNumber ||
       currentPusherState != RammerStates::In)
    {
        if(!currentOrdnanceArrived)
            setError(AutoLoadingSystemStates::SetCassetteDown, AutoLoadingSystemErrors::OrdnanceIsNotArrived);
        if(ordnanceNumberToLoad != currentOrdnanceNumber)
            setError(AutoLoadingSystemStates::SetCassetteDown, AutoLoadingSystemErrors::WrongOrdnanceNumber);
        if(currentPusherState != RammerStates::In)
            setError(AutoLoadingSystemStates::SetCassetteDown, AutoLoadingSystemErrors::RammerIsNotIn);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setCassettePosition(CassetteCommands::Down);
    setErrorOk(AutoLoadingSystemStates::SetCassetteDown);
}

void RoboteQ_AutoLoadingSystem::setCassetteForLoadCommand(bool force)
{
    if(!currentOrdnanceArrived)
    {
        setError(AutoLoadingSystemStates::SetCassetteForLoad, AutoLoadingSystemErrors::OrdnanceIsNotArrived);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setCassettePosition(CassetteCommands::AmmoLoad);
    setErrorOk(AutoLoadingSystemStates::SetCassetteForLoad);
}

void RoboteQ_AutoLoadingSystem::setShellUpCommand(bool force)
{
    if(currentPusherState != RammerStates::In ||
       currentFrameState != FrameStates::Up)
    {
        if(currentPusherState != RammerStates::In)
            setError(AutoLoadingSystemStates::SetShellUp, AutoLoadingSystemErrors::RammerIsNotIn);
        if(currentFrameState != FrameStates::Up)
            setError(AutoLoadingSystemStates::SetShellUp, AutoLoadingSystemErrors::FrameIsNotUp);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setCassettePosition(CassetteCommands::Shell);
    setErrorOk(AutoLoadingSystemStates::SetShellUp);
}

void RoboteQ_AutoLoadingSystem::setChargeUpCommand(bool force)
{
    if(currentPusherState != RammerStates::In ||
       currentFrameState != FrameStates::Up)
    {
        if(currentPusherState != RammerStates::In)
            setError(AutoLoadingSystemStates::SetChargeUp, AutoLoadingSystemErrors::RammerIsNotIn);
        if(currentFrameState != FrameStates::Up)
            setError(AutoLoadingSystemStates::SetChargeUp, AutoLoadingSystemErrors::FrameIsNotUp);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setCassettePosition(CassetteCommands::Charge);
    setErrorOk(AutoLoadingSystemStates::SetChargeUp);
}

void RoboteQ_AutoLoadingSystem::setCassettePosition(CassetteCommands position, bool force)
{
    cassettePositionCommand = position;
}

void RoboteQ_AutoLoadingSystem::setFrameUpCommand(bool force)
{
    if(currentGunAngleState != GunAngleStates::Load)
    {
        setError(AutoLoadingSystemStates::SetFrameUp, AutoLoadingSystemErrors::GunIsNotOnLoadAngle);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setFramePosition(FrameCommands::Up);
    setErrorOk(AutoLoadingSystemStates::SetFrameUp);
}

void RoboteQ_AutoLoadingSystem::setFrameDown(bool force)
{
    if(currentCassetteState != CassetteStates::Down ||
       currentPusherState != RammerStates::In)
    {
        if(currentCassetteState != CassetteStates::Down)
            setError(AutoLoadingSystemStates::SetFrameDown, AutoLoadingSystemErrors::CassetteIsNotDown);
        if(currentPusherState != RammerStates::In)
            setError(AutoLoadingSystemStates::SetFrameDown, AutoLoadingSystemErrors::RammerIsNotIn);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setFramePosition(FrameCommands::Down);
    setErrorOk(AutoLoadingSystemStates::SetFrameDown);
}

void RoboteQ_AutoLoadingSystem::setFrameStop(bool force)
{
    setFramePosition(FrameCommands::Idle);
    setErrorOk(AutoLoadingSystemStates::FrameStop);
}

void RoboteQ_AutoLoadingSystem::setFramePosition(FrameCommands command, bool force)
{
    frameCommand = command;
}

void RoboteQ_AutoLoadingSystem::setGunLockCommand(bool force)
{
    setGunLockAndTransporterCommand(GunLockTransporterCommands::SetGunLock);
    setErrorOk(AutoLoadingSystemStates::SetGunLock);
}

void RoboteQ_AutoLoadingSystem::setGunUnlockCommand(bool force)
{
    setGunLockAndTransporterCommand(GunLockTransporterCommands::UnsetGunLock);
    setErrorOk(AutoLoadingSystemStates::SetGunUnlock);
}

void RoboteQ_AutoLoadingSystem::disableGunLock(bool force)
{
    setGunLockAndTransporterCommand(GunLockTransporterCommands::Idle);
    setErrorOk(AutoLoadingSystemStates::DisableGunLock);
}

void RoboteQ_AutoLoadingSystem::startTransporter(bool force)
{
    if(currentGunLockState != GunLockStates::Lock)
    {
        if(currentGunLockState != GunLockStates::Lock)
            setError(AutoLoadingSystemStates::TransporterStart, AutoLoadingSystemErrors::GunIsNotLocked);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setGunLockAndTransporterCommand(GunLockTransporterCommands::StartTransporter);
    setErrorOk(AutoLoadingSystemStates::TransporterStart);
}

void RoboteQ_AutoLoadingSystem::stopTransporter(bool force)
{
    disableGunLock();
    setErrorOk(AutoLoadingSystemStates::TransporterStop);
}

void RoboteQ_AutoLoadingSystem::setGunLockAndTransporterCommand(GunLockTransporterCommands state, bool force)
{
    gunLockTransporterStateCommand = state;
}

void RoboteQ_AutoLoadingSystem::setOrdnanceNumberCommand(quint32 number, bool force)
{
    if(currentCassetteState != CassetteStates::Down ||
       number < 0 || number > 21)
    {
        if(currentCassetteState != CassetteStates::Down)
            setError(AutoLoadingSystemStates::SetOrdnanceNumber, AutoLoadingSystemErrors::CassetteIsNotDown);
        if(number < 0 || number > 21)
            setError(AutoLoadingSystemStates::SetOrdnanceNumber, AutoLoadingSystemErrors::WrongOrdnanceNumber);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    ordnanceNumberCommand = number;
    setErrorOk(AutoLoadingSystemStates::SetOrdnanceNumber);
}

void RoboteQ_AutoLoadingSystem::setCarouselParking(bool force)
{
    if(currentCassetteState != CassetteStates::Down)
    {
        if(currentCassetteState != CassetteStates::Down)
            setError(AutoLoadingSystemStates::CarouselParking, AutoLoadingSystemErrors::CassetteIsNotDown);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setCarouselParkingCommand(CarouselParkingCommands::Park);
    setErrorOk(AutoLoadingSystemStates::CarouselParking);
}

void RoboteQ_AutoLoadingSystem::setCarouselIdle(bool force)
{
    setCarouselParkingCommand(CarouselParkingCommands::Idle);
    setErrorOk(AutoLoadingSystemStates::CarouselParking);
}

void RoboteQ_AutoLoadingSystem::setCarouselParkingCommand(CarouselParkingCommands command, bool force)
{
    carouselParkingCommand = command;
}

void RoboteQ_AutoLoadingSystem::setRammerInCommand(bool force)
{
    setRammerPosition(RammerCommands::In);
    setErrorOk(AutoLoadingSystemStates::RammerIn);
}

void RoboteQ_AutoLoadingSystem::setRammerOutCommand(bool force)
{
    if(currentCassetteState == CassetteStates::Moving ||
       currentGunAngleState != GunAngleStates::Load ||
       currentFrameState != FrameStates::Up)
    {
        if(currentCassetteState == CassetteStates::Moving)
            setError(AutoLoadingSystemStates::RammerOut, AutoLoadingSystemErrors::CassetteIsMoving);
        if(currentGunAngleState != GunAngleStates::Load)
            setError(AutoLoadingSystemStates::RammerOut, AutoLoadingSystemErrors::GunIsNotOnLoadAngle);
        if(currentFrameState != FrameStates::Up)
            setError(AutoLoadingSystemStates::RammerOut, AutoLoadingSystemErrors::FrameIsNotUp);

        state = AutoLoadingSystemStates::StopAll;
        clearCommands();
        if(!force)
            return;
    }
    setRammerPosition(RammerCommands::Out);
    setErrorOk(AutoLoadingSystemStates::RammerOut);
}

void RoboteQ_AutoLoadingSystem::setRammerPosition(RammerCommands position, bool force)
{
    pusherPositionCommand = position;
}

void RoboteQ_AutoLoadingSystem::StartGunLoad(quint32 ordnanceNumber)
{
    if(mode != AutoLoadingSystemModes::Idle && state != AutoLoadingSystemStates::Idle)
        return;

    ordnanceNumberToLoad = ordnanceNumber;
    mode = AutoLoadingSystemModes::GunLoad;
    state = AutoLoadingSystemStates::SetGunOnLoadAngle;
}

void RoboteQ_AutoLoadingSystem::StartAmmoLoad(quint32 ordnanceNumber)
{
    if(mode != AutoLoadingSystemModes::Idle && state != AutoLoadingSystemStates::Idle)
        return;

    ordnanceNumberToLoad = ordnanceNumber;
    mode = AutoLoadingSystemModes::AmmoLoad;
    state = AutoLoadingSystemStates::SetOrdnanceNumber;
}

void RoboteQ_AutoLoadingSystem::Process()
{
    switch(mode)
    {
    case AutoLoadingSystemModes::AmmoLoad:
        AmmoLoadProcess();
        break;
    case AutoLoadingSystemModes::GunLoad:
        GunLoadProcess();
        break;
    case AutoLoadingSystemModes::Idle:
    default:
        break;
    }
}

void RoboteQ_AutoLoadingSystem::AmmoLoadProcess()
{
    if(mode != AutoLoadingSystemModes::AmmoLoad)
        state = AutoLoadingSystemStates::StopAll;

    switch(state)
    {
        case AutoLoadingSystemStates::SetOrdnanceNumber:
        {
            setOrdnanceNumberCommand(ordnanceNumberToLoad);

            if(currentOrdnanceArrived && currentOrdnanceNumber == ordnanceNumberToLoad)
                state = AutoLoadingSystemStates::SetCassetteForLoad;

            break;
        }
        case AutoLoadingSystemStates::SetCassetteForLoad:
        {
            setCassetteForLoadCommand();

            if(currentCassetteState == CassetteStates::AmmoLoad)
                state = AutoLoadingSystemStates::WaitUserToSetCassetteDown;

            break;
        }
        case AutoLoadingSystemStates::WaitUserToSetCassetteDown:
        {
            if(userSetCassetteDown)
                state = AutoLoadingSystemStates::SetCassetteDown;

            break;
        }
        case AutoLoadingSystemStates::SetCassetteDown:
        {
            setCassetteDownCommand();

            if(currentCassetteState == CassetteStates::Down)
                state = AutoLoadingSystemStates::StopAll;

            break;
        }
        case AutoLoadingSystemStates::StopAll:
        {
            clearCommands();
            state = AutoLoadingSystemStates::Idle;
            mode = AutoLoadingSystemModes::Idle;
            break;
        }
        default:
            state = AutoLoadingSystemStates::StopAll;
            break;
    }
}

void RoboteQ_AutoLoadingSystem::GunLoadProcess()
{
    if(mode != AutoLoadingSystemModes::GunLoad)
        state = AutoLoadingSystemStates::StopAll;

    switch(state)
    {
        case AutoLoadingSystemStates::SetGunOnLoadAngle:
        {
            setGunOnLoadAngleCommand();

            if(currentGunAngleState == GunAngleStates::Load)
                state = AutoLoadingSystemStates::SetGunLock;

            break;
        }
        case AutoLoadingSystemStates::SetGunLock:
        {
            setGunLockCommand();

            if(currentGunLockState == GunLockStates::Lock)
                state = AutoLoadingSystemStates::TransporterStart;

            break;
        }
        case AutoLoadingSystemStates::TransporterStart:
        {
            startTransporter();
            state = AutoLoadingSystemStates::SetFrameUp;
            break;
        }
        case AutoLoadingSystemStates::SetFrameUp:
        {
            setFrameUpCommand();

            if(currentFrameState == FrameStates::Up)
                state = AutoLoadingSystemStates::SetOrdnanceNumber;
            break;
        }
        case AutoLoadingSystemStates::SetOrdnanceNumber:
        {
            setOrdnanceNumberCommand(ordnanceNumberToLoad);

            if(currentOrdnanceArrived && currentOrdnanceNumber == ordnanceNumberToLoad)
                state = AutoLoadingSystemStates::SetShellUp;
            break;
        }
        case AutoLoadingSystemStates::SetShellUp:
        {
            setShellUpCommand();

            if(currentCassetteState == CassetteStates::Shell)
                state = AutoLoadingSystemStates::RammerOutShell;
            break;
        }
        case AutoLoadingSystemStates::RammerOutShell:
        {
            setRammerOutCommand();

            if(currentPusherState == RammerStates::Out)
                state = AutoLoadingSystemStates::RammerInShell;
            break;
        }
        case AutoLoadingSystemStates::RammerInShell:
        {
            setRammerInCommand();

            if(currentPusherState == RammerStates::In)
                state = AutoLoadingSystemStates::SetChargeUp;
            break;
        }
        case AutoLoadingSystemStates::SetChargeUp:
        {
            setChargeUpCommand();

            if(currentCassetteState == CassetteStates::Charge)
                state = AutoLoadingSystemStates::PusherOutCharge;
            break;
        }
        case AutoLoadingSystemStates::PusherOutCharge:
        {
            setRammerOutCommand();

            if(currentPusherState == RammerStates::Out)
                state = AutoLoadingSystemStates::PusherInCharge;
            break;
        }
        case AutoLoadingSystemStates::PusherInCharge:
        {
            setRammerInCommand();

            if(currentPusherState == RammerStates::In)
                state = AutoLoadingSystemStates::SetCassetteDown;
            break;
        }
        case AutoLoadingSystemStates::SetCassetteDown:
        {
            setCassetteDownCommand();

            if(currentCassetteState == CassetteStates::Down)
                state = AutoLoadingSystemStates::SetFrameDown;
            break;
        }
        case AutoLoadingSystemStates::SetFrameDown:
        {
            setFrameDown();

            if(currentFrameState == FrameStates::Down)
                state = AutoLoadingSystemStates::SetGunUnlock;
            break;
        }
        case AutoLoadingSystemStates::SetGunUnlock:
        {
            setGunUnlockCommand();

            if(currentGunLockState == GunLockStates::Unlock)
                state = AutoLoadingSystemStates::TransporterStop;
            break;
        }
        case AutoLoadingSystemStates::TransporterStop:
        {
            stopTransporter();
            state = AutoLoadingSystemStates::SetGunOnFireAngle;
            break;
        }
        case AutoLoadingSystemStates::SetGunOnFireAngle:
        {
            setGunOnFireAngleCommand();

            if(currentGunAngleState == GunAngleStates::Fire)
                state = AutoLoadingSystemStates::StopAll;
            break;
        }
        case AutoLoadingSystemStates::StopAll:
        {
            clearCommands();
            state = AutoLoadingSystemStates::Idle;
            mode = AutoLoadingSystemModes::Idle;
            break;
        }
        default:
            state = AutoLoadingSystemStates::StopAll;
            break;
    }
}

void RoboteQ_AutoLoadingSystem::setCurrentGunPosition(quint8 load, quint8 fire)
{
    if(load == 1)
        currentGunAngleState = GunAngleStates::Load;
    else if(fire == 1)
        currentGunAngleState = GunAngleStates::Fire;
    else
        currentGunAngleState = GunAngleStates::Moving;
}

void RoboteQ_AutoLoadingSystem::setCurrentCassettePosition(quint8 down, quint8 ammoLoad, quint8 shell, quint8 charge)
{
    if(down == 1)
        currentCassetteState = CassetteStates::Down;
    else if(ammoLoad == 1)
        currentCassetteState = CassetteStates::AmmoLoad;
    else if(shell == 1)
        currentCassetteState = CassetteStates::Shell;
    else if(charge == 1)
        currentCassetteState = CassetteStates::Charge;
    else
        currentCassetteState = CassetteStates::Moving;
}

void RoboteQ_AutoLoadingSystem::setCurrentFrameState(quint8 down, quint8 up)
{
    if(down == 1)
        currentFrameState = FrameStates::Down;
    else if(up == 1)
        currentFrameState = FrameStates::Up;
    else
        currentFrameState = FrameStates::Moving;
}

void RoboteQ_AutoLoadingSystem::setCurrentOrdnanceNumber(quint32 number, quint8 arrived)
{
    currentOrdnanceNumber = number;
    currentOrdnanceArrived = arrived;
}

void RoboteQ_AutoLoadingSystem::setOrdnanceArrived(bool arrived)
{
    currentOrdnanceArrived = arrived;
}

void RoboteQ_AutoLoadingSystem::setCurrentRammerState(quint8 out, quint8 in)
{
    if(out == 1)
        currentPusherState = RammerStates::Out;
    else if(in == 1)
        currentPusherState = RammerStates::In;
    else
        currentPusherState = RammerStates::Moving;
}

void RoboteQ_AutoLoadingSystem::setCurrentGunLockState(quint8 lock, quint8 unlock)
{
    if(lock == 1)
        currentGunLockState = GunLockStates::Lock;
    else if(unlock == 1)
        currentGunLockState = GunLockStates::Unlock;
    else
        currentGunLockState = GunLockStates::Moving;
}

void RoboteQ_AutoLoadingSystem::setCurrentTransporterState(quint16 state)
{
    currentTransporterState = state;
}

void RoboteQ_AutoLoadingSystem::setUserSetCassetteDown()
{
    userSetCassetteDown = true;
}

quint16 RoboteQ_AutoLoadingSystem::getCassettePositionCommandInt()
{
    return static_cast<quint16>(cassettePositionCommand);
}

quint8 RoboteQ_AutoLoadingSystem::getPusherPositionCommandInt()
{
    return static_cast<quint8>(pusherPositionCommand);
}

quint16 RoboteQ_AutoLoadingSystem::getGunLockTransporterCommandInt()
{
    return static_cast<quint16>(gunLockTransporterStateCommand);
}

quint32 RoboteQ_AutoLoadingSystem::getGunAngleCommandInt()
{
    return static_cast<quint32>(gunAngleCommand);
}

quint32 RoboteQ_AutoLoadingSystem::getFrameCommandInt()
{
    return static_cast<quint32>(frameCommand);
}

quint32 RoboteQ_AutoLoadingSystem::getOrdnanceNumberCommandInt()
{
    return ordnanceNumberCommand;
}

quint32 RoboteQ_AutoLoadingSystem::getCarouselParkingCommandInt()
{
    return static_cast<quint32>(carouselParkingCommand);
}

QString RoboteQ_AutoLoadingSystem::getCurrentStateString()
{
    return QVariant::fromValue(state).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentModeString()
{
    return QVariant::fromValue(mode).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentGunAngleStateString()
{
    return QVariant::fromValue(currentGunAngleState).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentCassetteStateString()
{
    return QVariant::fromValue(currentCassetteState).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentGunLockStateString()
{
    return QVariant::fromValue(currentGunLockState).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentTransporterStateString()
{
    return QString::number(currentTransporterState);
}

QString RoboteQ_AutoLoadingSystem::getCurrentPusherStateString()
{
    return QVariant::fromValue(currentPusherState).value<QString>();
}

QString RoboteQ_AutoLoadingSystem::getCurrentOrdnanceNumberString()
{
    return QString::number(currentOrdnanceNumber);
}

QString RoboteQ_AutoLoadingSystem::getOrdnanceNumberToLoad()
{
    return QString::number(ordnanceNumberToLoad);
}

QString RoboteQ_AutoLoadingSystem::getCurrentOrdnanceArrivedString()
{
    return currentOrdnanceArrived ? "True" : "False";
}

QString RoboteQ_AutoLoadingSystem::getCurrentFrameStateString()
{
    return QVariant::fromValue(currentFrameState).value<QString>();
}
