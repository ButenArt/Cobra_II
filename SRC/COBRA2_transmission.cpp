#include "COBRA2_transmission.h"

COBRA2_Transmission::COBRA2_Transmission()
{
    //gearRatios << -7.38 << 0 << 7.82 << 4.03 << 2.5 << 1.53 << 1.0;
    gearRatios << -9.0f << 0.0f << 8.5f << 10.3f << 9.8f << 11.8f << 15.0f;
    gearMaxVelocities << -11.7f << 0 << 15.0f << 30.0f << 50.0f << 80.0f << 120.0f;
    gearMinVelocities << 0 << 0 << 0 << 0 << 10.0f << 20.0f << 30.0f;
    gearTransferRatios << 1.429f << 0 << 0.667f;

    Transmission = new TTransmission();
    Transmission->HandBrake = 0;
    Transmission->Brake = 0;
    Transmission->Gear = 0;
    Transmission->TransferGear = 0;
    Transmission->GearDivider = false;
    Transmission->Coupling = 0;
    Transmission->DiffLock = false;
}

COBRA2_Transmission::~COBRA2_Transmission()
{
    delete Transmission;
}

void COBRA2_Transmission::Transmission_Reset()
{
    Transmission->HandBrake = 0;
    Transmission->Brake = 0;
    Transmission->Gear = 0;
    Transmission->TransferGear = 0;
    Transmission->GearDivider = false;
    Transmission->Coupling = 0;
    Transmission->DiffLock = false;
}

void COBRA2_Transmission::Transmission_SetCoupling(float coupling)
{
    if(coupling<0) coupling = 0;
    if(coupling>1) coupling = 1;
    Transmission->Coupling = coupling;
}

void COBRA2_Transmission::Transmission_SetBrake(float brake)
{
    if(brake<0) brake = 0;
    if(brake>1) brake = 1;
    Transmission->Brake = brake;
}

void COBRA2_Transmission::Transmission_SetDiff(bool diffLock)
{
    Transmission->DiffLock = diffLock;
}

void COBRA2_Transmission::Transmission_SetGear(int gear)
{
    if(gear!=Transmission->Gear)
        if((gear >= -1) && (gear <= 5)) Transmission->Gear = gear;
}

void COBRA2_Transmission::Transmission_SetTransferGear(int gear)
{
    if(gear!=Transmission->TransferGear)
        if((gear >= -1) && (gear <= 1)) Transmission->TransferGear = gear;
}

float COBRA2_Transmission::getCurrentGearRatio()
{
    return gearRatios[Transmission->Gear+1];
}

float COBRA2_Transmission::getCurrentTransferGearRatio()
{
    return gearTransferRatios[Transmission->TransferGear+1];
}

float COBRA2_Transmission::getCurrentGearMaxVelocity()
{
    return gearMaxVelocities[Transmission->Gear+1];
}

float COBRA2_Transmission::getCurrentGearMinVelocity()
{
    return gearMinVelocities[Transmission->Gear+1];
}
