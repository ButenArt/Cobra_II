#ifndef COBRA2_CAN_DATA_H
#define COBRA2_CAN_DATA_H

#include "QTime"
#include "Common/CAN_common.h"
#include "Common/SettingsFile.h"

const auto N_MAB_UNITS = 8;
const auto N_VID_UNITS = 8;
const auto N_IN_UNITS = 8;
const auto N_OUT_UNITS = 8;
const auto N_POT_UNITS = 8;
const auto N_LCD_UNITS = 8;
const auto N_ENC_UNITS = 1;
const auto N_ENUM_UNITS = 2;

struct T_UNITS
{
    std::vector<TMABData> MAB;
    std::vector<TVIDData> VID;
    std::vector<TGPIOINData> GPIN;
    std::vector<TGPIOOUTData> GPOUT;
    std::vector<TPOTData> POT;
    std::vector<TENCData> ENC;
    std::vector<TLEDData> LED;
    std::vector<TLCDData> LCD;
    std::vector<TENUMData> ENUM;
};


class T_COBRE2_CAN : public CANBusComm
{
  Q_OBJECT

public:
    T_COBRE2_CAN(TSettingsFile *ini, TLogging *log);
    ~T_COBRE2_CAN();
    void AutoTXEnable();
    void AutoTXDisable();
    void AutoRXEnable();
    void AutoRXDisable();
    void HeartBeatEnable();
    void HeartBeatDisable();
    T_UNITS UNITS;
private:
    TSettingsFile *_ini;
    WORD _TxInterval;
    WORD _RxInterval;
    bool _canHeartBeat;
    void ReadCAN(BYTE port);
private slots:
    void slotTX();
    void slotRX();
    void slotHeartBeat();
};

#endif // COBRE2_CAN_DATA_H
