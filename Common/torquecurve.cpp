#include "torquecurve.h"
#include <QTextStream>
#include <QFile>

TorqueCurve::TorqueCurve()
{
    float* torqueData;
}

void TorqueCurve::init(QString filename)
{
    QStringList torqueList;
    torqueList.clear();

    QFile f(filename);
    if (f.open(QIODevice::ReadOnly))
    {
        //file opened successfully
        QString data;
        data = f.readAll();
        torqueList = data.split(',');

        f.close();
    }

    if(torqueList.length() < 3)
        return;

    minRpms = torqueList.at(0).toInt();
    maxRpms = torqueList.at(1).toInt();

    for(auto i = 2; i < torqueList.length(); i++) {
        float torque = torqueList.at(i).toFloat();
        torqueData[i + minRpms - 2] = torque;
        if (maxTorque < torque)
            maxTorque = torque;
    }
}

float TorqueCurve::getTorque(int rpm)
{
    if (rpm < minRpms) return 0;
    if (rpm > maxRpms) rpm = maxRpms;

    return torqueData[rpm];
}

float TorqueCurve::getMaxTorque() const
{
    return maxTorque;
}
