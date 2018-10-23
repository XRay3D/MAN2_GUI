#ifndef SCPI_H
#define SCPI_H

#include "common_interfaces.h"

#include <QSerialPort>
#include <QMutex>

class SCPI : public QSerialPort, public CommonInterfaces {
    Q_OBJECT
public:
    explicit SCPI(QObject* parent = 0);
    bool Ping(const QString& potName = QString());
    double GetDcVoltage();
    double GetAcVoltage();
    double GetDcCurrent();

private:
    QByteArray& WriteRead(const QByteArray& data);
    void Write(const QByteArray& data);
    QByteArray m_data;
    int m_counter;
    QMutex m_mutex;
};

#endif // SCPI_H
