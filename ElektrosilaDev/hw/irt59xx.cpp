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
    if (m_connected) {
        data += Elemer::ControlSum(parcel) + '\r';
        qDebug() << "write" << data;
        write(data);
        data.clear();
        do {
            waitForReadyRead(10);
            timeout -= 10;
            QCoreApplication::processEvents(QEventLoop::AllEvents /*, 10*/);
            data.append(readAll());
        } while (!data.contains('\r') && timeout > 0);
        qDebug() << "read" << data;
    }
    return data;
}

bool IRT59XX::GetViltage(double& val)
{
    QMutexLocker Locker(&m_mutex);
    if (m_connected) {
        if (Check(WriteRead(":1;5;0;"), m_array)) {
            if (Check(WriteRead(":1;6;"), m_array)) {
                QByteArray data;
                for (int i = 0; i < 4; ++i) {
                    if (Check(WriteRead(":1;6;"), m_array) > 1) {
                        data.append(m_array.at(1).toInt());
                    }
                    else {
                        return false;
                    }
                }
                val = *(float*)data.data();
                emit GetViltageSignal(val);
                return true;
            }
        }
    }
    return false;
}

bool IRT59XX::Ping(const QString& portName)
{
    QMutexLocker Locker(&m_mutex);
    m_connected = true;
    close();
    if (!portName.isEmpty()) {
        emit setPortName(portName);
    }
    thread()->usleep(100);
    if (open(QSerialPort::ReadWrite)) {
        thread()->usleep(100);
        setDataTerminalReady(false);
        setRequestToSend(true);
        if (Check(WriteRead(":1;0;"), m_array) > 1) {
            if (m_array.size() > 1 && m_array.at(1).toInt() == 9) {
                //                double val;
                //                qDebug() << GetViltage(val) << val;
                return m_connected;
            }
        }
    }
    close();
    qDebug() << errorString();
    m_connected = false;
    return m_connected;
}

bool IRT59XX::IsConnected() const
{
    return m_connected;
}
