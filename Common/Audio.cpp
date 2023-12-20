#include "Audio.h"
#include <QDebug>
#include <QCoreApplication>
#include <QAudioFormat>

T_Audio::T_Audio()
{
    SoundsEnabled = false;
    defaultSamleRate = 22050;

    audioFormat.setSampleRate(defaultSamleRate);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);

    sounds = new TSounds;
    for(auto i=0; i<N_SOUNDS; i++)
    {
        sounds->URL[i] = ":/resource/empty.wav";
        sounds->VOL[i] = 0.0f;
        sounds->MIX[i] = false;
    }

    audioOutputList.clear();
    audioTrashList.clear();
    byteArrayList.clear();
    bufferList.clear();

    for(auto i=0; i<N_SOUNDS; i++)
    {
        QAudioOutput *effect = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), audioFormat, this);
        audioOutputList.append(effect);
        QByteArray *byteArray = new QByteArray;
        byteArrayList.append(byteArray);
        QBuffer *audiobuffer = new QBuffer;
        bufferList.append(audiobuffer);

        QFile inputFile(sounds->URL[i]);
        inputFile.open(QIODevice::ReadOnly);
        byteArrayList[i]->clear();
        byteArrayList[i]->insert(0, inputFile.readAll());
        inputFile.close();
        bufferList[i]->setBuffer(byteArrayList[i]);
        bufferList[i]->open(QIODevice::ReadOnly);
        bufferList[i]->seek(bufferList[i]->size());

        audioOutputList[i]->setVolume(sounds->VOL[i]);
        audioOutputList[i]->start(bufferList[i]);
        audioOutputList[i]->suspend();
    }

    trashTimer = new QTimer(this);
    trashTimer->setInterval(50);
    connect(trashTimer,SIGNAL(timeout()),this, SLOT(slotClearTrash()));
    trashTimer->start();

    connect(this,SIGNAL(signalMakeSoundtrack()),this,SLOT(slotMakeSoundtrack()));
}

T_Audio::~T_Audio()
{
    for(auto i=0; i<N_SOUNDS; i++)
    {
        if(audioOutputList[i])
        {
            audioOutputList[i]->stop();
            delete audioOutputList[i];
        }
        if(bufferList[i]) delete bufferList[i];
        if(byteArrayList[i]) delete byteArrayList[i];
    }
    delete sounds;
    delete trashTimer;
}

void T_Audio::Init()
{
    audioFormat.setSampleRate(defaultSamleRate);
    for(auto i=0; i<N_SOUNDS; i++)
    {
        sounds->MIX[i] = false;
    }
    emit signalMakeSoundtrack();
}

void T_Audio::Play(BYTE num, bool play)
{
    if(!SoundsEnabled) return;
    sounds->MIX[num] = play;
    emit signalMakeSoundtrack();
}

void T_Audio::slotMakeSoundtrack()
{
    for(auto i=0; i<N_SOUNDS; i++)
    {
        if(sounds->MIX[i])
        {
            if(bufferList[i]->atEnd())
            {
                bufferList[i]->seek(0);
                audioOutputList[i]->resume();
            }
            if(bufferList[i]->pos()>=bufferList[i]->size()-2)  // проигрываем не до конца, вроде как помогает убрать щелчки
            {
                audioOutputList[i]->suspend();
                bufferList[i]->seek(bufferList[i]->size());
            }
        }
        else
        {
            audioOutputList[i]->suspend();
            audioOutputList[i]->setVolume(sounds->VOL[i]);
            bufferList[i]->seek(bufferList[i]->size());
        }
    }
}

void T_Audio::LoadSounds()
{
    for(auto i=0; i<N_SOUNDS; i++)
    {
        QFile inputFile(sounds->URL[i]);
        inputFile.open(QIODevice::ReadOnly);
        byteArrayList[i]->clear();
        byteArrayList[i]->insert(0, inputFile.readAll());
        byteArrayList[i]->remove(0,50);  // обрезаем спереди чтобы убрать щелчки
        //byteArrayList[i]->chop(2);       // обрезаем сзади чтобы убрать щелчки
        inputFile.close();
        bufferList[i]->setBuffer(byteArrayList[i]);
        bufferList[i]->open(QIODevice::ReadOnly);
        bufferList[i]->seek(bufferList[i]->size());

        audioOutputList[i]->setVolume(sounds->VOL[i]);
        audioOutputList[i]->start(bufferList[i]);
        audioOutputList[i]->suspend();
    }
}

void T_Audio::SetSampleRate(int source, int mutate, int samplerate)
{
    if(!SoundsEnabled) return;
    QAudioFormat audioformat = audioOutputList[mutate]->format();
    if(audioformat.sampleRate() == samplerate) return;
    audioFormat.setSampleRate(samplerate);
    audioOutputList[source]->setVolume(sounds->VOL[source]);
    audioOutputList[mutate]->setVolume(0.0f);
    sounds->MIX[mutate] = false;
    audioTrashList.append(audioOutputList[mutate]);
    QAudioOutput *effect = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), audioFormat, this);
    effect->setVolume(sounds->VOL[mutate]);
    bufferList[mutate]->seek(bufferList[source]->pos());
    effect->start(bufferList[mutate]);
    audioOutputList[mutate] = effect;
    sounds->MIX[mutate] = true;
    audioOutputList[mutate]->resume();
    audioOutputList[source]->setVolume(0.0f);
}

void T_Audio::slotClearTrash()
{
    if(!audioTrashList.isEmpty())
    {
        delete audioTrashList.takeFirst();
    }
}
