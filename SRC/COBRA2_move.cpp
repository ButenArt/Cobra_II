#include "COBRA2_Move.h"

COBRA2_Move::COBRA2_Move(COBRA2_Engine *engine, COBRA2_Transmission *transmission)
{
    _engine = engine;
    _transmission = transmission;

    Move = new TMove();
    Move->Moving = false;
    Move->Start_Move = false;
    Move->Stop_Move = false;
    Move->Kilometrage = 0;
    Move->bodyVelocity = 0;
    Move->RealCoupling = 0;
    Move->RealBrake = 0;
    Move->torqueMultiplier = 0;
    Move->Pitch = 0;
    Move->engineTorque = 0;
    for(int i=0; i<Move->WHEELS_NUM; i++)
    {
        Move->WheelsTorque[i] = 0;
        Move->WheelsSpeed[i] = 0;
        Move->brakeTorque[i] = 0;
        Move->compensationTorque[i] = 0;
    }
}

COBRA2_Move::~COBRA2_Move()
{
    delete Move;
}

void COBRA2_Move::Init_position(int T_init)
{
    _transmission->Transmission_Reset();
    _engine->Motor_Reset(T_init);
    Move->Moving = false;
    Move->Start_Move = false;
    Move->Stop_Move = false;
    Move->Kilometrage = 0;
    Move->bodyVelocity = 0;
    Move->RealCoupling = 0;
    Move->RealBrake = 0;
    Move->torqueMultiplier = 0;
    Move->Pitch = 0;
    Move->engineTorque = 0;
    for(int i=0; i<Move->WHEELS_NUM; i++)
    {
        Move->WheelsTorque[i] = 0;
        Move->WheelsSpeed[i] = 0;
        Move->brakeTorque[i] = 0;
        Move->compensationTorque[i] = 0;
    }
}

void COBRA2_Move::Movement(float dT, float Starter)
{
    // -- завестись с толкача --
    if(!_engine->Motor->Started && Move->bodyVelocity > 7.0f && _transmission->Transmission->Coupling > 0.8f && _transmission->Transmission->Gear == 1) Starter = 15000.0f;
    // -------------------------

    _engine->Motor_Move(dT,Move->engineTorque,Starter,0);

    Move->Kilometrage = Move->Kilometrage + qAbs(Move->bodyVelocity)/3600.0f*dT*Move->CORRECTION_INTERVAL;

    bool tmpMoving = Move->Moving;
    if(_engine->Motor->Started) Move->Moving = (qAbs(Move->bodyVelocity)>0.5f); else Move->Moving = false;
    if(tmpMoving && !Move->Moving)
    {
        Move->Stop_Move = true;
        Move->Start_Move = false;
    }
    if(!tmpMoving && Move->Moving)
    {
        Move->Start_Move = true;
        Move->Stop_Move = false;
    }
}

qreal COBRA2_Move::calculatePitchTorque()
{
    if(_transmission->Transmission->Gear == 0 || _transmission->Transmission->Coupling <= Move->MAB_VIBRATIONS) return 0;

    qreal pitchForce = Move->BODY_MASS * 9.81f * qSin(Move->Pitch * M_PI / 180);
    qreal pitchTorque = pitchForce * Move->WheelR;
    qreal ratios = _transmission->getCurrentGearRatio() * _transmission->Transmission->ReducerRatio * _transmission->Transmission->AxleRatio * _transmission->Transmission->K_TRANSMISSION;
    if(_transmission->getCurrentTransferGearRatio() !=0) ratios = ratios * _transmission->getCurrentTransferGearRatio();
    qreal pitchEngineTorque = pitchTorque * _transmission->Transmission->Coupling / ratios;

    if(qAbs(Move->bodyVelocity) < 0.1f) pitchEngineTorque = 0;
    return pitchEngineTorque;
}
