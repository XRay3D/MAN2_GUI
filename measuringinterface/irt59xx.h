#ifndef IRT59XX_H
#define IRT59XX_H

#include <QSerialPort>
#include "common_interfaces.h"
#include "elemer.h"

class IRT59XX : public QSerialPort, public Elemer, public COMMON_INTERFACES {
    Q_OBJECT
public:
    explicit IRT59XX(QObject* parent = 0);
    QByteArray WriteRead(const QByteArray& parcel, int timeout = 100);
    bool Ping(const QString& portName = QString()) { return false; }
    bool IsConnected() const { return false; }
};

#endif; // IRT59XX_H
