#ifndef IRT59XX_H
#define IRT59XX_H

#include <QSerialPort>
#include "elemer.h"

class IRT59XX : public QSerialPort, public Elemer {
    Q_OBJECT
public:
    explicit IRT59XX(QObject* parent = 0);
    QByteArray WriteRead(const QByteArray& parcel, int timeout = 100);
};

#endif; // IRT59XX_H
