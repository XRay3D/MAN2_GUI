#ifndef COMMON_INTERFACES_H
#define COMMON_INTERFACES_H

#include <QObject>

class COMMON_INTERFACES {
public:
    COMMON_INTERFACES();
    virtual bool Ping(const QString& portName = QString()) = 0;
    virtual bool IsConnected() const = 0;
};

#endif // COMMON_INTERFACES_H
