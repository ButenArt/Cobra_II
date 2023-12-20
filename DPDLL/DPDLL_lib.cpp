#include "DPDLL_lib.h"
#include <QLibrary>
#include <QCoreApplication>
#include <QMessageBox>


T_DPDLL_lib::T_DPDLL_lib()
{
    DPDLL_driver_Loaded = false;
    DPDLL_gunner_Loaded = false;
}

T_DPDLL_lib::~T_DPDLL_lib()
{

}

bool T_DPDLL_lib::InitDPDLL_driver()
{
    DPDLL_driver_Loaded = false;
    QString dllpath = QCoreApplication::applicationDirPath().append("/DPDLL_driver/DP.dll");
    QLibrary library(dllpath);
    if(!library.load())
    {
        QMessageBox::warning(0,"Warning", "Driver DP.dll not available !");
    }
    else
    {
        InitSimPlatf_1 = (CPP_InitSimPlatf)library.resolve("InitSimPlatf");
        UpdatePlatformState_1 = (CPP_UpdatePlatformState)library.resolve("UpdatePlatformState");
        FreeSimPlatf_1 = (CPP_FreeSimPlatf)library.resolve("FreeSimPlatf");

        SetPlatfDown_1 = (CPP_SetPlatfDown)library.resolve("SetPlatfDown");
        SetPlatfMid_1 = (CPP_SetPlatfMid)library.resolve("SetPlatfMid");
        SetPlatfData_1 = (CPP_SetPlatfData)library.resolve("SetPlatfData");

        Get_Platf_Type_1 = (CPP_Get_Platf_Type)library.resolve("Get_Platf_Type");

        if(InitSimPlatf_1)
        {
            InitSimPlatf_1();
            DPDLL_driver_Loaded = true;
        }
        else
        {
            QMessageBox::warning(0,"Warning", "Driver DP.dll incorrect load !");
        }
    }
    return DPDLL_driver_Loaded;
}

void T_DPDLL_lib::FreeDPDLL_driver()
{
    DPDLL_driver_Loaded = false;
    if(FreeSimPlatf_1) FreeSimPlatf_1();
}

void T_DPDLL_lib::SetPlatfDownDPDLL_driver()
{
    if(SetPlatfDown_1) SetPlatfDown_1();
}

void T_DPDLL_lib::SetPlatfMidDPDLL_driver()
{
    if(SetPlatfMid_1) SetPlatfMid_1();
}

void T_DPDLL_lib::SetPlatfDataDPDLL_driver()
{
    if(SetPlatfData_1) SetPlatfData_1();
}

int T_DPDLL_lib::UpdatePlatformStateDPDLL_driver(TCtrl_Input_Rec* Ctrl_Input_Rec)
{
    if(DPDLL_driver_Loaded)
    {
        if(UpdatePlatformState_1) return UpdatePlatformState_1(Ctrl_Input_Rec);
    }

    return -1;
}

WORD T_DPDLL_lib::Get_Platf_TypeDPDLL_driver()
{
    if(DPDLL_driver_Loaded)
    {
        if(Get_Platf_Type_1) return Get_Platf_Type_1();
    }

    return 65535;
}

bool T_DPDLL_lib::InitDPDLL_gunner()
{
    DPDLL_gunner_Loaded = false;
    QString dllpath = QCoreApplication::applicationDirPath().append("/DPDLL_gunner/DP.dll");
    QLibrary library(dllpath);
    if(!library.load())
    {
        QMessageBox::warning(0,"Warning", "Gunner DP.dll not available !");
    }
    else
    {
        InitSimPlatf_2 = (CPP_InitSimPlatf)library.resolve("InitSimPlatf");
        UpdatePlatformState_2 = (CPP_UpdatePlatformState)library.resolve("UpdatePlatformState");
        FreeSimPlatf_2 = (CPP_FreeSimPlatf)library.resolve("FreeSimPlatf");

        SetPlatfDown_2 = (CPP_SetPlatfDown)library.resolve("SetPlatfDown");
        SetPlatfMid_2 = (CPP_SetPlatfMid)library.resolve("SetPlatfMid");
        SetPlatfData_2 = (CPP_SetPlatfData)library.resolve("SetPlatfData");

        Get_Platf_Type_2 = (CPP_Get_Platf_Type)library.resolve("Get_Platf_Type");

        if(InitSimPlatf_2)
        {
            InitSimPlatf_2();
            DPDLL_gunner_Loaded = true;
        }
        else
        {
            QMessageBox::warning(0,"Warning", "Gunner DP.dll incorrect load !");
        }
    }
    return DPDLL_gunner_Loaded;
}

void T_DPDLL_lib::FreeDPDLL_gunner()
{
    DPDLL_gunner_Loaded = false;
    if(FreeSimPlatf_2) FreeSimPlatf_2();
}

void T_DPDLL_lib::SetPlatfDownDPDLL_gunner()
{
    if(SetPlatfDown_2) SetPlatfDown_2();
}

void T_DPDLL_lib::SetPlatfMidDPDLL_gunner()
{
    if(SetPlatfMid_2) SetPlatfMid_2();
}

void T_DPDLL_lib::SetPlatfDataDPDLL_gunner()
{
    if(SetPlatfData_2) SetPlatfData_2();
}

int T_DPDLL_lib::UpdatePlatformStateDPDLL_gunner(TCtrl_Input_Rec* Ctrl_Input_Rec)
{
    if(DPDLL_gunner_Loaded)
    {
        if(UpdatePlatformState_2) return UpdatePlatformState_2(Ctrl_Input_Rec);
    }

    return -1;
}

WORD T_DPDLL_lib::Get_Platf_TypeDPDLL_gunner()
{
    if(DPDLL_gunner_Loaded)
    {
        if(Get_Platf_Type_2) return Get_Platf_Type_2();
    }

    return 65535;
}
