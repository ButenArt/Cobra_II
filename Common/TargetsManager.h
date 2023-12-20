#ifndef TARGETSMANAGER_H
#define TARGETSMANAGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "Imager.h"
#include "Instructor.h"
#include "Logging.h"


class T_TargetsManager : public QObject
{
    Q_OBJECT
public:
    explicit T_TargetsManager(TLogging *log, QObject *parent = 0);
    ~T_TargetsManager();
    bool commandMoveForward;           // команда "Вперед" визуализацией обработана
protected:
    T_Imager *_Imager;
    T_Instructor *_Instructor;
    TLogging *_log;
private:
    void TargetsManagerStart();
    void TargetsManagerStop();
    QDateTime startingDateTime;
    QTimer *UpdateTimer;
    QTimer *ManagerTimer;
    int secsFromStart;
private slots:
    void slotUpdateManager();
    void slotTargetsManager();
};


#endif // TARGETSMANAGER_H
