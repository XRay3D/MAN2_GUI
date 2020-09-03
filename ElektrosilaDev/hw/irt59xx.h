#ifndef IRT59XX_H
#define IRT59XX_H

#include <QSerialPort>
#include "common_interfaces.h"
#include "elemer.h"

class IRT59XX : public QSerialPort, public Elemer, public COMMON_INTERFACES {
    Q_OBJECT
public:
    explicit IRT59XX(QObject* parent = 0);
    bool GetViltage(double & val);
    bool Ping(const QString& portName = QString());
    bool IsConnected() const;

signals:
    void GetViltageSignal(double val);

private:
    QByteArray WriteRead(const QByteArray& parcel, int timeout = 100);
    QMutex m_mutex;
    QList<QByteArray> m_array;
    bool m_connected = false;
};

#endif // IRT59XX_H
