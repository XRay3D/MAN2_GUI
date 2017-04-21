#ifndef ELEMER_H
#define ELEMER_H

#include <QDebug>
#include <QtGui>
#include <stdint.h>

class Elemer {
public:
    explicit Elemer();

    QByteArray ControlSum(const QByteArray& data);
    int Check(const QByteArray& Parcel, QList<QByteArray>& Array);
};

#endif // ELEMER_H
