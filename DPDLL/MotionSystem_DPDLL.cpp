#include "MotionSystem_DPDLL.h"

T_DPDLL::T_DPDLL(T_Server *Server, T_Imager *Imager, T_Instructor *Instructor)
{
    _Server = Server;
    _Imager = Imager;
    _Instructor = Instructor;
    _DPDLL_driver_Loaded = false;
    _MotionSystemDriverActive = false;
    _ResidentDriverActive = false;
    _DPDLL_gunner_Loaded = false;
    _MotionSystemGunnerActive = false;
    _ResidentGunnerActive = false;
    platformType_driver = 0;
    platformType_gunner = 0;
    DPDLL_lib = new T_DPDLL_lib();
    DPDLLTimer = new QTimer(this);
    DPDLLTimer->setTimerType(Qt::PreciseTimer);
    DPDLLTimer->setInterval(55);
    connect(DPDLLTimer,SIGNAL(timeout()),this, SLOT(slotSendParameters()));
    DPDLLTimer->stop();
}

T_DPDLL::~T_DPDLL()
{
    DPDLL_lib->FreeDPDLL_driver();
    DPDLL_lib->FreeDPDLL_gunner();
    delete DPDLL_lib;
    delete DPDLLTimer;
}

void T_DPDLL::Init_driver()
{
    _DPDLL_driver_Loaded = DPDLL_lib->InitDPDLL_driver();
    if(_DPDLL_driver_Loaded)
    {
        platformType_driver = DPDLL_lib->Get_Platf_TypeDPDLL_driver();
        DPDLLTimer->start();
    }
}

void T_DPDLL::Init_gunner()
{
    _DPDLL_gunner_Loaded = DPDLL_lib->InitDPDLL_gunner();
    if(_DPDLL_gunner_Loaded)
    {
        platformType_gunner = DPDLL_lib->Get_Platf_TypeDPDLL_gunner();
        DPDLLTimer->start();
    }
}

void T_DPDLL::SetPlatformDown_driver()
{
    if(_DPDLL_driver_Loaded)
    {
        DPDLL_lib->SetPlatfDownDPDLL_driver();
    }
}

void T_DPDLL::SetPlatformDown_gunner()
{
    if(_DPDLL_gunner_Loaded)
    {
        DPDLL_lib->SetPlatfDownDPDLL_gunner();
    }
}

void T_DPDLL::SetPlatformData_driver()
{
    if(_DPDLL_driver_Loaded)
    {
        DPDLL_lib->SetPlatfDataDPDLL_driver();
    }
}

void T_DPDLL::SetPlatformData_gunner()
{
    if(_DPDLL_gunner_Loaded)
    {
        DPDLL_lib->SetPlatfDataDPDLL_gunner();
    }
}

void T_DPDLL::slotSendParameters()
{
    TCtrl_Input_Rec Ctrl_Input_Rec;
    int dll_driver_result = 0;
    int dll_gunner_result = 0;
    if(_Instructor->exerciseStarted && _Server->ResetPowerTickCounter == 0)
    {
        Ctrl_Input_Rec.Angl_X = _Imager->ImagerState->Roll * M_PI / 180;    // крен - переводим градусы в радианы
        Ctrl_Input_Rec.Angl_Y = _Imager->ImagerState->Pitch * M_PI / 180;   // тангаж - переводим градусы в радианы
        Ctrl_Input_Rec.Angl_H = _Imager->ImagerState->Course * M_PI / 180;  // направление - переводим градусы в радианы
        Ctrl_Input_Rec.Angl_V_X = 0;
        Ctrl_Input_Rec.Angl_V_Y = 0;
        Ctrl_Input_Rec.Angl_V_H = 0;
        Ctrl_Input_Rec.Acsel_X = 0;
        Ctrl_Input_Rec.Acsel_Y = 0;
        Ctrl_Input_Rec.Acsel_H = 0;
        Ctrl_Input_Rec.V_lim = 0;
        Ctrl_Input_Rec.Acsel = false;
        Ctrl_Input_Rec.Breack = false;
        //Обновление состояния платформы
        if(_DPDLL_driver_Loaded) dll_driver_result = DPDLL_lib->UpdatePlatformStateDPDLL_driver(&Ctrl_Input_Rec);
        if(_DPDLL_gunner_Loaded) dll_gunner_result = DPDLL_lib->UpdatePlatformStateDPDLL_gunner(&Ctrl_Input_Rec);
    }
    else
    {
        Ctrl_Input_Rec.Angl_X = 0;
        Ctrl_Input_Rec.Angl_Y = 0;
        Ctrl_Input_Rec.Angl_H = 0;
        Ctrl_Input_Rec.Angl_V_X = 0;
        Ctrl_Input_Rec.Angl_V_Y = 0;
        Ctrl_Input_Rec.Angl_V_H = 0;
        Ctrl_Input_Rec.Acsel_X = 0;
        Ctrl_Input_Rec.Acsel_Y = 0;
        Ctrl_Input_Rec.Acsel_H = -1000;
        Ctrl_Input_Rec.V_lim = 0;
        Ctrl_Input_Rec.Acsel = false;
        Ctrl_Input_Rec.Breack = false;
        //Обновление состояния платформы
        if(_DPDLL_driver_Loaded) dll_driver_result = DPDLL_lib->UpdatePlatformStateDPDLL_driver(&Ctrl_Input_Rec);
        if(_DPDLL_gunner_Loaded) dll_gunner_result = DPDLL_lib->UpdatePlatformStateDPDLL_gunner(&Ctrl_Input_Rec);
    }

    switch(dll_driver_result)
    {
        case 0:
        {
            _MotionSystemDriverActive = false;
            _ResidentDriverActive = false;
        }
        break;
        case 2:
        {
            _MotionSystemDriverActive = false;
            _ResidentDriverActive = true;
        }
        break;
        case 3:
        {
            _MotionSystemDriverActive = true;
            _ResidentDriverActive = true;
        }
        break;
        default:
        {
            _MotionSystemDriverActive = false;
            _ResidentDriverActive = false;
        }
        break;
    }

    switch(dll_gunner_result)
    {
        case 0:
        {
            _MotionSystemGunnerActive = false;
            _ResidentGunnerActive = false;
        }
        break;
        case 2:
        {
            _MotionSystemGunnerActive = false;
            _ResidentGunnerActive = true;
        }
        break;
        case 3:
        {
            _MotionSystemGunnerActive = true;
            _ResidentGunnerActive = true;
        }
        break;
        default:
        {
            _MotionSystemGunnerActive = false;
            _ResidentGunnerActive = false;
        }
        break;
    }
}
