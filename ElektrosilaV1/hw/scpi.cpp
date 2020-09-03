#include "scpi.h"
#include <QDebug>

SCPI::SCPI(QObject* parent)
    : QSerialPort(parent)
{
    setBaudRate(QSerialPort::Baud9600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
}

bool SCPI::ping(const QString& potName)
{
    QMutexLocker locker(&m_mutex);
    close();
    m_connected = false;
    setPortName(potName);
    if (open(ReadWrite)) {
        waitForReadyRead(100);
        setDataTerminalReady(true);
        setRequestToSend(true);
        waitForReadyRead(100);
        qDebug() << WriteRead("*IDN?");
        if (m_data.endsWith("\r\n")) {
            Write("SYSTem:REMote");
            Write("*CLS");
            //            Write("DETector:BANDwidth 3");
            m_connected = true;
        }
    }
    return m_connected;
}

QByteArray& SCPI::WriteRead(const QByteArray& data)
{
    m_data.clear();
    write(data + "\r\n");
    m_counter = 0;
    while (++m_counter < 1000 && !canReadLine())
        waitForReadyRead(10);
    m_data.append(readAll());
    return m_data;
}

double SCPI::GetDcVoltage()
{
    QMutexLocker locker(&m_mutex);
    if (IsConnected()) {
        WriteRead("MEASure:VOLTage:DC?");
        if (m_data.endsWith("\r\n"))
            return QString(m_data).toDouble();
    }
    return 0.0;
}

double SCPI::GetAcVoltage()
{
    QMutexLocker locker(&m_mutex);
    if (IsConnected()) {
        WriteRead("MEASure:VOLTage:AC?");
        if (m_data.endsWith("\r\n"))
            return QString(m_data).toDouble();
    }
    return 0.0;
}

double SCPI::GetDcCurrent()
{
    QMutexLocker locker(&m_mutex);
    if (IsConnected()) {
        WriteRead("MEASure:CURRent:DC?");
        if (m_data.endsWith("\r\n")) {
            qDebug() << "GetDcCurrent" << m_data;
            return QString(m_data).toDouble();
        }
    }
    return 0.0;
}

void SCPI::Write(const QByteArray& data)
{
    m_data.clear();
    write(data + "\r\n");
    m_counter = 0;
    while (++m_counter < 1000 && !((DataCarrierDetectSignal | DataSetReadySignal) & pinoutSignals()))
        waitForReadyRead(10);
}
