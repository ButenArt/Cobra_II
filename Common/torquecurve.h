#ifndef TORQUECURVE_H
#define TORQUECURVE_H

#include <QMap>
#include <QtCore>

#define sign(a) (a<0?-1:a==0?0:a>0)

class TorqueCurve
{
public:
    TorqueCurve();
    void init(QString filename);
    float getTorque(int rpm);
    float getMaxTorque() const;

private:
    int minRpms = 100;
    int maxRpms = 1000;
    float maxTorque = 0;

    QMap<quint32, qreal> torqueData;
};

#endif // ENGINETORQUE_H
