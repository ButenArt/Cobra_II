#ifndef DPDLL_LIB_H
#define DPDLL_LIB_H

#include <Windows.h>

#pragma pack(push, 1)
struct TCtrl_Input_Rec
{
    double Angl_X;      //угол поворотоа в радианах
    double Angl_Y;      //угол поворотоа в радианах
    double Angl_H;      //угол поворотоа в радианах
    double Angl_V_X;    //уголовая скорость в радианах/сек
    double Angl_V_Y;    //уголовая скорость в радианах/сек
    double Angl_V_H;    //уголовая скорость в радианах/сек
    double Acsel_X;     //линейное ускорение в g
    double Acsel_Y;     //линейное ускорение в g
    double Acsel_H;     //линейное ускорение в g
    double V_lim;       //ограничение скорости приводов 0..1 от максимальной
    bool Acsel;         //true-задаем линейные ускорения, false - задаем линейные перемещения
    bool Breack;        //включить тормоз
};
#pragma pack(pop)


class T_DPDLL_lib
{
public:
    T_DPDLL_lib();
    ~T_DPDLL_lib();

    bool InitDPDLL_driver();
    void FreeDPDLL_driver();
    int UpdatePlatformStateDPDLL_driver(TCtrl_Input_Rec*);
    void SetPlatfDownDPDLL_driver();
    void SetPlatfMidDPDLL_driver();
    void SetPlatfDataDPDLL_driver();
    WORD Get_Platf_TypeDPDLL_driver();

    bool InitDPDLL_gunner();
    void FreeDPDLL_gunner();
    int UpdatePlatformStateDPDLL_gunner(TCtrl_Input_Rec*);
    void SetPlatfDownDPDLL_gunner();
    void SetPlatfMidDPDLL_gunner();
    void SetPlatfDataDPDLL_gunner();
    WORD Get_Platf_TypeDPDLL_gunner();

private:
    bool DPDLL_driver_Loaded;
    bool DPDLL_gunner_Loaded;

    typedef int (__stdcall *CPP_InitSimPlatf)();
    CPP_InitSimPlatf InitSimPlatf_1;
    CPP_InitSimPlatf InitSimPlatf_2;

    typedef int (__stdcall *CPP_UpdatePlatformState)(TCtrl_Input_Rec*);
    CPP_UpdatePlatformState UpdatePlatformState_1;
    CPP_UpdatePlatformState UpdatePlatformState_2;

    typedef int (__stdcall *CPP_FreeSimPlatf)();
    CPP_FreeSimPlatf FreeSimPlatf_1;
    CPP_FreeSimPlatf FreeSimPlatf_2;

    typedef int (__stdcall *CPP_SetPlatfDown)();
    CPP_SetPlatfDown SetPlatfDown_1;
    CPP_SetPlatfDown SetPlatfDown_2;

    typedef int (__stdcall *CPP_SetPlatfMid)();
    CPP_SetPlatfMid SetPlatfMid_1;
    CPP_SetPlatfMid SetPlatfMid_2;

    typedef int (__stdcall *CPP_SetPlatfData)();
    CPP_SetPlatfData SetPlatfData_1;
    CPP_SetPlatfData SetPlatfData_2;

    typedef int (__stdcall *CPP_Get_Platf_Type)();
    CPP_Get_Platf_Type Get_Platf_Type_1;
    CPP_Get_Platf_Type Get_Platf_Type_2;
};

#endif // DPDLL_LIB_H
