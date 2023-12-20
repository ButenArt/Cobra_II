#ifndef COBRA2_UDP_OU_H
#define COBRA2_UDP_OU_H

#include "Common/UDP_OU.h"
#include "COBRA2.h"
#include "COBRA2_instructor.h"
#include "Common/SettingsFile.h"
#include "Common/Replay.h"

class TIGER_UDP_OU : public UDP_OU
{
    Q_OBJECT
  public:
    TIGER_UDP_OU(T_TIGER_Instructor *Instructor, T_TIGER_driver *driver, TSettingsFile *ini, TReplay *replay);
    ~TIGER_UDP_OU();
  private:
    T_TIGER_Instructor *_Instructor;
    T_TIGER_driver *_driver;
    TSettingsFile *_ini;
    TReplay *_replay;
    void OU_MV();
  private slots:
    void slotUDPSend();
};

#endif // COBRA2_UDP_OU_H
