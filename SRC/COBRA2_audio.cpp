#include "COBRA2_audio.h"

T_COBRA2_audio::T_COBRA2_audio()
{
    sounds->URL[0] = QCoreApplication::applicationDirPath().append("/wav/beep.wav");
    sounds->VOL[0] = 1.0f;
    sounds->MIX[0] = false;
    sounds->URL[1] = QCoreApplication::applicationDirPath().append("/wav/starter.wav");
    sounds->VOL[1] = 1.0f;
    sounds->MIX[1] = false;
    sounds->URL[2] = QCoreApplication::applicationDirPath().append("/wav/start.wav");
    sounds->VOL[2] = 1.0f;
    sounds->MIX[2] = false;
    sounds->URL[3] = QCoreApplication::applicationDirPath().append("/wav/motor.wav");
    sounds->VOL[3] = 1.0f;
    sounds->MIX[3] = false;
    sounds->URL[4] = QCoreApplication::applicationDirPath().append("/wav/vent.wav");
    sounds->VOL[4] = 1.0f;
    sounds->MIX[4] = false;
    sounds->URL[5] = QCoreApplication::applicationDirPath().append("/wav/preheater_flame.wav");
    sounds->VOL[5] = 1.0f;
    sounds->MIX[5] = false;
    sounds->URL[6] = QCoreApplication::applicationDirPath().append("/wav/pump_rear.wav");
    sounds->VOL[6] = 1.0f;
    sounds->MIX[6] = false;
    sounds->URL[7] = QCoreApplication::applicationDirPath().append("/wav/blower.wav");
    sounds->VOL[7] = 1.0f;
    sounds->MIX[7] = false;
    sounds->URL[8] = QCoreApplication::applicationDirPath().append("/wav/motor.wav");
    sounds->VOL[8] = 1.0f;
    sounds->MIX[8] = false;

    if(QDir().exists(QCoreApplication::applicationDirPath().append("/wav/"))) LoadSounds();
}

T_COBRA2_audio::~T_COBRA2_audio()
{

}

int T_COBRA2_audio::GetEngineSampleRate(int rpm)
{
    int samplerate = 8000;
    if(rpm<=700) samplerate = 8000;
    if(rpm>700 && rpm<=800) samplerate = 11025;
    if(rpm>800 && rpm<=1000) samplerate = 12000;
    if(rpm>1000 && rpm<=1400) samplerate = 16000;
    if(rpm>1400 && rpm<=1800) samplerate = 22050;
    if(rpm>1800 && rpm<=2000) samplerate = 24000;
    if(rpm>2000 && rpm<=2200) samplerate = 32000;
    if(rpm>2200) samplerate = 44100;
    return samplerate;
}
