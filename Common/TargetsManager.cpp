#include "TargetsManager.h"
#include <QDebug>

T_TargetsManager::T_TargetsManager(TLogging *log, QObject *parent) : QObject(parent)
{
    _log = log;
    startingDateTime = QDateTime::currentDateTimeUtc();
    secsFromStart = 0;

    commandMoveForward = false;

    UpdateTimer = new QTimer(this);
    UpdateTimer->setInterval(20);
    connect(UpdateTimer,SIGNAL(timeout()),this, SLOT(slotUpdateManager()));
    UpdateTimer->start();

    ManagerTimer = new QTimer(this);
    ManagerTimer->setInterval(20);
    connect(ManagerTimer,SIGNAL(timeout()),this, SLOT(slotTargetsManager()));
    ManagerTimer->stop();
}

T_TargetsManager::~T_TargetsManager()
{
    delete ManagerTimer;
    delete UpdateTimer;
}

void T_TargetsManager::TargetsManagerStart()
{
    ManagerTimer->stop();
    startingDateTime = QDateTime::currentDateTimeUtc();
    secsFromStart = 0;
    ManagerTimer->start();
}

void T_TargetsManager::TargetsManagerStop()
{
    ManagerTimer->stop();
    secsFromStart = 0;
}

void T_TargetsManager::slotUpdateManager()
{
    if(_Instructor->exerciseStarted)
    {
        if(commandMoveForward)
        {
            //_log->WriteBallisticsLog("Entered slotUpdateManager commandMoveForward = true");
            TargetsManagerStart();
            commandMoveForward = false;
        }
    }
    else TargetsManagerStop();
}

void T_TargetsManager::slotTargetsManager()
{
    secsFromStart = startingDateTime.secsTo(QDateTime::currentDateTimeUtc());
    TargetsControlDataPacket TargetsControl;
    TargetsControl.PacketReady = true;
    TargetsControl.targetControlList.clear();
    for(int i=0; i < _Instructor->Targets->mapTargetsList.count(); i++)
    {
        TargetControl targetControl;
        MapTargets mapTargets;
        mapTargets = _Instructor->Targets->mapTargetsList[i];

        if ((_Instructor->ExerciseParameters->night && mapTargets.onlyDay) ||
            (!_Instructor->ExerciseParameters->night && mapTargets.onlyNight))
            continue;

        if (!mapTargets.onlyDay && !mapTargets.onlyNight)
            continue;

        for(int j=0; j < mapTargets.targetShowNumber; j++)
        {
            if(mapTargets.targetsShowList[j].startTime == secsFromStart)
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = true;
    //            _log->WriteBallisticsLog("Raise target: ouid: " + QString::number(targetControl.ObjectiveUID) +
    //                                     " tuid: " + QString::number(targetControl.TargetUID) +
    //                                     " state: " + QString::number(targetControl.TargetState) +
    //                                     " delay: " + QString::number(mapTargets.delay) +
    //                                     " atime: " + QString::number(mapTargets.activeTime) +
    //                                     " secs: " + QString::number(secsFromStart));
                TargetsControl.targetControlList << targetControl;
            }
            else if(mapTargets.onlyDay && (mapTargets.targetsShowList[j].startTime + mapTargets.targetsShowList[j].dayTime == secsFromStart))
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = false;
    //            _log->WriteBallisticsLog("Down target: ouid: " + QString::number(targetControl.ObjectiveUID) +
    //                                     " tuid: " + QString::number(targetControl.TargetUID) +
    //                                     " state: " + QString::number(targetControl.TargetState) +
    //                                     " delay: " + QString::number(mapTargets.delay) +
    //                                     " atime: " + QString::number(mapTargets.activeTime) +
    //                                     " secs: " + QString::number(secsFromStart));
                TargetsControl.targetControlList << targetControl;
            }
            else if(mapTargets.onlyNight && (mapTargets.targetsShowList[j].startTime + mapTargets.targetsShowList[j].nightTime == secsFromStart))
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = false;
                TargetsControl.targetControlList << targetControl;
            }
        }

    }

    TargetsControl.mapTargetsCount = TargetsControl.targetControlList.size();
    if(TargetsControl.mapTargetsCount > 0)
    {
        //_log->WriteBallisticsLog("Enqueing maptargets " + QString::number(TargetsControl.mapTargetsCount));
        _Imager->TargetsControlEnqueuePacket(TargetsControl);
    }
}
