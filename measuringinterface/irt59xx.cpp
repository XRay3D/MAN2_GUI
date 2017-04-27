#include "irt59xx.h"
#include <QCoreApplication>

IRT59XX::IRT59XX(QObject* parent)
    : QSerialPort(parent)
{
    setBaudRate(QSerialPort::Baud9600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
}

QByteArray IRT59XX::WriteRead(const QByteArray& parcel, int timeout)
{
    QByteArray data(parcel);
    if (isOpen()) {
        data += Elemer::ControlSum(parcel) + '\r';
        qDebug() << data;
        write(data);
        data.clear();
        do {
            waitForReadyRead(10);
            timeout -= 10;
            QCoreApplication::processEvents(QEventLoop::AllEvents /*, 10*/);
            data.append(readAll());
        } while (!data.contains('\r') && timeout > 0);
        qDebug() << data;
    }
    return data;
}

double IRT59XX::GetViltage()
{
    QMutexLocker Locker(&m_mutex);
    if (isOpen()) {
        QByteArray data;
        Check(WriteRead(":1;5;0;"), m_array);
        Check(WriteRead(":1;6;"), m_array);
        for (int i = 0; i < 4; ++i) {
            if (Check(WriteRead(":1;6;"), m_array) > 1) {
                data.append(m_array.at(1).toInt());
            }
        }
        return *(float*)data.data();
    }
    return -9999;
}

bool IRT59XX::Ping(const QString& portName)
{
    QMutexLocker Locker(&m_mutex);
    m_connected = false;
    close();
    if (!portName.isEmpty()) {
        emit setPortName(portName);
    }
    if (open(QSerialPort::ReadWrite)) {
        if (Check(WriteRead(":1;0;"), m_array) > 1) {
            if (m_array.at(1).toInt() == 9) {
                m_connected = true;
                return m_connected;
            }
        }
    }
    close();
    qDebug() << errorString();
    return m_connected;
}

bool IRT59XX::IsConnected() const
{
    return m_connected;
}
