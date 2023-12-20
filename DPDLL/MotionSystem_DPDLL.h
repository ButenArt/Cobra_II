#ifndef MOTIONSYSTEM_DPDLL_H
#define MOTIONSYSTEM_DPDLL_H

#include <QObject>
#include <QTimer>
#include <QtCore>
#include "Common\server.h"
#include "Common\Imager.h"
#include "Common\Instructor.h"
#include "DPDLL_lib.h"

class T_DPDLL : public QObject
{
Q_OBJECT

public:
    T_DPDLL(T_Server *Server, T_Imager *Imager, T_Instructor *Instructor);
    ~T_DPDLL();
    bool DPDLLDriverActive() { return _DPDLL_driver_Loaded; }
    bool MotionSystemDriverActive() { return _MotionSystemDriverActive; }
    bool ResidentDriverActive() { return _ResidentDriverActive; }
    bool DPDLLGunnerActive() { return _DPDLL_gunner_Loaded; }
    bool MotionSystemGunnerActive() { return _MotionSystemGunnerActive; }
    bool ResidentGunnerActive() { return _ResidentGunnerActive; }
    void Init_driver();
    void Init_gunner();
    void SetPlatformDown_driver();
    void SetPlatformDown_gunner();
    void SetPlatformData_driver();
    void SetPlatformData_gunner();
    T_Imager *_Imager;
private:
    T_DPDLL_lib *DPDLL_lib;
    QTimer *DPDLLTimer;
    T_Server *_Server;
    T_Instructor *_Instructor;
    WORD platformType_driver;
    WORD platformType_gunner;
    bool _DPDLL_driver_Loaded;
    bool _MotionSystemDriverActive;
    bool _ResidentDriverActive;
    bool _DPDLL_gunner_Loaded;
    bool _MotionSystemGunnerActive;
    bool _ResidentGunnerActive;
private slots:
    void slotSendParameters();
};

#endif // MOTIONSYSTEM_DPDLL_H
