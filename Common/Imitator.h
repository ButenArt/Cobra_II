#ifndef IMITATOR_H
#define IMITATOR_H

#include <QObject>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
} ControlsImitatorTCPHeader;
#pragma pack(pop)

class T_ControlsImitator : public QObject
{
    Q_OBJECT

public:
    bool ControlsImitatorActive;
    //заголовок пакетов//
    ControlsImitatorTCPHeader PacketHeader;

    explicit T_ControlsImitator(QObject *parent = 0);
    ~T_ControlsImitator();
    virtual void Init();
};

#endif // IMITATOR_H
