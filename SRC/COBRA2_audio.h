#ifndef COBRA2_AUDIO_H
#define COBRA2_AUDIO_H

#include <windows.h>
#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include "Common\Audio.h"


class T_COBRA2_audio : public T_Audio
{
    Q_OBJECT

public:
    T_COBRA2_audio();
    ~T_COBRA2_audio();
    int GetEngineSampleRate(int rpm);
};


#endif // COBRA2_AUDIO_H
