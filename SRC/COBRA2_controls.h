#ifndef COBRA2_CONTROLS_H
#define COBRA2_CONTROLS_H

#include "Common/Controls.h"
#include "COBRA2_can_data.h"
#include "COBRA2.h"
#include "COBRA2_onboard.h"
#include "COBRA2_move.h"
#include "COBRA2_server.h"
#include "COBRA2_imager.h"
#include "COBRA2_instructor.h"
#include "COBRA2_motionsystem.h"
#include "COBRA2_serviceerrors.h"
#include "Common/SettingsFile.h"
#include "Common/Replay.h"
#include "DPDLL/MotionSystem_DPDLL.h"

//const static BYTE N_PARAM_IN = 64;
//const static BYTE N_PARAM_OUT = 64;
//const static BYTE N_PARAM_MAB = 4;
//const static BYTE N_PARAM_VID = 10;
//const static BYTE N_PARAM_ENUM = 1;
//const static BYTE N_PARAM_POTENTIOMETER = 1;

constexpr auto N_PARAM_IN = 64;
constexpr auto N_PARAM_OUT = 64;
constexpr auto N_PARAM_MAB = 8;
constexpr auto N_PARAM_VID = 16;
constexpr auto N_PARAM_POT = 8;
constexpr auto N_PARAM_ENUM = 8;

class T_COBRA2_controls : public T_Controls
{
    Q_OBJECT
  public:
    T_COBRA2_controls(T_COBRE2_CAN *can, T_COBRA2_Server *Server, T_COBRA2_Imager *Imager, T_COBRA2_Instructor *Instructor, T_COBRA2_driver *driver, COBRA2_Move *move, TSettingsFile *ini, TReplay *replay, T_COBRA2_ServiceErrors *errors, T_COBRA2_MotionSystem *MotionSystem, T_DPDLL *dpdll);
    ~T_COBRA2_controls();
    //QString GetVIDaddr(BYTE param_id);
    //QString GetVIDCaption(BYTE param_id);
    //float GetVIDValue(BYTE param_id);
    //WORD GetVIDSteps(BYTE param_id);
    //void SetVIDSteps(BYTE param_id);
    //QString GetMABaddr(BYTE param_id);
    //QString GetMABCaption(BYTE param_id);
    //bool MABOutOfRange(BYTE param_id);
    //WORD GetMABValue(BYTE param_id);
    //float GetMABNormalValue(BYTE param_id);
    //float MABNormalValue(BYTE param_id);
    //float DualMABNormalValue(BYTE rude_param_id, BYTE fine_param_id);
    //WORD GetGPIOINaddr(BYTE unitnum);
    //BYTE GetGPIOINrow(BYTE unitnum, BYTE rownum);
    //WORD GetGPIOOUTaddr(BYTE unitnum);
    //BYTE GetGPIOOUTrow(BYTE unitnum, BYTE rownum);
    //QString GetENUMaddr(BYTE param_id);
    //QString GetENUMCaption(BYTE param_id);
    //bool ENUMOutOfRange(BYTE param_id);
    //SHORT GetENUMValue(BYTE param_id);
    //float GetENUMNormalValue(BYTE param_id);
    //float ENUMNormalValue(BYTE param_id);
    //QString GetPOTaddr(BYTE param_id);
    //QString GetPOTCaption(BYTE param_id);
    //bool POTOutOfRange(BYTE param_id);
    //WORD GetPOTValue(BYTE param_id);
    //float GetPOTNormalValue(BYTE param_id);
    //float POTNormalValue(BYTE param_id);
    T_UNITS GetUnitsData();
    void CanHeartBeat();
    void PrintTumblers( );
    void PrintParametrs( );
private:
    //T_ParamsList *ParamsList;
    T_COBRA2_Server *_Server;
    T_COBRA2_Imager *_Imager;
    T_COBRA2_Instructor *_Instructor;
    T_COBRA2_MotionSystem *_MotionSystem;
    T_COBRE2_CAN *_can;
    T_COBRA2_driver *_driver;
    T_COBRA2_Onboard *_OnBoard;
    COBRA2_Move *_move;
    TSettingsFile *_ini;
    TReplay *_replay;
    T_COBRA2_ServiceErrors *_errors;
    T_DPDLL *_dpdll;
    void ComputeParameters();
    void ValidateParameters();
    //bool GetGPIOINBitValue(BYTE param_id);
    //void SetGPIOOUTBitValue(bool value, BYTE param_id);
    void ReadLogicINI(QString INIfilename);
  private slots:
    void slotValidateParameters();
  public slots:

};


#endif // COBRA2_CONTROLS_H
