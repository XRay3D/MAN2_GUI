#ifndef SCPI_H
#define SCPI_H

#include "common_interfaces.h"

#include <QSerialPort>

class SCPI : public QSerialPort, public COMMON_INTERFACES {
    Q_OBJECT
public:
    explicit SCPI(QObject* parent = 0);
    bool Ping(const QString& potName = QString());
    bool IsConnected() const;
    double GetVoltage();
    double GetCurrent();

private:
    QByteArray& WriteRead(const QByteArray& data, int timeout = 10);
    QByteArray m_data;
    int m_counter;
    bool m_isFound;
};

#endif // SCPI_H
