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
    if (open(ReadWrite)) {
        data += Elemer::ControlSum(parcel) + '\r';
        write(data);
        data.clear();
        do {
            waitForReadyRead(10);
            timeout -= 10;
            QCoreApplication::processEvents(QEventLoop::AllEvents/*, 10*/);
            data.append(readAll());
        } while (!data.contains('\r') && timeout > 0);
        close();
    }
    return data;
}
