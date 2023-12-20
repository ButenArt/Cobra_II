#ifndef AUDIO_H
#define AUDIO_H

#include <Windows.h>
#include <QObject>
#include <QAudioOutput>
#include <QFile>
#include <QBuffer>
#include <QList>
#include <QTimer>


const BYTE N_SOUNDS = 20;


class T_Audio : public QObject
{
    Q_OBJECT

public:
    T_Audio();
    ~T_Audio();
    bool SoundsEnabled;
    void Play(BYTE num, bool play);
    void Init();
    void SetSampleRate(int source, int mutate, int samplerate);

private:
    int defaultSamleRate;
    QAudioFormat audioFormat;
    QList<QAudioOutput *> audioOutputList;
    QList<QAudioOutput *> audioTrashList;
    QList<QBuffer *> bufferList;
    QList<QByteArray *> byteArrayList;
    QTimer *trashTimer;

protected:
    struct TSounds;
    TSounds *sounds;
    void LoadSounds();

signals:
    void signalMakeSoundtrack();

private slots:
    void slotMakeSoundtrack();
    void slotClearTrash();
};

#pragma pack(push, 1)
struct T_Audio::TSounds
{
    QString URL[N_SOUNDS];     // путь к файлу
      float VOL[N_SOUNDS];     // громкость
       bool MIX[N_SOUNDS];     // звук в треке
};
#pragma pack(pop)


#endif // AUDIO_H
